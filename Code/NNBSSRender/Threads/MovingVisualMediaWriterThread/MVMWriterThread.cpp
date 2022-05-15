///////////////////////////////////////////////////////////////////////////////
// Name:        MVMWriterThread.cpp
// Purpose:     Saving/writing Audio Visual(AV) data
//              Encryption has not been implemented yet
// Author:      Orkhan aliyev
// Created:     2022-04-12
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif
#include <wx/thread.h>// move to source

#include "MVMWriterThread.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
 // FFmpeg
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

/*************************************************************************************************************
************************* MVM WRITER**************************************************************************
**************************************************************************************************************/

CNNBSSMovingVisualMediaWriter::CNNBSSMovingVisualMediaWriter()
{
}
CNNBSSMovingVisualMediaWriter::~CNNBSSMovingVisualMediaWriter()
{
	if (p_Thread)
	{
		p_Thread->Delete();
		delete p_Thread;
		p_Thread = nullptr;// To safely delete the thread, destroy it first
	}
	m_writerState = false;
}

void CNNBSSMovingVisualMediaWriter::Record(const std::string& path, const int& frameWidth,
	const int& frameHeight, const int& FPS, const std::string encoder)
{

	m_WorkPath = path;
	m_FrameWidth = frameWidth;
	m_FrameHeight = frameHeight;
	m_Encoder = encoder;
	m_FPS = FPS;

	p_Thread = new CNNBSSMovingVisualMediaWriterThread(this);/*Create a new thread for this process*/

	if (p_Thread->Run() != wxTHREAD_NO_ERROR)
	{
		NNBSSMessageBoxSimple(_("Error"), _("Somethings went wrong while trying to save the video!"));
	}
	else
	{
		m_writerState = true;
	}
}
void CNNBSSMovingVisualMediaWriter::StopRecord()
{
	if (p_Thread)
	{
		p_Thread->Delete();
		delete p_Thread;
		p_Thread = nullptr;// To safely delete the thread, destroy it first
	}
	m_writerState = false;
}

bool CNNBSSMovingVisualMediaWriter::IsRecording() const
{
	return m_writerState;
}

void CNNBSSMovingVisualMediaWriter::AddToCacheQueue(cv::Mat frame)
{
	p_CachedFrames.emplace_back(frame);
}

/*************************************************************************************************************
************************* MVM WRITER THREAD*******************************************************************
**************************************************************************************************************/
CNNBSSMovingVisualMediaWriterThread::CNNBSSMovingVisualMediaWriterThread
(CNNBSSMovingVisualMediaWriter* p_Writer)
	: wxThread(wxTHREAD_JOINABLE), p_VideoWriter(p_Writer)
{
	// av_log_set_level(AV_LOG_DEBUG);

	const int dst_width = p_VideoWriter->GetFrameWidth();
	const int dst_height = p_VideoWriter->GetFrameHeight();
	const AVRational dst_fps = { p_VideoWriter->GetFPS(), 1 };

	// some device ignore above parameters for capturing image,
	// so we query actual parameters for image rescaler.
	const int cv_width = p_VideoWriter->GetFrameWidth();
	const int cv_height = p_VideoWriter->GetFrameHeight();
	const int cv_fps = p_VideoWriter->GetFPS();

	// open output format context
	outctx = nullptr;

	ret = avformat_alloc_output_context2(&outctx, nullptr, nullptr, p_VideoWriter->GetWorkPath().c_str());
	if (ret < 0) {
		std::cerr << "fail to avformat_alloc_output_context2(" << p_VideoWriter->GetWorkPath().c_str() << "): ret=" << ret;
	}

	// create new video stream
	vcodec = const_cast<AVCodec*>(avcodec_find_encoder_by_name(p_VideoWriter->GetEncoder().c_str()));
	if (vcodec == NULL)
	{
		// Find encoder automatically
		vcodec = const_cast<AVCodec*>(avcodec_find_encoder(outctx->oformat->video_codec));

		// Inform user about the problem
		NNBSSMessageBoxSimple(_("Error"), _("Encoder could not be set!"));
	}
	vstrm = avformat_new_stream(outctx, vcodec);
	if (!vstrm) {
		std::cerr << "fail to avformat_new_stream";
	}

	// open video encoder
	cctx = avcodec_alloc_context3(vcodec);
	if (!vstrm)
	{
		std::cerr << "fail to avcodec_alloc_context3";
	}
	cctx->width = dst_width;
	cctx->height = dst_height;
	cctx->pix_fmt = vcodec->pix_fmts[0];
	cctx->time_base = av_inv_q(dst_fps);
	cctx->framerate = dst_fps;
	if (outctx->oformat->flags & AVFMT_GLOBALHEADER)
		cctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	ret = avcodec_open2(cctx, vcodec, nullptr);
	if (ret < 0) {
		std::cerr << "fail to avcodec_open2: ret=" << ret;
	}
	avcodec_parameters_from_context(vstrm->codecpar, cctx);

	// initialize sample scaler
	swsctx = sws_getContext(
		cv_width, cv_height, AV_PIX_FMT_BGR24,
		dst_width, dst_height, cctx->pix_fmt,
		SWS_BILINEAR, nullptr, nullptr, nullptr);
	if (!swsctx) {
		std::cerr << "fail to sws_getContext";
	}

	// allocate frame buffer for encoding
	frame = av_frame_alloc();
	frame->width = dst_width;
	frame->height = dst_height;
	frame->format = static_cast<int>(cctx->pix_fmt);
	ret = av_frame_get_buffer(frame, 32);
	if (ret < 0) {
		std::cerr << "fail to av_frame_get_buffer: ret=" << ret;
	}

	// allocate packet to retrive encoded frame
	pkt = av_packet_alloc();

	// open output IO context
	ret = avio_open2(&outctx->pb, p_VideoWriter->GetWorkPath().c_str(), AVIO_FLAG_WRITE, nullptr, nullptr);
	if (ret < 0) {
		std::cerr << "fail to avio_open2: ret=" << ret;
	}

#ifdef _CONSOLE_
	std::cout
		<< "camera:  " << cv_width << 'x' << cv_height << '@' << cv_fps << "\n"
		<< "outfile: " << p_VideoWriter->GetWorkPath().c_str() << "\n"
		<< "format:  " << outctx->oformat->name << "\n"
		<< "vcodec:  " << vcodec->name << "\n"
		<< "size:    " << dst_width << 'x' << dst_height << "\n"
		<< "fps:     " << av_q2d(cctx->framerate) << "\n"
		<< "pixfmt:  " << av_get_pix_fmt_name(cctx->pix_fmt) << "\n"
		<< std::flush;
#endif 

	// write media container header (if any)
	ret = avformat_write_header(outctx, nullptr);
	if (ret < 0) {
		std::cerr << "fail to avformat_write_header: ret=" << ret;
	}
	
	//ret = av_opt_set(cctx,"512512512","512512512",AV_PKT_DATA_ENCRYPTION_INFO);
}

CNNBSSMovingVisualMediaWriterThread::~CNNBSSMovingVisualMediaWriterThread()
{
	avcodec_send_frame(cctx, nullptr);
	end_of_stream = true;
	//std::cout << nb_frames << " frames encoded" << std::endl;

	// write trailer and close file
	av_write_trailer(outctx);
	avio_close(outctx->pb);

	av_packet_free(&pkt);
	av_frame_free(&frame);
	sws_freeContext(swsctx);
	avcodec_free_context(&cctx);
	avformat_free_context(outctx);
};

wxThread::ExitCode CNNBSSMovingVisualMediaWriterThread::Entry()
{

	while (!TestDestroy())
	{
		if (p_VideoWriter->p_CachedFrames.empty())
			continue;

		p_Frames2Write = p_VideoWriter->p_CachedFrames;

		auto cur = p_Frames2Write.begin();

		if (!end_of_stream) {

			if (p_VideoWriter->GetFrameWidth() != (*cur).cols || p_VideoWriter->GetFrameHeight() != (*cur).rows)
				cv::resize((*cur), (*cur), cv::Size(p_VideoWriter->GetFrameWidth(), p_VideoWriter->GetFrameHeight()));

			// convert cv::Mat(OpenCV) to AVFrame(FFmpeg)
			const int stride[4] = { static_cast<int>((*cur).step[0]) };
			sws_scale(swsctx, &cur->data, stride, 0, (*cur).rows, frame->data, frame->linesize);
			frame->pts = frame_pts++;
			// encode video frame
			ret = avcodec_send_frame(cctx, frame);
			if (ret < 0) {
				std::cerr << "fail to avcodec_send_frame: ret=" << ret << "\n";
				break;
			}
		}
		while ((ret = avcodec_receive_packet(cctx, pkt)) >= 0) {
			// rescale packet timestamp
			pkt->duration = 1;
			av_packet_rescale_ts(pkt, cctx->time_base, vstrm->time_base);
			// write encoded packet
			av_write_frame(outctx, pkt);
			av_packet_unref(pkt);
			std::cout << nb_frames << '\r' << std::flush;  // dump progress
			++nb_frames;
		}
		if (ret == AVERROR_EOF)
			break;

		p_Frames2Write.erase(cur);
		p_VideoWriter->p_CachedFrames.erase(p_VideoWriter->p_CachedFrames.begin());
	}


	return static_cast<wxThread::ExitCode>(nullptr);
}