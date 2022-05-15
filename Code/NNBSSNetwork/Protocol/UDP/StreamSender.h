#ifndef STREAMSENDER_H
#define STREAMSENDER_H

// Forward Declarations
class SNNBSSMVMData;
namespace cv
{
	class Mat;
}

// Standard C Lib
#include <string>

class CNNBSSStreamSender : public wxThread
{
public:
	CNNBSSStreamSender(SNNBSSMVMData* params);
	~CNNBSSStreamSender() {};

	wxThreadError Start();
	wxThread::ExitCode Entry();

	// Cache data until it's sent
	void AddToCacheQueue(cv::Mat frame);

private:
	// MVM Params
	SNNBSSMVMData* m_Params = nullptr;

	// Don't forget to check these before pass.
	const std::string m_Address;
	const std::string m_AddressPort;
	const int m_Compression;

	// Frames that wil be pending thread's send queue 
	std::vector<cv::Mat> p_CachedFrames;
	std::vector<cv::Mat> p_Frames2Write;
};

#endif // ! STREAMSENDER_H