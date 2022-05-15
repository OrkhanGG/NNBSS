///////////////////////////////////////////////////////////////////////////////
// Name:        MVMWriterThread.h
// Purpose:     Saving/writing Audio Visual(AV) data
//              Encryption has not been implemented yet
// Author:      Orkhan aliyev
// Created:     2022-04-12
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#ifndef MVMWRITERTHREAD_H
#define MVMWRITERTHREAD_H

namespace cv
{
	class Mat;
	class VideoWriter;
}

class AVFormatContext;
class AVCodec;
class AVStream;
class AVCodecContext;
class SwsContext;
class AVFrame;
class AVPacket;

// Standard C Lib
#include <vector>

class CNNBSSMovingVisualMediaWriter;

class CNNBSSMovingVisualMediaWriterThread : public wxThread
{
public:
	CNNBSSMovingVisualMediaWriterThread(CNNBSSMovingVisualMediaWriter* p_Writer);
	~CNNBSSMovingVisualMediaWriterThread();

private:
	const int m_sleepBreak = 10;// in seconds
	
	CNNBSSMovingVisualMediaWriter* p_VideoWriter{nullptr};

	ExitCode Entry() override;

	AVFormatContext* outctx = nullptr;
	AVCodec* vcodec = nullptr;
	AVStream* vstrm = nullptr;
	AVCodecContext* cctx = nullptr;
	SwsContext* swsctx = nullptr;
	AVFrame* frame = nullptr;
	AVPacket* pkt = nullptr;

	int ret;// brief this
	bool end_of_stream{false};// brief this
		// encoding loop
	int64_t frame_pts = 0;
	unsigned nb_frames = 0;

	std::vector<cv::Mat> p_Frames2Write;
};

class CNNBSSMovingVisualMediaWriter
{
public:
	CNNBSSMovingVisualMediaWriter();
	~CNNBSSMovingVisualMediaWriter();

	void Record(const std::string& path,
		const int& frameWidth, const int& frameHeight,
		const int& FPS,
		const std::string encoder);

	void StopRecord();

	// Cache data until it's saved
	void AddToCacheQueue(cv::Mat frame);

	bool IsRecording() const;

	// Frames that wil be pending thread's write queue 
	std::vector<cv::Mat> p_CachedFrames;

	std::string GetWorkPath() const { return m_WorkPath; }
	int GetFrameWidth() const { return m_FrameWidth; }
	int GetFrameHeight() const { return m_FrameHeight; }
	int GetFPS() const { return m_FPS; }
	std::string GetEncoder() const { return m_Encoder; }
private:
	// Thread to execute processes without interrupting UX
	// Mostly will write/read video frames with some additional information
	CNNBSSMovingVisualMediaWriterThread* p_Thread{ nullptr };

	bool m_writerState{false};

	// Thread will have to access this data for once on start
	std::string m_WorkPath;
	int m_FrameWidth;
	int m_FrameHeight;
	int m_FPS;
	std::string m_Encoder;// Encoder
};

#endif // ! MVMWRITERTHREAD_H