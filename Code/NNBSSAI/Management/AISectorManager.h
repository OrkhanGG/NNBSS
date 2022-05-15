#ifndef AISECTORMANAGER_H
#define AISECTORMANAGER_H

// Object DetectionBase
#include <NNBSSAI/VisualInputOutput/ObjectDetection/ObjectDetectionBase.h>
// Face Recognition Base
#include <NNBSSAI/VisualInputOutput/FaceRecognition/FaceRecognitionBase.h>
// Notifications
#include <NNBSSUI/GUI/Objects/Notifications/NotificationsManager.h>

// Checks whether given CascadeClassifier is valid or not
inline bool CheckCascadeClassifier(std::string classifierPath)
{
	if (classifierPath.empty())
	{
		NNBSSErrorShow("Cascade classifier path was empty!", 100);
		return false;
	}

	cv::Mat blank(64, 64, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::cvtColor(blank, blank, cv::COLOR_BGR2GRAY); // Convert to Gray Scale
	// Resize the Grayscale Image
	cv::equalizeHist(blank, blank);

	try
	{
		cv::CascadeClassifier p_TestCC;
		p_TestCC.load(classifierPath);
		
		p_TestCC.detectMultiScale(blank, std::vector<cv::Rect>());
	}
	catch (const std::exception& e)
	{
		NNBSSErrorShow("%s", 100, e.what());
		return false;
	}
	catch (...)
	{
		NNBSSErrorShow("Cascade classifier could not be opened!", 100);
		return false;
	}

	return true;
}

#endif // ! AISECTORMANAGER_H