///////////////////////////////////////////////////////////////////////////////
// Name:        DisplayResolution.cpp
// Purpose:     Mostly contains Getter functions about DisplayResolution 
//              to make related work easier, not to repeat unnecessarily and sometimes to avoid hardcode.
// Author:      Orkhan Aliyev
// Created:     2022-04-09
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef DISPLAYRESOLUTION_H
#define DISPLAYRESOLUTION_H

// Standard C Lib
#include <vector>

class SNNBSSDisplayResolution
{
public:
	typedef std::string ResolutionAbbreviation;
	typedef int ResolutionWidth;
	typedef int ResolutionHeight;

	std::vector<std::pair<ResolutionAbbreviation, std::tuple<ResolutionWidth, ResolutionHeight>>>
		GetResolutions() const { return Resolutions; }

	// Returns width and height according to abbreviation: 1280x720 ->720p
	void GetResolution(const ResolutionAbbreviation& abbreviation, ResolutionWidth& resWidth, ResolutionHeight& resHeight);

	ResolutionAbbreviation GetResolutionAbbreviation(const ResolutionWidth& resWidth, const ResolutionHeight& resHeight);
private:

	std::vector<std::pair<ResolutionAbbreviation, std::tuple<ResolutionWidth, ResolutionHeight>>>
		Resolutions =
	{
		{"320x240",{320,240}},
		{"480x360",{480,360}},
		{"640x360",{640,360}},
		{"640x480",{640,480}},
		{"1280x720",{1280,720}},
		{"1920x1080",{1920,1080}},
		{"2048x1152",{2048,1152}},
		{"2432x1366",{2048,1152}},
		{"3840x2160",{3840, 2160}}
	};
};
inline SNNBSSDisplayResolution CNNBSSDisplayResolutionAddressHandle()
{
	static SNNBSSDisplayResolution s_VRObj;
	return s_VRObj;
}
//------------------------

#endif // ! DISPLAYRESOLUTION_H