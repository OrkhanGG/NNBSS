///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

class wxWindow;
class wxButton;
class wxStaticBitmap;
class wxStaticText;
class wxBoxSizer;
class wxString;
class wxCommandEvent;
class wxTimerEvent;

// Standard C Lib
#include <tuple>
#include <vector>

// wxWidgets
#include <wx/dialog.h>

class CNNBSSNotifications
{
public:

	enum type 
	{
		N_TYPE_BUSYINFO = 0,
		N_TYPE_INFORMATION_MESSAGE,
	    N_TYPE_ERROR_MESSAGE,
		N_TYPE_DIALOG
	};

	CNNBSSNotifications();
	~CNNBSSNotifications();

	void PopUpBusyInfo(const wxString& infoMsg, bool show = true);
	void PopUpInformationMessage(const wxString& infoMsg, unsigned int timeout = 100U);
	void PopUpErrorMessage(const wxString& infoMsg, unsigned int timeout = 100U);

	void OnPressClose(wxCommandEvent& evt);

private:
	//___________________GENERAL____________________________//
		// Int = type, wxMiniFrame = The frame will be popped up
	std::vector<std::tuple<long, wxDialog*>> p_Notifications;
	
	//___________________BusyInfo___________________________//
	wxStaticText* BusyInfoText = nullptr;
	wxBoxSizer* BusyInfoMainSizer = nullptr; 

	//___________________Messages___________________________//
	long m_frameID = 0;// This class can't have instances, that's why we do this without static
	wxStaticText* InformationMessageText = nullptr;
	wxButton* closeButton = nullptr;
	wxBoxSizer* ComponentSizer = nullptr;
	wxStaticBitmap* IconBitmap = nullptr;

	long m_OldDisplayID = -1;
};

class CNNBSSMessageBox : public wxDialog
{
public:
	CNNBSSMessageBox(const wxString& message,
		const wxString& caption,
		long style = 0L,
		wxWindow* parent = nullptr,
		int x = 0, int y = 0);

private:
	void OnAnyButtonUP(wxMouseEvent& evt);
};

#endif // ! NOTIFICATIONS_H