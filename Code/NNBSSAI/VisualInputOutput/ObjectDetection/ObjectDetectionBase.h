#ifndef OBJECTDETECTIONBASE_H
#define OBJECTDETECTIONBASE_H

// Standard C Lib
#include <vector>
#include <map>
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

class CNNBSSAIObjectDetection
{
public:
	// Load CascadeClassifier and Set Parameters(optional) then Detect
	CNNBSSAIObjectDetection() {};
	~CNNBSSAIObjectDetection() {};

	bool LoadCascadeClassifier(cv::CascadeClassifier& classifier);
	bool LoadCascadeClassifier(const cv::String& classifierPath);

	void SetParameters(double arrayScaleFactor = 1.0, double scaleFactor = 1.1,
		int minNeighbors = 3,
		cv::Size minSize = cv::Size(),
		cv::Size maxSize = cv::Size());

	std::vector<cv::Rect> Detect(cv::Mat& inputArray, std::string classifierPath);

	void StabilizeRectangles(std::vector<cv::Rect>& rectangle);

private:
	bool InRange(const int low, const int high, const int x);
	
	std::vector<cv::Rect> rectangleCache;

	cv::CascadeClassifier m_CascadeClassifier;

	std::string CascadeClassifierPath = std::string("");
	double m_arrayScaleFactor = 1.0;
	double m_scaleFactor = 1.1;
	int m_minNeighbors = 3;
	cv::Size m_minSize = cv::Size();
	cv::Size m_maxSize = cv::Size();

};

#endif // ! OBJECTDETECTIONBASE_H