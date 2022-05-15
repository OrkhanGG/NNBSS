///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "TitleBar.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS UI
#include <NNBSSUI/GUI/Objects/Controls/Button/Button.h>

CNNBSS_SC_TitleBar::~CNNBSS_SC_TitleBar()
{
}

void CNNBSS_SC_TitleBar::_SetupTitleBarInterface()
{
	SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	//_________________________BUTTONS_____________________________//
	CNNBSSControlButton* p_ServicesButton = new CNNBSSControlButton(this, NNBSS_EVT_ADD_SERVICES_PAGE, _("Services")
		, wxDefaultPosition, wxSize(64, 32));
	p_ServicesButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	CNNBSSControlButton* p_LayoutOptionsButton = new CNNBSSControlButton(this, NNBSS_EVT_SHOW_LAYOUT_OPTIONS, _("Layout")
		, wxDefaultPosition, wxSize(64, 32));
	p_LayoutOptionsButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	CNNBSSControlButton* p_PreferencesButton = new CNNBSSControlButton(this, NNBSS_EVT_SHOW_OPTIONS, _("Preferences")
		, wxDefaultPosition, wxSize(64, 32));
	p_PreferencesButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	CNNBSSControlButton* p_AboutButton = new CNNBSSControlButton(this, NNBSS_EVT_SHOW_ABOUT_US, _("About")
		, wxDefaultPosition, wxSize(64, 32));
	p_AboutButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	// Minimize button
	MinimizeButton = new CNNBSSControlButton(this, TB_ID_MINIMIZE, wxEmptyString, wxDefaultPosition, wxSize(64, 32), wxBORDER_NONE);
	MinimizeButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_HEADER_MINIMIZE_BUTTON));
	MinimizeButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	MinimizeButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);

	// Restore button
	RestoreButton = new CNNBSSControlButton(this, TB_ID_RESTORE, wxEmptyString, wxDefaultPosition, wxSize(64, 32), wxBORDER_NONE);
	RestoreButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_HEADER_RESTORE_BUTTON));
	RestoreButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	RestoreButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
	// Quit Button
	QuitButton = new CNNBSSControlButton(this, TB_ID_QUIT, wxEmptyString, wxDefaultPosition, wxSize(64, 32), wxBORDER_NONE);
	QuitButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_HEADER_X_BUTTON));
	QuitButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	QuitButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);

	//______________BUTTON SIZERS_____________________//
	wxBoxSizer* hsizer = new wxBoxSizer(wxHORIZONTAL);
	hsizer->AddSpacer(FromDIP(10));
	hsizer->Add(p_ServicesButton, wxSizerFlags().Proportion(0));
	hsizer->Add(p_LayoutOptionsButton, wxSizerFlags().Proportion(0));
	hsizer->Add(p_PreferencesButton, wxSizerFlags().Proportion(0));
	hsizer->Add(p_AboutButton, wxSizerFlags().Proportion(0));
	hsizer->AddStretchSpacer();
	hsizer->Add(MinimizeButton, wxSizerFlags().Proportion(0));
	hsizer->Add(RestoreButton, wxSizerFlags().Proportion(0));
	hsizer->Add(QuitButton, wxSizerFlags().Proportion(0));

	//_______________________________________________//

	////___________________________________________EVENT_BINDS_______________________________________//
	//		//__MENU_BAR__
	Bind(wxEVT_LEFT_DOWN, &CNNBSS_SC_TitleBar::TitleBarOnLeftDown, this);
	Bind(wxEVT_LEFT_DCLICK, &CNNBSS_SC_TitleBar::TitleBarOnLeftDClick, this);
	Bind(wxEVT_LEFT_UP, &CNNBSS_SC_TitleBar::TitleBarOnLeftUp, this);
	Bind(wxEVT_MOTION, &CNNBSS_SC_TitleBar::TitleBarOnMouseMove, this);
	Update();
	////________________________________QUIT/MAXIMIZE/MINIMIZE_BUTTON_EVENTS________________________//
	//		 //_CLICK_
	p_ServicesButton->Bind(wxEVT_LEFT_UP, &CNNBSS_SC_TitleBar::OnAnyButtonClick, this);
	p_LayoutOptionsButton->Bind(wxEVT_LEFT_UP, &CNNBSS_SC_TitleBar::OnAnyButtonClick, this);
	p_PreferencesButton->Bind(wxEVT_LEFT_UP, &CNNBSS_SC_TitleBar::OnAnyButtonClick, this);
	p_AboutButton->Bind(wxEVT_LEFT_UP, &CNNBSS_SC_TitleBar::OnAnyButtonClick, this);

	MinimizeButton->Bind(wxEVT_LEFT_UP, &CNNBSS_SC_TitleBar::OnAnyButtonClick, this);
	RestoreButton->Bind(wxEVT_LEFT_UP, &CNNBSS_SC_TitleBar::OnAnyButtonClick, this);
	QuitButton->Bind(wxEVT_LEFT_UP, &CNNBSS_SC_TitleBar::OnAnyButtonClick, this);
	//      //

	// Base Sizer
	wxBoxSizer* p_BaseSizer = new wxBoxSizer(wxVERTICAL);
	p_BaseSizer->Add(hsizer,wxSizerFlags().Proportion(1).Expand());
	SetSizer(p_BaseSizer);
}

void CNNBSS_SC_TitleBar::TitleBarOnLeftDown(wxMouseEvent& evt)
{
	CaptureMouse();
	wxPoint pos = ::wxGetMousePosition();
	wxPoint origin = GetActiveUXFrame()->GetPosition();
	int dx = pos.x - origin.x;
	int dy = pos.y - origin.y;
	m_MouseDelta = wxPoint(dx, dy);
}

void CNNBSS_SC_TitleBar::TitleBarOnLeftDClick(wxMouseEvent& evt)
{
	if (this != nullptr)
	{
		if (GetActiveUXFrame()->IsMaximized())
		{
			if (this->HasCapture())
				this->ReleaseMouse();
			GetActiveUXFrame()->CenterOnScreen();
			GetActiveUXFrame()->Maximize(false);
		}
		else
		{
			if (this->HasCapture())
				this->ReleaseMouse();
			GetActiveUXFrame()->CenterOnScreen();
			GetActiveUXFrame()->Maximize(true);
		}
	}
}

void CNNBSS_SC_TitleBar::TitleBarOnLeftUp(wxMouseEvent& evt)
{
	if (HasCapture())
	{
		ReleaseMouse();
	}
}

void CNNBSS_SC_TitleBar::OnAnyButtonClick(wxMouseEvent& evt)
{
	switch (evt.GetId())
	{
	case TB_ID_QUIT:
		GetActiveUXFrame()->Close(true);
		break;
	case TB_ID_RESTORE:

		if (HasCapture())
			ReleaseMouse();

		if (GetActiveUXFrame()->IsMaximized()) {
			GetActiveUXFrame()->CenterOnScreen();
			GetActiveUXFrame()->Maximize(false);
		}
		else {
			GetActiveUXFrame()->CenterOnScreen();
			GetActiveUXFrame()->Maximize(true);
		}
		break;
	case TB_ID_MINIMIZE:
		GetActiveUXFrame()->Iconize(true);
		break;
	default:
		CNNBSSMouseEventsAddressHandle()->OnAnyButtonClick(evt);
		break;
	}

	evt.Skip();
}

void CNNBSS_SC_TitleBar::TitleBarOnMouseMove(wxMouseEvent& evt)
{
	if (evt.Dragging() && evt.LeftIsDown())
	{
		// Don't let maximized frame move
		if (GetActiveUXFrame()->IsMaximized())
			GetActiveUXFrame()->Maximize(false);

		wxPoint pos = wxGetMousePosition();
		GetActiveUXFrame()->Move(wxPoint(pos.x - m_MouseDelta.x, pos.y - m_MouseDelta.y));
	}
}
