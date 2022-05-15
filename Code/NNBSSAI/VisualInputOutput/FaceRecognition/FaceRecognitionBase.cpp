#include "FaceRecognitionBase.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// Standard C Lib
#include <fstream>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
// OpenCV
#include <opencv2/opencv.hpp>

bool CNNBSSAIFaceRecognition::Init()
{
	image_data.clear();
	label_data.clear();
	m_PersonData.clear();

	try
	{
		if (!ReadInformation(image_data, label_data, m_PersonData))
			return false;
	}
	catch (const cv::Exception& e)
	{
		// Error
		return false;
	}
	catch (...)
	{
		// Error
		return false;
	}

	// Quit if there are not enough images for this demo.
	if (image_data.size() <= 1) {
		// Error
		return false;
	}

	try
	{
		_eigenFaceRecognizerModel = cv::face::EigenFaceRecognizer::create();

		_eigenFaceRecognizerModel->train(image_data, label_data);
	}
	catch (const cv::Exception& e) {
		// Error
		return false;
	}
	catch (...)
	{
		// Error
		return false;
	}

	image_data.pop_back();
	label_data.pop_back();

	return true;
}

bool CNNBSSAIFaceRecognition::PerformFaceRecognition(cv::Mat& ROI, int& predictedLabel, double& confidence, SNNBSSPersonData& personData)
{
	// Face recognition
	cvtColor(ROI, ROI, cv::COLOR_BGR2GRAY);
	cv::resize(ROI, ROI, cv::Size(NNBSS_RECOGNITION_SCALE, NNBSS_RECOGNITION_SCALE));// Don't forget that, ROI size should match the dataset image size
	try
	{
		//predictedLabel = _eigenFaceRecognizerModel->predict(ROI);

		confidence = 1000.0;// 1000 does not mean anything at all. it's just a random value
		_eigenFaceRecognizerModel->predict(ROI, predictedLabel, confidence);
		personData = m_PersonData[predictedLabel];
	}
	catch (const cv::Exception& e)
	{
		return false;
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool CNNBSSAIFaceRecognition::RequestWriteCache(cv::Mat& ROI, SNNBSSPersonData& PersonData)
{
	cv::resize(ROI, ROI, cv::Size(NNBSS_RECOGNITION_SCALE, NNBSS_RECOGNITION_SCALE));
	
	TiXmlDocument document(NNBSS_DIR__FACIALDATABASE_XML_PATH);
	if (!document.LoadFile())
	{
		document.SaveFile();
	}

	// add shortcuts
	std::string m_PName(PersonData.Name)
		, m_PSurename(PersonData.Surename)
		, m_PFatherName(PersonData.FatherName)
		, m_PAge(PersonData.Age)
		, m_PGender(PersonData.Gender)
		, m_PBirthday(PersonData.BirthDay)
		, m_PBirthMonth(PersonData.BirthMonth)
		, m_PBirthYear(PersonData.BirthYear)
		, m_PLength(PersonData.Length)
		, m_PContainingFolderName(PersonData.GetContainingFolderName());

	auto CreateNewAttribute = [&]()
	{
		TiXmlElement* p_DatasetElement = new TiXmlElement("FacialDataset");
		p_DatasetElement->SetAttribute("Name", m_PName.c_str());
		p_DatasetElement->SetAttribute("Surename", m_PSurename.c_str());
		p_DatasetElement->SetAttribute("FatherName", m_PFatherName.c_str());
		p_DatasetElement->SetAttribute("Age", m_PAge.c_str());
		p_DatasetElement->SetAttribute("Gender", m_PGender.c_str());
		p_DatasetElement->SetAttribute("Birthday", m_PBirthday.c_str());
		p_DatasetElement->SetAttribute("BirthMonth", m_PBirthMonth.c_str());
		p_DatasetElement->SetAttribute("BirthYear", m_PBirthYear.c_str());
		p_DatasetElement->SetAttribute("Length", m_PLength.c_str());

		return p_DatasetElement;
	};

	auto GetFolder = [&]()
	{
		struct stat info;
		
		if (stat(std::string(NNBSS_DIR__FACIALDATABASE_DIR.ToStdString() + m_PContainingFolderName).c_str(), &info) != 0)
			return std::string();
		else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
			return std::string(NNBSS_DIR__FACIALDATABASE_DIR.ToStdString() + m_PContainingFolderName);
		else
			return std::string();
	};

	const auto SaveROI = [&](const unsigned int ImageRowNumber)
	{
		std::string p_ImageName = NNBSS_DIR__FACIALDATABASE_DIR.ToStdString() + m_PContainingFolderName+"\\"+std::to_string(ImageRowNumber) + NNBSS_RECOGNITION_IMAGE_EXTENSION;
		cv::imwrite(p_ImageName, ROI);
	};

	if (GetFolder().empty())
	{
		// create the folder if it does not exist
		std::filesystem::create_directories(NNBSS_DIR__FACIALDATABASE_DIR.ToStdString() + m_PContainingFolderName);
	}

	// do we have root?
	TiXmlElement* d_Root = document.RootElement();
	if (d_Root == NULL)
	{
		TiXmlComment* dontmodifyComment = new TiXmlComment("Do not manually modify this file!");
		document.LinkEndChild(dontmodifyComment);
		// create if does not exist
		d_Root = new TiXmlElement("Root");
		document.LinkEndChild(d_Root);
	}

	// do we have the xml element that we're looking for?
	bool Found = false;
	for (TiXmlElement* e = d_Root->FirstChildElement("FacialDataset"); e != NULL; e = e->NextSiblingElement("FacialDataset"))
	{
		if (e->Attribute("Name") == m_PName
			&& e->Attribute("Surename") == m_PSurename
			&& e->Attribute("FatherName") == m_PFatherName
			&& e->Attribute("Age") == m_PAge
			&& e->Attribute("Gender") == m_PGender
			&& e->Attribute("Birthday") == m_PBirthday
			&& e->Attribute("BirthMonth") == m_PBirthMonth
			&& e->Attribute("BirthYear") == m_PBirthYear
			&& e->Attribute("Length") == m_PLength)
		{
			int ChildCount = 0;
			for (TiXmlElement* fdsElement = e->FirstChildElement("File");
				fdsElement != NULL; fdsElement = fdsElement->NextSiblingElement("File"))
			{
				ChildCount++;
			}

			TiXmlElement* p_NewElement = new TiXmlElement("File");
			p_NewElement->SetAttribute("Name",std::string(std::to_string(ChildCount)+ NNBSS_RECOGNITION_IMAGE_EXTENSION).c_str());
			e->LinkEndChild(p_NewElement);

			SaveROI(ChildCount);

			Found = true;
			break;
		}
	}

	if (!Found)
	{
		TiXmlElement* p_Element = CreateNewAttribute();
		d_Root->LinkEndChild(p_Element);

		TiXmlElement* p_NewElement = new TiXmlElement("File");
		p_NewElement->SetAttribute("Name", std::string(std::to_string(0) + NNBSS_RECOGNITION_IMAGE_EXTENSION).c_str());
		p_Element->LinkEndChild(p_NewElement);

		SaveROI(0);
	}

	document.SaveFile();

	return false;
}

cv::Mat CNNBSSAIFaceRecognition::NormalizeMat_0_255(cv::InputArray _src) {
	cv::Mat src = _src.getMat();
	// Create and return normalized image:
	cv::Mat dst;
	switch (src.channels()) {
	case 1:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

bool CNNBSSAIFaceRecognition::ReadInformation(std::vector<cv::Mat>& images, std::vector<int>& labels, std::map<int, SNNBSSPersonData>& personData)
{

	TiXmlDocument document(NNBSS_DIR__FACIALDATABASE_XML_PATH);
	if (!document.LoadFile())
	{
		return false;
	}

	// do we have root?
	TiXmlElement* d_Root = document.RootElement();
	if (d_Root == NULL)
	{
		return false;
	}

	bool Found = false;
	int LabelCounter = 0;
	for (TiXmlElement* e = d_Root->FirstChildElement("FacialDataset")
		; e != NULL; e = e->NextSiblingElement("FacialDataset"))
	{
		SNNBSSPersonData pData;
		{
			// Person Data
			pData.Name = e->Attribute("Name");
			pData.Surename = e->Attribute("Surename");
			pData.FatherName = e->Attribute("FatherName");
			pData.Age = e->Attribute("Age");
			pData.Gender = e->Attribute("Gender");
			pData.BirthDay = e->Attribute("Birthday");
			pData.BirthMonth = e->Attribute("BirthMonth");
			pData.BirthYear = e->Attribute("BirthYear");
			pData.Length = e->Attribute("Length");

		}

		for (TiXmlElement* fdsElement = e->FirstChildElement("File");
			fdsElement != NULL; fdsElement = fdsElement->NextSiblingElement("File"))
		{
			cv::Mat To = cv::imread(std::string(NNBSS_DIR__FACIALDATABASE_DIR.ToStdString()+ pData.GetContainingFolderName()+"\\" + fdsElement->Attribute("Name")).c_str(), 0);
			images.push_back(To);
			labels.push_back(LabelCounter);
		}
		
		{
			personData[LabelCounter] = pData;
		}

		LabelCounter++;

		Found = true;
	}

	if (!Found)
	{
		return false;
	}

	return true;
}