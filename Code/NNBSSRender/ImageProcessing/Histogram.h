///////////////////////////////////////////////////////////////////////////////
// Name:        RenderSectorManager.h
// Purpose:     All Histogram related stuff will be handled here AMAP
// Author:      Orkhan Aliyev
// Created:     2022-03-31
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

namespace cv
{
	class Mat;
}

class CNNBSSImgProcHistogram
{
public:
	CNNBSSImgProcHistogram() {};
	~CNNBSSImgProcHistogram() {};

	void HistogramFlags(const long& flags, bool set)
	{
		if (set) {
			if (!(m_HistogramFlags & flags))
			{
				m_HistogramFlags |= flags;
			}
		}
		else
		{
			if (m_HistogramFlags & flags)
				m_HistogramFlags &= ~flags;
		}
	}
	void DisplayHistogramOn(cv::Mat& src, cv::Mat& dst);
private:
	bool IsOpaque()
	{
		if (m_HistogramFlags & HISTOGRAM_OPAQUE)
			return true;

		return false;
	}

	enum HistogramOptions
	{
		HISTOGRAM_OPAQUE = 1,// Histogram will be displayed on a 2D plane with flat a colour
		HISTOGRAM_INDICATOR_LABELS
	};
	long m_HistogramFlags{ 0 };
};

#endif // ! HISTOGRAM_H