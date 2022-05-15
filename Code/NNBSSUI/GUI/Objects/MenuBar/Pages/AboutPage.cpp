// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "AboutPage.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

#include <wx/button.h>
#include <wx/sizer.h>
#include "wx/image.h"
#include "wx/splash.h"
#include "wx/dcmemory.h"

const char* AdditionalInformation[] =
	{ "By using this application, You agree to check www.loam.com periodically",
	"for new information and terms that govern your use of NNBSS. Loam may modify",
	"this Agreement at any time. Loam will inform you about revisions",
	"to this Agreement by email and/or by a notice on our home page",
	"and/or during log in. Revisions to terms affecting existing NNBSS shall be effective",
	"thirty (30) days after posting at www.loam.com. If you do not agree with the new terms",
	"your only remedy is to stop using NNBSS."};

void CNNBSSAboutPage::_RequestCreate()
{
	wxImage::AddHandler(new wxPNGHandler);

	wxBitmap bitmap;

	const char* ssFileName = "SplashScreen.png";
	bitmap.LoadFile(wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Engine", ssFileName), wxBITMAP_TYPE_PNG);

	int splashScreenDisplayDuration = -1;

	if (bitmap.IsOk())
	{
		// we can even draw dynamic artwork onto our splashscreen
		DecorateSplashScreen(bitmap);

		// show the splashscreen
		wxSplashScreen* _SS  = new wxSplashScreen(bitmap,
			wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_NO_TIMEOUT,
			splashScreenDisplayDuration, GetActiveUXFrame(), wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxSIMPLE_BORDER | wxSTAY_ON_TOP);
	}
}

void CNNBSSAboutPage::DecorateSplashScreen(wxBitmap& bmp)
{
	// USE A MEMORY DC TO DRAW DIRECTLY ONTO THE BITMAP
	wxMemoryDC memDc(bmp);

	const wxRect bannerRect(wxPoint(0, 0),
		wxPoint(bmp.GetWidth(), bmp.GetHeight()));
	wxDCBrushChanger bc(memDc, wxBrush(RGB_COLOURDEF_TRIPLE20));

	memDc.DrawRectangle(bannerRect);

	memDc.SetBrush(wxBrush(RGB_COLOURDEF_NNBSSBLUE));

	memDc.DrawBitmap(NNBSS_PNG2BMP(NNBSSART_LOGO_MEDIUM_PNG), wxPoint((bannerRect.width / 2)-64, 30), true);
	memDc.DrawBitmap(NNBSS_PNG2BMP(NNBSSART_LOAM_LOGO_512P), wxPoint((bannerRect.width / 2) - 128, 300), true);

	wxString description = wxString::Format("%s version: %s", ENGINE_NAME, ENGINE_VERSION);
	wxString year(__DATE__);
	wxString copyrightLabel = wxString::Format("%s%s %s. %s",
		wxString::FromUTF8("\xc2\xa9"), year.Mid(year.Length() - 4), CORP_NAME,
		"All rights reserved.");


	std::string InfoText;
	for (int c = 0; c < (sizeof AdditionalInformation/ sizeof AdditionalInformation[0]); c++)
	{
		InfoText += std::string(c < 1 ? "" : "\n")+AdditionalInformation[c];
	}

	memDc.SetFont(wxFontInfo(10));
	memDc.SetTextForeground(RGB_COLOURDEF_NNBSSBLUE);

	const wxRect additionalInfoRect(wxPoint(0, 160),
		wxPoint(bmp.GetWidth(), bmp.GetHeight()));
	memDc.DrawLabel(wxString(InfoText.c_str(), wxConvUTF8), additionalInfoRect.Deflate(10, 10), wxALIGN_CENTER_HORIZONTAL);

	memDc.SetFont(wxFontInfo(10));
	memDc.DrawLabel(description, bannerRect.Deflate(10, 10), wxALIGN_BOTTOM | wxALIGN_RIGHT);
	memDc.DrawLabel(copyrightLabel, bannerRect.Deflate(10, 10), wxALIGN_BOTTOM | wxALIGN_LEFT);


	memDc.SetBrush(*wxTRANSPARENT_BRUSH);
	memDc.SetPen(wxPen(RGB_COLOURDEF_NNBSSBLUE, 5, wxPENSTYLE_SOLID));

	// inline
	memDc.DrawRectangle(bannerRect.Deflate(1,1));
}