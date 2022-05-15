#include "StreamReceiver.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

// Standard C Lib
//#include "opencv2/opencv.hpp"
//#include <chrono>
//#include <iostream>
//#include <map>
//#include <thread>

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/system/system_error.hpp>

#include <NNBSSNetwork/Kit/CountdownTimer.h>// Move to respective places


#include <NNBSSNetwork/Kit/Logger.h> // Move to Respective Places
#include <NNBSSNetwork/Kit/OpenCVUtils.h>// Move to NNBSS Render Utils
#include <NNBSSNetwork/Kit/TimeLogger.h> // Move to respective places

#include <NNBSSNetwork/Kit/VideoWindow.h>


struct FrameStitcher
{
	explicit FrameStitcher(int parts_num_)
		: _parts_num(parts_num_), _image_buffer(5 * MB)
	{
	}

	FrameStitcher(FrameStitcher&&) = default;

	void add(const InputBuffer::Header& h_, ::asio::const_buffer part_)
	{
		if (_parts_num <= 0)
			return;
		assert(_parts_num > 0);
		assert(h_.part_begin + part_.size() <= _image_buffer.size());

		memcpy(_image_buffer.data() + h_.part_begin, part_.data(), part_.size());
		_parts_num--;
	}

	bool is_complete() const { return _parts_num == 0; }

	cv::Mat decoded() const
	{
		assert(is_complete());
		return cv::imdecode(_image_buffer, cv::IMREAD_UNCHANGED);
	}

private:
	int _parts_num;
	std::vector<uchar> _image_buffer;
};

struct FramesManager
{
	void add(const InputBuffer::Header& h_, ::asio::const_buffer part_)
	{
		const int old_frame_allowance = 10;
		if (h_.frame_id < _last_frame_id - old_frame_allowance ||
			h_.frame_id < _last_complete_frame)
		{
			// Too old, throw it away
			Logger::Debug("Got old frame", h_.frame_id, ". Discarded");
			return;
		}
		else
		{
			Logger::Debug("Got frame id", h_.frame_id, ". Current last frame id", _last_frame_id);
			// Throw away the old one and start with the new one
			_last_frame_id = std::max(_last_frame_id, h_.frame_id);
			_frames.erase(_last_frame_id - old_frame_allowance);
		}

		auto frameIter = _frames.find(h_.frame_id);
		if (frameIter == _frames.cend())
		{
			// TimeLogger t("New frame found: " + std::to_string(h_.frame_id),
			//            std::cout);
			frameIter = _frames
				.insert(std::make_pair(h_.frame_id,
					FrameStitcher(h_.total_parts)))
				.first;
		}

		auto& frameStitcher = frameIter->second;

		frameStitcher.add(h_, part_);

		if (frameStitcher.is_complete())
		{
			if (h_.frame_id >= _last_frame_id)
			{
				_last_complete_frame = h_.frame_id;
			}
			else
			{
				// It's stale, throw it away
				_frames.erase(h_.frame_id);
			}
		}
	}

	bool is_frame_ready() const { return _last_complete_frame != -1; }

	cv::Mat get_last_frame()
	{
		assert(_last_complete_frame > -1);
		FrameStitcher frame_stitcher = std::move(_frames.at(_last_complete_frame));
		Logger::Debug("Decoded frame", _last_complete_frame);

		// Clean all old frames
		for (int i = _last_cleaned; i < _last_complete_frame; ++i)
		{
			_frames.erase(i);
		}

		_last_complete_frame = -1;
		return frame_stitcher.decoded();
	}

private:
	int _last_cleaned{};
	int _last_complete_frame{ -1 };
	int _last_frame_id{};
	std::map<int, FrameStitcher> _frames;
};

struct Handler
{
	Handler(boost::asio::ip::udp::socket& socket_,
		InputBuffer& input_buffer_,
		LockFreeSpsc<InputBuffer>& disruptor_)
		: _socket(socket_),
		_input_buffer(input_buffer_),
		_disruptor(disruptor_)
	{
	}
	void operator()(system::error_code err, std::size_t recv_size_)
	{
		if (err)
		{
			Logger::Error("Recv Err ", err.message());
		}
		else
		{
			static bool frame_dropped{};
			InputBuffer::Header header = _input_buffer.get_header();

			if (header.part_id == 0)
			{
				// Try again with new frame
				frame_dropped = false;
			}

			if (!frame_dropped && !_disruptor.try_push(std::move(_input_buffer)))
			{
				// Couldn't insert this part, let's skip all the rest of the parts
				// until the next frame
				frame_dropped = true;
			}

			_socket.async_receive(_input_buffer.data(), *this);

			static CountdownTimer timer(std::chrono::milliseconds(1000));
			static int64_t recv_bytes_per_second = 0;

			recv_bytes_per_second += recv_size_;

			// Output Stats every second
			if (timer.is_it_time_yet())
			{
				Logger::Info(
					"Streaming Rate", recv_bytes_per_second / 1000.f, "KB/s");
				recv_bytes_per_second = 0;
			}
		}
	}

private:
	boost::asio::ip::udp::socket& _socket;
	InputBuffer& _input_buffer;
	LockFreeSpsc<InputBuffer>& _disruptor;
};

CNNBSSStreamReceiver::CNNBSSStreamReceiver(SNNBSSMVMData* params)
	: p_MVM(params),
	m_Resolver(ioContext), m_Recv_socket(ioContext)
	, m_disruptor(new LockFreeSpsc<InputBuffer>(10000))
{
}

CNNBSSStreamReceiver::~CNNBSSStreamReceiver()
{
}

bool CNNBSSStreamReceiver::Setup()
{
	std::string recv_address_; int port = -1;
	try
	{
		std::string connectionAddress = p_MVM->m_MVMSourceAddress.ToStdString();

		if (connectionAddress.find(':') == std::string::npos)
			return false;

		// Check address
		recv_address_ = connectionAddress.substr(0, connectionAddress.find(':'));

		// Check port
		port = std::stoi(connectionAddress.substr(connectionAddress.find(':')+1, connectionAddress.size()));

		if (recv_address_.empty() || port == -1)
			return false;
	}
	catch (...)
	{
		return false;
	}

	//wxLogMessage("address: %s port: %i", recv_address_,port);

	try
	{
		InputBuffer input_buffer;

		boost::asio::ip::udp::endpoint recv_endpoint(
			boost::asio::ip::address::from_string(recv_address_), port);
		m_Recv_socket.open(boost::asio::ip::udp::v4());

		m_Recv_socket.bind(recv_endpoint);
		LockFreeSpsc<InputBuffer>* test;
		Handler handler(m_Recv_socket, input_buffer, *m_disruptor);

		m_Recv_socket.async_receive(input_buffer.data(), handler);

		Logger::Info("Waiting for connections on",
			recv_endpoint.address().to_string(),
			recv_endpoint.port());

		ioContext.run();
	}
	catch (const std::exception& e_)
	{
		Logger::Error("Receiver Error: ", e_.what());
		return false;
	}
	catch (...)
	{
		Logger::Error("Receiver Error: ");
		return false;
	}

	return true;
}

cv::Mat CNNBSSStreamReceiver::GetCurrentFrame()
{
	try
	{
		FramesManager frame_manager;
		cv::Mat frame;

		InputBuffer part_buf;
		while (m_disruptor->try_pop(part_buf))
		{
			auto [header, part] = part_buf.parse();
			Logger::Debug("Received", header);

			frame_manager.add(header, part);

			if (frame_manager.is_frame_ready())
			{
				Logger::Debug("Updating Frame");
				frame = frame_manager.get_last_frame();
			}
		}

		return frame;
	}
	catch (const std::exception& e_)
	{
		Logger::Error("Receiver Error: ", e_.what());
	}

	return cv::Mat();
}

void CNNBSSStreamReceiver::Terminate()
{
	m_Recv_socket.close();
}
