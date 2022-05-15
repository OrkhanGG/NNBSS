#ifndef STREAMRECEIVER_H
#define STREAMRECEIVER_H

namespace cv
{
	class Mat;
}

// NNBSS Network(Temporal)
#include <NNBSSNetwork/Kit/InputBuffer.h> // Move to respective places(not the include line, but the file itself)
#include <NNBSSNetwork/Kit/LockFreeSpsc.h>
// Boost
#include <boost/asio/ip/udp.hpp>

class CNNBSSStreamReceiver
{
public:
	CNNBSSStreamReceiver(SNNBSSMVMData* params);
	~CNNBSSStreamReceiver();

	bool Setup();
	cv::Mat GetCurrentFrame();
	void Terminate();

private:
	SNNBSSMVMData* p_MVM = nullptr;

	boost::asio::io_context ioContext;
	boost::asio::ip::udp::resolver m_Resolver;
	boost::asio::ip::udp::socket m_Recv_socket;

	LockFreeSpsc<InputBuffer>* m_disruptor = nullptr;
};

#endif // ! STREAMRECEIVER_H