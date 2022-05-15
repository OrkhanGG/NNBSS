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
#include <wx/dialog.h>
#include <wx/bmpbuttn.h>
#include <wx/checkbox.h>

#include "LogIn.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

CNNBSSLogIn::~CNNBSSLogIn()
{
	CNNBSSLogInAddressHandle(true);
}

void CNNBSSLogIn::_Create(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
{
	Create(parent, id, title, pos, size, style);
}

void CNNBSSLogIn::_SetupLogInInterface()
{
	// Create background under any case
	panel = new wxPanel(this, wxID_ANY);
	panel->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	if (CNNBSSAuthorizationAddressHandle()->_HaveValidProfile())
	{
		//LOGO on title bar
		p_LOGO = new wxBitmapButton(panel, wxID_ANY, NNBSS_PNG2BMP(NNBSSART_LOGO_MEDIUM_PNG), wxDefaultPosition, wxSize(128, 128), wxBORDER_NONE);
		p_LOGO->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

		m_usernameLabel = new wxStaticText(panel, wxID_ANY, _("Username"), wxDefaultPosition);
		m_usernameLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);

		m_usernameEntry = new wxTextCtrl(panel, wxID_ANY);
		m_usernameEntry->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_usernameEntry->SetForegroundColour(RGB_COLOURDEF_WHITE);

		m_passwordLabel = new wxStaticText(panel, wxID_ANY, _("Password"), wxDefaultPosition);
		m_passwordLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);

		m_passwordEntry = new wxTextCtrl(panel, wxID_ANY, wxString(""),
			wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
		m_passwordEntry->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_passwordEntry->SetForegroundColour(RGB_COLOURDEF_WHITE);

		p_RememberMe = new wxCheckBox(panel, wxID_ANY, _("Remember Me"), wxDefaultPosition,
			wxDefaultSize, wxNO_BORDER);
		p_RememberMe->SetForegroundColour(RGB_COLOURDEF_WHITE);

		m_buttonLogin = new wxButton(panel, NNBSS_EVT_OK, _("Login"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		m_buttonLogin->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_buttonLogin->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_buttonLogin->SetBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_IDLE));

		m_buttonQuit = new wxButton(panel, wxID_CANCEL, _("Quit"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		m_buttonQuit->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_buttonQuit->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_buttonQuit->SetBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_IDLE));

		m_informationLabel = new wxStaticText(panel, wxID_ANY, _("After 6 wrong try program will be terminated."));
		m_informationLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_informationLabel->Center();
		m_informationLabel->Hide();

		// Setup sizers
		p_ElementsSizer = new wxGridSizer(4, 1, 0, 0);
		p_ElementsSizer->Add(m_usernameLabel, 0, wxALIGN_CENTER, 0);
		p_ElementsSizer->Add(m_usernameEntry, 0, wxALIGN_CENTER, 0);
		p_ElementsSizer->Add(m_passwordLabel, 0, wxALIGN_CENTER, 10);
		p_ElementsSizer->Add(m_passwordEntry, 0, wxALIGN_CENTER, 10);

		p_ElementSizerHorizontal = new wxBoxSizer(wxHORIZONTAL);
		p_ElementSizerHorizontal->AddStretchSpacer();
		p_ElementSizerHorizontal->Add(m_buttonLogin, 0, wxALIGN_CENTER, 10);
		p_ElementSizerHorizontal->AddSpacer(FromDIP(5));
		p_ElementSizerHorizontal->Add(m_buttonQuit, 0, wxALIGN_CENTER, 10);
		p_ElementSizerHorizontal->AddStretchSpacer();

		VerticalSizer = new wxBoxSizer(wxVERTICAL);
		VerticalSizer->AddSpacer(FromDIP(20));
		VerticalSizer->Add(p_LOGO, 0, wxALIGN_CENTER, 10);
		VerticalSizer->AddSpacer(FromDIP(10));
		VerticalSizer->Add(p_ElementsSizer, 0, wxALIGN_CENTER, 10);
		VerticalSizer->AddSpacer(FromDIP(20));
		VerticalSizer->Add(p_RememberMe, 0, wxALIGN_CENTER, 10);
		VerticalSizer->AddSpacer(FromDIP(20));
		VerticalSizer->Add(p_ElementSizerHorizontal, 0, wxALIGN_CENTER, 10);;
		VerticalSizer->Add(m_informationLabel, 0, wxALIGN_CENTER, 10);
		VerticalSizer->AddSpacer(FromDIP(40));
		VerticalSizer->Layout();

		HorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
		HorizontalSizer->AddSpacer(FromDIP(40));
		HorizontalSizer->Add(VerticalSizer, 1, wxALIGN_CENTER);
		HorizontalSizer->AddSpacer(FromDIP(35));
		panel->SetSizerAndFit(HorizontalSizer);

	}
	else
	{
		//LOGO on title bar
		p_LOGO = new wxBitmapButton(panel, wxID_ANY, NNBSS_PNG2BMP(NNBSSART_LOGO_MEDIUM_PNG), wxDefaultPosition, wxSize(128, 128), wxBORDER_NONE);
		p_LOGO->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);


		p_WarningTitle = new wxButton(panel, wxID_ANY, _("Please, set admin username and password."),
			wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		p_WarningTitle->SetBitmap(NNBSS_PNG2BMP(NNBSSART_WARNING_SMALL_IDLE));
		p_WarningTitle->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_WarningTitle->SetForegroundColour(RGB_COLOURDEF_WHITE);

		wxString passwordInfoText = _("Use a password that has 8 to 32 characters.\n It can be a combination of letter(s), number(s) and symbol(s)\n with at least two kinds of them.");
		p_PasswordInfo = new wxButton(panel, wxID_ANY, passwordInfoText,
			wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		p_PasswordInfo->SetBitmap(NNBSS_PNG2BMP(NNBSSART_WARNING_SMALL_IDLE));
		p_PasswordInfo->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_PasswordInfo->SetForegroundColour(RGB_COLOURDEF_WHITE);

		m_usernameLabel = new wxStaticText(panel, wxID_ANY, _("Username"), wxDefaultPosition);
		m_usernameLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);

		m_usernameEntry = new wxTextCtrl(panel, wxID_ANY);
		m_usernameEntry->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_usernameEntry->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_usernameEntry->SetHint(_("Admin Username"));

		m_passwordLabel = new wxStaticText(panel, wxID_ANY, _("Password"), wxDefaultPosition);
		m_passwordLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);

		m_passwordEntry = new wxTextCtrl(panel, wxID_ANY, wxString(""),
			wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
		m_passwordEntry->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_passwordEntry->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_passwordEntry->SetHint(_("Admin Password"));

		m_buttonLogin = new wxButton(panel, NNBSS_EVT_OK, _("Register"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		m_buttonLogin->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_buttonLogin->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_buttonLogin->SetBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_IDLE));

		m_buttonQuit = new wxButton(panel, wxID_CANCEL, _("Quit"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		m_buttonQuit->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_buttonQuit->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_buttonQuit->SetBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_IDLE));

		m_informationLabel = new wxStaticText(panel, wxID_ANY, _("After 6 wrong try program will be terminated."));
		m_informationLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_informationLabel->Center();
		m_informationLabel->Hide();

		p_RememberMe = new wxCheckBox(panel, wxID_ANY, _("Remember Me"), wxDefaultPosition,
			wxDefaultSize, wxNO_BORDER);
		p_RememberMe->SetForegroundColour(RGB_COLOURDEF_WHITE);

		// Setup sizers
		p_ElementsSizer = new wxGridSizer(4, 1, 0, 0);
		p_ElementsSizer->Add(m_usernameLabel, 0, wxALIGN_CENTER, 0);
		p_ElementsSizer->Add(m_usernameEntry, 0, wxALIGN_CENTER, 0);
		p_ElementsSizer->Add(m_passwordLabel, 0, wxALIGN_CENTER, 10);
		p_ElementsSizer->Add(m_passwordEntry, 0, wxALIGN_CENTER, 10);

		p_ElementSizerHorizontal = new wxBoxSizer(wxHORIZONTAL);
		p_ElementSizerHorizontal->AddStretchSpacer();
		p_ElementSizerHorizontal->Add(m_buttonLogin, 0, wxALIGN_CENTER, 10);
		p_ElementSizerHorizontal->AddSpacer(FromDIP(5));
		p_ElementSizerHorizontal->Add(m_buttonQuit, 0, wxALIGN_CENTER, 10);
		p_ElementSizerHorizontal->AddStretchSpacer();

		VerticalSizer = new wxBoxSizer(wxVERTICAL);
		VerticalSizer->AddSpacer(FromDIP(20));
		VerticalSizer->Add(p_LOGO, 0, wxALIGN_CENTER, 10);
		VerticalSizer->Add(p_WarningTitle, 0, wxALIGN_CENTER, 10);
		VerticalSizer->AddSpacer(FromDIP(10));
		VerticalSizer->Add(p_ElementsSizer, 0, wxALIGN_CENTER, 10);
		VerticalSizer->AddSpacer(FromDIP(5));
		VerticalSizer->Add(p_PasswordInfo, 0, wxALIGN_CENTER, 10);
		VerticalSizer->AddSpacer(FromDIP(20));
		VerticalSizer->Add(p_RememberMe, 0, wxALIGN_CENTER, 10);
		VerticalSizer->AddSpacer(FromDIP(20));
		VerticalSizer->Add(p_ElementSizerHorizontal, 0, wxALIGN_CENTER, 10);
		VerticalSizer->AddSpacer(FromDIP(5));
		VerticalSizer->Add(m_informationLabel, 0, wxALIGN_CENTER, 10);
		VerticalSizer->AddSpacer(FromDIP(30));
		VerticalSizer->Layout();

		HorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
		HorizontalSizer->AddSpacer(FromDIP(40));
		HorizontalSizer->Add(VerticalSizer, 1, wxALIGN_CENTER);
		HorizontalSizer->AddSpacer(FromDIP(35));
		panel->SetSizerAndFit(HorizontalSizer);

	}

	//// Event binds
		//    // Click
	m_buttonLogin->Bind(wxEVT_LEFT_UP, &CNNBSSLogIn::OnLogIn, this);
	m_buttonQuit->Bind(wxEVT_LEFT_UP, &CNNBSSLogIn::OnQuit, this);

	// Since all cases will be executed with "panel", there's no problem to add the "panel" into the sizer
	// it's like "global" this way
	MainSizer = new wxBoxSizer(wxVERTICAL);
	MainSizer->Add(panel, 1, wxEXPAND);
	this->SetSizerAndFit(MainSizer);
	MainSizer->Layout();

	this->Center();
	this->SetWindowStyle(wxFRAME_SHAPED | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP);

}

void CNNBSSLogIn::OnLogIn(wxMouseEvent& evt)
{
	if (!CheckLogInDemands())
		return;

	EndModal(wxID_OK);
	evt.Skip();
}

void CNNBSSLogIn::OnQuit(wxMouseEvent& evt)
{
	evt.Skip();
}

bool CNNBSSLogIn::CheckLogInDemands()
{

	wxString username = m_usernameEntry->GetValue(), password = m_passwordEntry->GetValue();

	if (username.empty() || password.empty())
	{
		NNBSSMessageBoxSimple(_("Error"), _("Both username and password must be entered!"), wxICON_ERROR);
		return false;
	}

	if (username.size() > 32 || password.size() > 32)
	{
		NNBSSMessageBoxSimple(_("Error"), _("Username or password length can't be bigger than 32 characters!"), wxICON_ERROR);
		return false;
	}
	if (!CNNBSSAuthorizationAddressHandle()->_HaveValidProfile())
	{
		CNNBSSAuthorizationAddressHandle()->_SaveProfile("Admin", username, password);
	}
	else
	{
		// Check if the username&password is entered accurately
		if (!CNNBSSAuthorizationAddressHandle()->_CheckLogInInfo(username, password))
		{
			NNBSSMessageBoxSimple(_("Error"), _("Username or password is incorrect!"), wxICON_ERROR);
			return false;
		}
	}
	// Registration / First entering

	wxString currentProfileName = m_usernameEntry->GetValue();
	if (p_RememberMe->IsChecked())
		CNNBSSAuthorizationAddressHandle()->_SetRemember(currentProfileName,true);

	return true;
}