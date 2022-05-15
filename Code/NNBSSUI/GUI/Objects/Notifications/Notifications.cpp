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
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/scrolwin.h>
#include <wx/valtext.h>
#endif
#include <wx/artprov.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/display.h>
#include <wx/minifram.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/timer.h>//--- <<< don't forget to implement timer for notification messages

#include "Notifications.h"
// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS UI
#include <NNBSSUI/GUI/Objects/Controls/Button/Button.h>
// Standard C Lib
#include <random>

CNNBSSNotifications::CNNBSSNotifications()
{
	p_Notifications.clear();
};

CNNBSSNotifications::~CNNBSSNotifications()
{
	if (p_Notifications.size() > 0)
	{
		for (int c = 0; c < p_Notifications.size(); c++)
		{
			if (std::get<1>(p_Notifications[c])->GetChildren().GetCount() > 0)
			{
				std::get<1>(p_Notifications[c])->DestroyChildren();
				std::get<1>(p_Notifications[c])->Destroy();
			}
		}
	}
};

// This will lock the main frame unless you call this function again with show = false argument
void CNNBSSNotifications::PopUpBusyInfo(const wxString& infoMsg, bool show)
{
	wxDialog* CurrentFrame = nullptr;

	if (show)
	{
		bool wasInList = false;

		auto createWhenItIsAbsent = [&]()
		{

			// if it's empty, create a new;
			p_Notifications.emplace_back(std::make_tuple(N_TYPE_BUSYINFO,
				new wxDialog(GetActiveUXFrame(), wxID_ANY, "InformationMessage", wxPoint(10, 10),
					wxSize(10, 10), wxNO_BORDER | wxSTAY_ON_TOP,
					wxString::Format("InfoMessageFrame%i", m_frameID))));

			// make CurrentFrame variable equal to recently added one
			auto it = std::find_if(p_Notifications.begin(), p_Notifications.end(),
				[](const std::tuple<int, wxDialog*>& e)
				{	// Lambda expression
					return std::get<0>(e) == N_TYPE_BUSYINFO;
				});

			// If found, get the info and set.
			if (it != p_Notifications.end())
			{
				CurrentFrame = std::get<1>(*it);
			}
			else
			{
				// Theorically, this sequence will never be experienced...
				wxLogFatalError("An unexpected error occured while creating a frame of GUI: MF NOT FOUND IN THE LIST!");
			}
		};

		if (!(p_Notifications.empty()))
		{

			// if it's not empty, check if the wxDialog type exist that we look for.
			auto it = std::find_if(p_Notifications.begin(), p_Notifications.end(),
				[](const std::tuple<int, wxDialog*>& e)
				{
					// Find by type.
					return std::get<0>(e) == N_TYPE_BUSYINFO;
				});

			// If found, get the info and set.
			if (it != p_Notifications.end())
			{
				// This condition is true when Info Panel is visible and recieves call(s)
				wasInList = true;
				CurrentFrame = std::get<1>(*it);
			}
			else
			{
				createWhenItIsAbsent();
			}
		}
		else
		{
			createWhenItIsAbsent();
		}

		const int FrameXPos = wxDisplay(wxDisplay().GetFromWindow((wxWindow*)GetActiveUXFrame())).GetGeometry().x;
		const int FrameYPos = wxDisplay(wxDisplay().GetFromWindow((wxWindow*)GetActiveUXFrame())).GetGeometry().y;

		CurrentFrame->SetPosition(wxPoint(FrameXPos, FrameYPos));

		if (!wasInList)
		{
			//Main
			CurrentFrame->SetBackgroundColour(RGB_COLOURDEF_TRIPLE35);
			//Icon
			wxIcon icon = wxArtProvider::GetIcon(wxART_FIND);
			wxStaticBitmap* IconBitmap = new wxStaticBitmap(CurrentFrame, wxID_ANY, icon, wxPoint(10, 10));

			//Message/text
			BusyInfoText = new wxStaticText(CurrentFrame, wxID_ANY, infoMsg);
			wxFont SYMBOL_EXTENDED_LARGE(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
			BusyInfoText->SetFont(SYMBOL_EXTENDED_LARGE);
			BusyInfoText->SetForegroundColour(*wxWHITE);
			//Fit to background
			BusyInfoMainSizer = new wxBoxSizer(wxHORIZONTAL);
			BusyInfoMainSizer->Add(IconBitmap, 0, wxALL | wxALIGN_CENTER, 10);
			BusyInfoMainSizer->Add(BusyInfoText, 0, wxALL | wxALIGN_CENTER, 10);
			CurrentFrame->SetSizerAndFit(BusyInfoMainSizer);


			CurrentFrame->Show();
			CurrentFrame->CenterOnScreen();
			CurrentFrame->Refresh();
			CurrentFrame->Update();
			// a bit wait for a real refreshed frame. because after this a loading will happen
			Sleep(500);
		}
		else
		{
			// update text
		}
	}
	else
	{

		for (int c = 0; c < p_Notifications.size(); c++)
		{
			if (std::get<0>(p_Notifications[c]) == N_TYPE_BUSYINFO)
			{
				std::get<1>(p_Notifications[c])->DestroyChildren();
				std::get<1>(p_Notifications[c])->Destroy();
				p_Notifications.erase(p_Notifications.begin() + c);
			}
		}

	}
}

void CNNBSSNotifications::PopUpInformationMessage(const wxString& infoMsg, unsigned int timeout)
{
	wxDialog* CurrentFrame = nullptr;

	bool wasInList = false;

	auto createWhenItIsAbsent = [&]()
	{

		// if it's empty, create a new;
		p_Notifications.emplace_back(std::make_tuple(N_TYPE_INFORMATION_MESSAGE,
			new wxDialog(GetActiveUXFrame(), wxID_ANY, "InformationMessage", wxPoint(150, 200),
				wxSize(10, 10), wxNO_BORDER | wxSTAY_ON_TOP,
				wxString::Format("InfoMessageFrame%i", m_frameID))));

		// make CurrentFrame variable equal to recently added one
		auto it = std::find_if(p_Notifications.begin(), p_Notifications.end(),
			[](const std::tuple<int, wxDialog*>& e)
			{	// Lambda expression
				return std::get<0>(e) == N_TYPE_INFORMATION_MESSAGE;
			});

		// If found, get the info and set.
		if (it != p_Notifications.end())
		{
			CurrentFrame = std::get<1>(*it);
		}
		else
		{
			// Theorically, this sequence will never be experienced...
			wxLogFatalError("An unexpected error occured while creating a frame of GUI: MF NOT FOUND IN THE LIST!");
		}
	};

	if (!(p_Notifications.empty()))
	{
		// We don't want to let program to show information and error or other type messages at once.
		// so , check if any of them exist to close at first.
		for (int c = 0; c < p_Notifications.size(); c++)
		{
			if (std::get<0>(p_Notifications[c]) == N_TYPE_ERROR_MESSAGE)
			{
				std::get<1>(p_Notifications[c])->DestroyChildren();
				std::get<1>(p_Notifications[c])->Destroy();
				p_Notifications.erase(p_Notifications.begin() + c);
			}
		}

		// if it's not empty, check if the wxDialog type exist that we look for.
		auto it = std::find_if(p_Notifications.begin(), p_Notifications.end(),
			[](const std::tuple<int, wxDialog*>& e)
			{
				// Find by type.
				return std::get<0>(e) == N_TYPE_INFORMATION_MESSAGE;
			});

		// If found, get the info and set.
		if (it != p_Notifications.end())
		{
			// This condition is true when Info Panel is visible and recieves call(s)
			wasInList = true;
			CurrentFrame = std::get<1>(*it);
		}
		else
		{
			createWhenItIsAbsent();
		}
	}
	else
	{
		createWhenItIsAbsent();
	}

	const int FrameXPos = wxDisplay(wxDisplay().GetFromWindow((wxWindow*)GetActiveUXFrame())).GetGeometry().x;
	const int FrameYPos = wxDisplay(wxDisplay().GetFromWindow((wxWindow*)GetActiveUXFrame())).GetGeometry().y;

	CurrentFrame->SetPosition(wxPoint(FrameXPos, FrameYPos));

	if (!wasInList)
	{
		if (CurrentFrame != nullptr)
		{
			if (CurrentFrame->IsShownOnScreen())
			{
				return;
			}
		}
		else
		{
			wxLogFatalError("An unexpected error occured while creating a frame of GUI: MF NOT FOUND IN THE LIST!(ID:2)");
		}
		//Main
		CurrentFrame->SetBackgroundColour(RGB_COLOURDEF_NNBSSBLUE);
		//Icon
		wxIcon icon = wxArtProvider::GetIcon(wxART_INFORMATION);
		wxStaticBitmap* IconBitmap = new wxStaticBitmap(CurrentFrame, wxID_ANY, icon, wxPoint(10, 10));
		//Message/text
		InformationMessageText = new wxStaticText(CurrentFrame, wxID_ANY, infoMsg);
		wxFont SYMBOL_EXTENDED_LARGE(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
		InformationMessageText->SetFont(SYMBOL_EXTENDED_LARGE);
		InformationMessageText->SetForegroundColour(*wxWHITE);
		//Close button
		closeButton = new wxButton(CurrentFrame, N_TYPE_INFORMATION_MESSAGE, "X", wxDefaultPosition, wxSize(25, 25), wxNO_BORDER);
		closeButton->SetBackgroundColour(RGB_COLOURDEF_NNBSSBLUE);
		closeButton->SetForegroundColour(*wxWHITE);
		closeButton->SetFont(SYMBOL_EXTENDED_LARGE);
		//Fit to background
		ComponentSizer = new wxBoxSizer(wxHORIZONTAL);
		ComponentSizer->Add(IconBitmap, 0, wxALL | wxALIGN_CENTER, 10);
		ComponentSizer->Add(InformationMessageText, 0, wxALL | wxALIGN_CENTER, 10);
		ComponentSizer->Add(closeButton, 0, wxALL | wxALIGN_CENTER, 10);
		CurrentFrame->SetSizerAndFit(ComponentSizer);

		// show this with an effect
		CurrentFrame->ShowWithEffect(wxSHOW_EFFECT_EXPAND, timeout);
		closeButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CNNBSSNotifications::OnPressClose, this);
	}
	else
	{
		if (!(CurrentFrame->IsVisible()))
			CurrentFrame->ShowWithEffect(wxSHOW_EFFECT_EXPAND, timeout);
		else
		{
			CurrentFrame->Hide();
			CurrentFrame->ShowWithEffect(wxSHOW_EFFECT_ROLL_TO_RIGHT, timeout);
		}
		static int lineCount = 0;
		if (!(InformationMessageText->GetLabel().empty()) && lineCount <= 10) {
			lineCount++;
			wxString oldString = InformationMessageText->GetLabel();
			InformationMessageText->SetLabel(wxString::Format("%s\n%s", oldString, infoMsg));
		}
		else
		{
			lineCount = 0;
			InformationMessageText->SetLabel(infoMsg);
		}
		ComponentSizer->Layout();
		ComponentSizer->Fit(CurrentFrame);

		CurrentFrame->Refresh(); CurrentFrame->Update();
	}

	m_frameID++;
}

void CNNBSSNotifications::PopUpErrorMessage(const wxString& infoMsg, unsigned int timeout)
{
	wxDialog* CurrentFrame = nullptr;

	bool wasInList = false;

	auto createWhenItIsAbsent = [&]()
	{
		// if it's empty, create a new;
		p_Notifications.emplace_back(std::make_tuple(N_TYPE_ERROR_MESSAGE,
			new wxDialog(GetActiveUXFrame(), wxID_ANY, "InformationMessage", wxPoint(150, 200),
				wxSize(10, 10), wxNO_BORDER | wxSTAY_ON_TOP,
				wxString::Format("InfoMessageFrame%i", m_frameID))));

		// make CurrentFrame variable equal to recently added one
		auto it = std::find_if(p_Notifications.begin(), p_Notifications.end(),
			[](const std::tuple<int, wxDialog*>& e)
			{	// Lambda expression
				return std::get<0>(e) == N_TYPE_ERROR_MESSAGE;
			});

		// If found, get the info and set.
		if (it != p_Notifications.end())
		{
			CurrentFrame = std::get<1>(*it);
		}
		else
		{
			// Theorically, this sequence will never be experienced...
			wxLogFatalError("An unexpected error occured while creating a frame of GUI: MF NOT FOUND IN THE LIST!");
		}
	};

	if (!(p_Notifications.empty()))
	{
		// We don't want to let program to show information and error or other type messages at once.
		// so , check if any of them exist to close at first.
		for (int c = 0; c < p_Notifications.size(); c++)
		{
			if (std::get<0>(p_Notifications[c]) == N_TYPE_INFORMATION_MESSAGE)
			{
				std::get<1>(p_Notifications[c])->DestroyChildren();
				std::get<1>(p_Notifications[c])->Destroy();
				p_Notifications.erase(p_Notifications.begin() + c);
			}
		}

		// if it's not empty, check if the wxDialog type exist that we look for.
		auto it = std::find_if(p_Notifications.begin(), p_Notifications.end(),
			[](const std::tuple<int, wxDialog*>& e)
			{
				// Find by type.
				return std::get<0>(e) == N_TYPE_ERROR_MESSAGE;
			});

		// If found, get the info and set.
		if (it != p_Notifications.end())
		{
			// This condition is true when Info Panel is visible and recieves call(s)
			wasInList = true;
			CurrentFrame = std::get<1>(*it);
		}
		else
		{
			createWhenItIsAbsent();
		}
	}
	else
	{
		createWhenItIsAbsent();
	}

	const int FrameXPos = wxDisplay(wxDisplay().GetFromWindow((wxWindow*)GetActiveUXFrame())).GetGeometry().x;
	const int FrameYPos = wxDisplay(wxDisplay().GetFromWindow((wxWindow*)GetActiveUXFrame())).GetGeometry().y;

	if (!wasInList)
	{
		if (CurrentFrame != nullptr)
		{
			if (CurrentFrame->IsShownOnScreen())
			{
				return;
			}
		}
		else
		{
			wxLogFatalError("An unexpected error occured while creating a frame of GUI: MF NOT FOUND IN THE LIST!(ID:2)");
		}
		//Main
		CurrentFrame->SetPosition(wxPoint(FrameXPos, FrameYPos));
		CurrentFrame->SetBackgroundColour(RGB_COLOURDEF_NNBSSRED);
		//Icon
		wxIcon icon = wxArtProvider::GetIcon(wxART_ERROR);
		wxStaticBitmap* IconBitmap = new wxStaticBitmap(CurrentFrame, wxID_ANY, icon, wxPoint(10, 10));
		//Message/text
		InformationMessageText = new wxStaticText(CurrentFrame, wxID_ANY, infoMsg);
		const wxFont SYMBOL_EXTENDED_LARGE(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
		InformationMessageText->SetFont(SYMBOL_EXTENDED_LARGE);
		InformationMessageText->SetForegroundColour(*wxWHITE);
		//Close button
		closeButton = new wxButton(CurrentFrame, N_TYPE_ERROR_MESSAGE, "X", wxDefaultPosition, wxSize(25, 25), wxNO_BORDER);
		closeButton->SetBackgroundColour(RGB_COLOURDEF_NNBSSRED);
		closeButton->SetForegroundColour(*wxWHITE);
		closeButton->SetFont(SYMBOL_EXTENDED_LARGE);
		//Fit to background
		ComponentSizer = new wxBoxSizer(wxHORIZONTAL);
		ComponentSizer->Add(IconBitmap, 0, wxALL | wxALIGN_CENTER, 10);
		ComponentSizer->Add(InformationMessageText, 0, wxALL | wxALIGN_CENTER, 10);
		ComponentSizer->Add(closeButton, 0, wxALL | wxALIGN_CENTER, 10);
		CurrentFrame->SetSizerAndFit(ComponentSizer);

		// show this with an effect
		CurrentFrame->ShowWithEffect(wxSHOW_EFFECT_EXPAND, timeout);
		closeButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CNNBSSNotifications::OnPressClose, this);
	}
	else
	{
		if (m_OldDisplayID != wxDisplay().GetFromWindow((wxWindow*)GetActiveUXFrame()))
		{
			CurrentFrame->SetPosition(wxPoint(FrameXPos, FrameYPos));

			m_OldDisplayID = wxDisplay().GetFromWindow((wxWindow*)GetActiveUXFrame());
		}

		if (!(CurrentFrame->IsVisible()))
		{
			CurrentFrame->ShowWithEffect(wxSHOW_EFFECT_EXPAND, timeout);
		}
		else
		{
			CurrentFrame->Hide();
			CurrentFrame->ShowWithEffect(wxSHOW_EFFECT_ROLL_TO_RIGHT, timeout);
		}

		static int lineCount = 0;
		if (!(InformationMessageText->GetLabel().empty()) && lineCount <= 10) {
			lineCount++;
			const wxString oldString = InformationMessageText->GetLabel();
			InformationMessageText->SetLabel(wxString::Format("%s\n%s", oldString, infoMsg));
		}
		else
		{
			lineCount = 0;
			InformationMessageText->SetLabel(infoMsg);
		}
		ComponentSizer->Layout();
		ComponentSizer->Fit(CurrentFrame);

		CurrentFrame->Refresh(); CurrentFrame->Update();
	}

	m_frameID++;

}

void CNNBSSNotifications::OnPressClose(wxCommandEvent& evt)
{
	for (int c = 0; c < p_Notifications.size(); c++)
	{
		if (std::get<0>(p_Notifications[c]) == evt.GetId())
		{
			std::get<1>(p_Notifications[c])->Close();
			p_Notifications.erase(p_Notifications.begin() + c);
		}
	}
	evt.Skip();
}

CNNBSSMessageBox::CNNBSSMessageBox(const wxString& message, const wxString& caption, long style, wxWindow* parent, int x, int y)
	: wxDialog(parent, wxID_ANY, caption, wxPoint(x, y), wxDefaultSize, style)
{
	SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	wxBoxSizer* p_BaseSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* p_Header = new wxPanel(this, wxID_ANY);
	p_Header->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	wxPanel* p_Body = new wxPanel(this, wxID_ANY);
	p_Body->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	wxPanel* p_SubBody = new wxPanel(this, wxID_ANY);
	p_SubBody->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	wxBoxSizer* p_HeaderSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* p_BodySizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* p_SubBodySizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* p_Caption = new wxStaticText(p_Header, wxID_ANY, caption);
	p_Caption->SetForegroundColour(RGB_COLOURDEF_WHITE);

	wxStaticText* p_Message = new wxStaticText(p_Body, wxID_ANY, message);
	p_Message->SetForegroundColour(RGB_COLOURDEF_WHITE);

	CNNBSSControlButton* p_OK = new CNNBSSControlButton(p_SubBody, wxID_OK, _("OK"), wxDefaultPosition, wxSize(64, 32));
	p_OK->Bind(wxEVT_LEFT_UP, &CNNBSSMessageBox::OnAnyButtonUP, this);
	p_OK->SetFocus();

	p_HeaderSizer->AddSpacer(10);
	p_HeaderSizer->Add(p_Caption, wxSizerFlags().CenterVertical());
	p_HeaderSizer->AddSpacer(10);
	p_HeaderSizer->AddStretchSpacer();
	if (style & wxYES_NO)
	{
		CNNBSSControlButton* p_XButton = new CNNBSSControlButton(p_Header, wxID_CANCEL, wxEmptyString, wxDefaultPosition, wxSize(64, 32));
		p_XButton->Bind(wxEVT_LEFT_UP, &CNNBSSMessageBox::OnAnyButtonUP, this);
		p_XButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_HEADER_X_BUTTON), CNNBSSControlButton::ButtonStates::BS_DEFAULT);
		p_HeaderSizer->Add(p_XButton);
	}
	p_Header->SetSizer(p_HeaderSizer);

	p_BodySizer->AddSpacer(10);
	p_BodySizer->Add(p_Message, wxSizerFlags().CenterVertical());
	p_BodySizer->AddSpacer(10);
	p_Body->SetSizer(p_BodySizer);

	p_SubBodySizer->Add(p_OK, wxSizerFlags().Proportion(1));
	if (style & wxYES_NO)
	{
		CNNBSSControlButton* p_CANCEL = new CNNBSSControlButton(p_SubBody, wxID_NO, _("Cancel"), wxDefaultPosition, wxSize(64, 32));
		p_CANCEL->Bind(wxEVT_LEFT_UP, &CNNBSSMessageBox::OnAnyButtonUP, this);
		p_SubBodySizer->Add(p_CANCEL, wxSizerFlags().Proportion(1));
	}
	p_SubBody->SetSizer(p_SubBodySizer);

	p_BaseSizer->Add(p_Header, wxSizerFlags().Proportion(1).Expand());
	p_BaseSizer->AddSpacer(10);
	p_BaseSizer->Add(p_Body, wxSizerFlags().Proportion(1).Expand());
	p_BaseSizer->AddSpacer(10);
	p_BaseSizer->Add(p_SubBody, wxSizerFlags().Proportion(1).Expand());
	SetSizer(p_BaseSizer);
	Fit();
	SetAutoLayout(true);

	CenterOnScreen();// hardcoded to be centered
}

void CNNBSSMessageBox::OnAnyButtonUP(wxMouseEvent& evt)
{
	EndModal(evt.GetId());
	evt.Skip();
}
