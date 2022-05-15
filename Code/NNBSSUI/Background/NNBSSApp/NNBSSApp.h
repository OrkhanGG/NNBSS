///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#define START_NNBSS_WITH_WXWIDGETS 1 // Only used for debug purposes

#if START_NNBSS_WITH_WXWIDGETS

#ifndef NNBSSAPP_H
#define NNBSSAPP_H

class wxSingleInstanceChecker;

// -- application --
class CCNNBSSApp : public wxApp
{
public:
	bool OnInit() wxOVERRIDE;

private:
	wxSingleInstanceChecker* si_Checker = nullptr;

	//void DecorateSplashScreen(wxBitmap& bmp);
};
wxDECLARE_APP(CCNNBSSApp);
wxIMPLEMENT_APP(CCNNBSSApp);

#endif // ! NNBSSAPP_H

#endif