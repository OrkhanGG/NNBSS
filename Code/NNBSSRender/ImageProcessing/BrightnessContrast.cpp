///////////////////////////////////////////////////////////////////////////////
// Name:        BrightnessContrast.cpp
// Purpose:     All brightness and contrast related stuff should be handled here.
//              Anything in the code related and missing here should be moved ASAP.
// Author:      Orkhan Aliyev
// Created:     2022-04-09
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#include "BrightnessContrast.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

void CNNBSSImgProcBrightnessContrast::Brightness(cv::Mat& inputOutput, const int& brightness)
{
	for (int y = 0; y < inputOutput.rows; y++) {
		for (int x = 0; x < inputOutput.cols; x++) {
			for (int c = 0; c < inputOutput.channels(); c++) {
				inputOutput.at<cv::Vec3b>(y, x)[c] =
					cv::saturate_cast<uchar>(
						/*contrast * */inputOutput.at<cv::Vec3b>(y, x)[c] + brightness);
			}
		}
	}
}

void CNNBSSImgProcBrightnessContrast::Contrast(cv::Mat& inputOutput, const int& contrast)
{
	for (int y = 0; y < inputOutput.rows; y++) {
		for (int x = 0; x < inputOutput.cols; x++) {
			for (int c = 0; c < inputOutput.channels(); c++) {
				inputOutput.at<cv::Vec3b>(y, x)[c] =
					cv::saturate_cast<uchar>(
						contrast * inputOutput.at<cv::Vec3b>(y, x)[c] /*+ brightness*/);
			}
		}
	}
}

void CNNBSSImgProcBrightnessContrast::BrightnessContrast(cv::Mat& inputOutput, const int& brightness, const int& contrast)
{
	for (int y = 0; y < inputOutput.rows; y++) {
		for (int x = 0; x < inputOutput.cols; x++) {
			for (int c = 0; c < inputOutput.channels(); c++) {
				inputOutput.at<cv::Vec3b>(y, x)[c] =
					cv::saturate_cast<uchar>(
						contrast * inputOutput.at<cv::Vec3b>(y, x)[c] + brightness);
			}
		}
	}
}
