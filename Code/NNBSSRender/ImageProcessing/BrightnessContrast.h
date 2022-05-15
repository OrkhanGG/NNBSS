///////////////////////////////////////////////////////////////////////////////
// Name:        BrightnessContrast.cpp
// Purpose:     All brightness and contrast related stuff should be handled here.
//              Anything in the code related and missing here should be moved ASAP.
// Author:      Orkhan Aliyev
// Created:     2022-04-09
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef BRIGHTNESSCONTRAST_H
#define BRIGHTNESSCONTRAST_H

namespace cv
{
	class Mat;
}

class CNNBSSImgProcBrightnessContrast
{
public:
	CNNBSSImgProcBrightnessContrast() {};
	~CNNBSSImgProcBrightnessContrast() {};

	int GetDefaultBrightness() const { return m_defaultBrightness; }
	int GetDefaultContrast() const { return m_defaultContrast; }

	void Brightness(cv::Mat& inputOutput, const int& brightness);
	void Contrast(cv::Mat& inputOutput, const int& contrast);
	void BrightnessContrast(cv::Mat& inputOutput, const int& brightness, const int& contrast);

private:
	static constexpr int m_defaultBrightness = 1;
	static constexpr int m_defaultContrast = 1;
};

#endif // ! BRIGHTNESSCONTRAST_H