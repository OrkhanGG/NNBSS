#include "ObjectDetectionBase.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// wxWidgets
#include <wx/log.h>

bool CNNBSSAIObjectDetection::LoadCascadeClassifier(cv::CascadeClassifier& classifier)
{
	if (classifier.empty())
		return false;

	m_CascadeClassifier = classifier;

	return true;
}

bool CNNBSSAIObjectDetection::LoadCascadeClassifier(const cv::String& classifierPath)
{
	m_CascadeClassifier.load(classifierPath);

	if (m_CascadeClassifier.empty())
		return false;
	else
		return true;
}

void CNNBSSAIObjectDetection::SetParameters(double arrayScaleFactor, double scaleFactor,
	int minNeighbors, cv::Size minSize, cv::Size maxSize)
{
	m_arrayScaleFactor = arrayScaleFactor;
	m_scaleFactor = scaleFactor;
	m_minNeighbors = minNeighbors;
	m_minSize = minSize;
	m_maxSize = maxSize;
}

std::vector<cv::Rect> CNNBSSAIObjectDetection::Detect(cv::Mat& inputArray, std::string classifierPath)
{
	std::vector<cv::Rect> detection;

	double fx = 1 / m_arrayScaleFactor;

	try
	{
		if (m_CascadeClassifier.empty())
		{
			m_CascadeClassifier.load(classifierPath);
		}

		cv::Mat gray, smallImg;

		cv::cvtColor(inputArray, gray, cv::COLOR_BGR2GRAY); // Convert to Gray Scale

		// Resize the Grayscale Image
		cv::resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR);
		cv::equalizeHist(smallImg, smallImg);

		// Detect by cascade classifier
		m_CascadeClassifier.detectMultiScale(smallImg, detection, m_scaleFactor,
			m_minNeighbors, 0/*, m_minSize, m_maxSize*/);
	}
	catch (std::runtime_error& e)
	{
		throw std::runtime_error("RuntimeError: ::CNNBSSAIObjectDetection Detect");
	}
	catch (...)
	{
		throw std::runtime_error("Unknown Exception ::CNNBSSAIObjectDetection");
	}


	for (auto& p : detection)
	{
		p.x *= (fx* m_arrayScaleFactor)* m_arrayScaleFactor;
		p.y *= (fx * m_arrayScaleFactor)* m_arrayScaleFactor;
		p.width *= (fx * m_arrayScaleFactor)* m_arrayScaleFactor;
		p.height *= (fx * m_arrayScaleFactor)* m_arrayScaleFactor;
	}

	return detection;
}

bool CNNBSSAIObjectDetection::InRange(const int low, const int high, const int x)
{
	return (low <= x && x <= high);
}

void CNNBSSAIObjectDetection::StabilizeRectangles(std::vector<cv::Rect>& rectangle)
{
	//const int stabilizeRate = 2;

	//if (rectangleCache.empty()) 
	//{
	//	rectangleCache = rectangle;
	//}

	//{
	//	for (const auto& p : rectangleCache)
	//	{
	//		for (auto& t : rectangle)
	//		{
	//			if (InRange(p.x - stabilizeRate, p.x + stabilizeRate, p.x) && InRange(p.y - stabilizeRate, p.y + stabilizeRate, p.y)
	//				&& InRange(p.width - stabilizeRate, p.width + stabilizeRate, p.width) && InRange(p.height - stabilizeRate, p.height + stabilizeRate, p.height))
	//			{
	//				t = p;
	//			}
	//		}
	//	}
	//}
	//rectangleCache.clear();
	//rectangleCache = rectangle;
}