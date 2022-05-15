///////////////////////////////////////////////////////////////////////////////
// Name:        RenderSectorManager.h
// Purpose:     All Histogram related stuff will be handled here AMAP
// Author:      Orkhan Aliyev
// Created:     2022-03-31
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#include "Histogram.h"

#include <NNBSSBridge/MainBridge.h>

#include <opencv2/imgproc.hpp>

void CNNBSSImgProcHistogram::DisplayHistogramOn(cv::Mat& src, cv::Mat& dst)
{
	std::vector<cv::Mat> bgr_planes;
	cv::split(src, bgr_planes);
	const int histSize = 256;
	const float range[] = { 0, 256 }; //the upper boundary is exclusive
	const float* histRange[] = { range };
	bool uniform = true, accumulate = false;
	cv::Mat b_hist, g_hist, r_hist;
	cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
	cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
	cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);
	
	const int hist_w = src.cols, hist_h = src.rows;
	const int bin_w = cvRound((double)hist_w / histSize);
	
	cv::Mat histImage;
	if (IsOpaque())
		histImage = cv::Mat(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
	else
		histImage = src;

	cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
	for (int i = 1; i < histSize; i++)
	{
		// BGR TO RGB; First and Third elements' values are swapped
		cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
			cv::Scalar(0, 0, 255), 2, 8, 0);

		cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
			cv::Scalar(0, 255, 0), 2, 8, 0);

		cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
			cv::Scalar(255, 0, 0), 2, 8, 0);
	}
	dst = histImage;
}