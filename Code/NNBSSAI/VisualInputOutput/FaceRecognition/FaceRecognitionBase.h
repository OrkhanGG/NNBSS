#ifndef FACERECOGNITIONBASE_H
#define FACERECOGNITIONBASE_H

//---------------------------
// Standard C Lib
#include <iostream>
#include <vector>
#include <map>
//---------------------------
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/face.hpp>
//---------------------------
inline const int NNBSS_RECOGNITION_SCALE = 64;
inline const std::string NNBSS_RECOGNITION_IMAGE_EXTENSION = ".jpg";
//---------------------------
struct SNNBSSPersonData
{
	std::string Name;
	std::string Surename;
	std::string FatherName;
	std::string Age;

	std::string Gender;
	std::string Length;

    std::string BirthDay;
	std::string BirthMonth;
	std::string BirthYear;

	std::string GetContainingFolderName()
	{
		return Name + Surename + FatherName + Age + Gender;
	}
};
//---------------------------
class CNNBSSAIFaceRecognition
{
public:
	//------------------------
	CNNBSSAIFaceRecognition() {};
	~CNNBSSAIFaceRecognition() {};
	//------------------------
	bool Init();
	//------------------------
	bool PerformFaceRecognition(cv::Mat& ROI, int& predictedLabel, double& confidence, SNNBSSPersonData& personData);
	//------------------------
	bool RequestWriteCache(cv::Mat& ROI, SNNBSSPersonData& PersonData);
private:
	//------------------------
	cv::Mat NormalizeMat_0_255(cv::InputArray _src);
	bool ReadInformation(std::vector<cv::Mat>& images, std::vector<int>& labels, std::map<int, SNNBSSPersonData>& personData);
	//------------------------
	std::vector<cv::Mat> image_data;
	std::vector<int> label_data;
	std::map<int, SNNBSSPersonData> m_PersonData;
	//------------------------
	cv::Ptr<cv::face::EigenFaceRecognizer> _eigenFaceRecognizerModel;

};

#endif // ! FACERECOGNITIONBASE_H