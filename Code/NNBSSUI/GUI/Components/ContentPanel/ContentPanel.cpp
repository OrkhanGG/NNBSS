///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ContentPanel.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/control.h"
#include "wx/settings.h"
#endif // WX_PRECOMP
#include <wx/srchctrl.h>

#include "ContentPanel.h"
// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS UI
#include "NNBSSUI/GUI/Objects/Controls/Button/Button.h"

constexpr int NNBSS_EVT_SEARCH_DEVICE_LIST = 2943;
constexpr int NNBSS_EVT_SEARCH_NOTIFICATION_LIST = 2944;


CNNBSSContentPanel::CNNBSSContentPanel() : _IsCreated(false), _CurrentContent(NNBSS_CONTENTPAGE_CURRENTCONTENT_SERVICES)
{
}

CNNBSSContentPanel::~CNNBSSContentPanel()
{
}

CNNBSSContentPanel* CNNBSSContentPanel::GetPanel()
{
	return this;
}

void CNNBSSContentPanel::_SetupColours(const wxColour& mBackgroundColour)
{
	SetBackgroundColour(mBackgroundColour);
}

void CNNBSSContentPanel::_ShowServicesContent(bool destroy)
{
	if (!destroy)
	{
		p_ServicesPanel = new wxPanel(this);
		p_ServicesPanel->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

		p_ServicesPanelGeneralSizer = new wxBoxSizer(wxVERTICAL);
		p_ServicesPanelGeneralSizer->Add(p_ServicesPanel, wxSizerFlags().Proportion(1).Expand());
		SetSizer(p_ServicesPanelGeneralSizer);

		wxFont ButtonFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		wxSize m_ServicesButtonSize(256, 128);
		wxDirection m_ServicesButtonIconPosition = wxLEFT;
		
		DevicesButton = new CNNBSSControlButton(p_ServicesPanel, NNBSS_EVT_SHOW_DEVICES_PANEL, _("Device List"),
			wxDefaultPosition, m_ServicesButtonSize, wxNO_BORDER);
		DevicesButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		DevicesButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		DevicesButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_DEVICE_IDLE));
		DevicesButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_DEVICE_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
		DevicesButton->SetButtonHelpLabel(_("Add/remove or Import/Export \ndevices."));
		DevicesButton->SetButtonFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		DevicesButton->SetButtonRoundness(10);
		DevicesButton->SetButtonLabelAlignment(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

		GridViewButton = new CNNBSSControlButton(p_ServicesPanel, NNBSS_EVT_SHOW_GRIDVIEW_PANEL, _("Grid View"),
			wxDefaultPosition, m_ServicesButtonSize, wxNO_BORDER);
		GridViewButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		GridViewButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		GridViewButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_GRIDVIEW_IDLE));
		GridViewButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_GRIDVIEW_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
		GridViewButton->SetButtonHelpLabel(_("Project added camera devices\nto screen."));
		GridViewButton->SetButtonFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		GridViewButton->SetButtonRoundness(10);
		GridViewButton->SetButtonLabelAlignment(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

		NotificationSystemButton = new CNNBSSControlButton(p_ServicesPanel, NNBSS_EVT_SHOW_NOTIFICATIONS_PANEL,
			_("Notifications"),
			wxDefaultPosition, m_ServicesButtonSize, wxNO_BORDER);
		NotificationSystemButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		NotificationSystemButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		NotificationSystemButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_NOTIFICATIONS_IDLE));
		NotificationSystemButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_NOTIFICATIONS_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
		NotificationSystemButton->SetButtonHelpLabel(_("Notifications are displayed in\nthis panel."));
		NotificationSystemButton->SetButtonFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		NotificationSystemButton->SetButtonRoundness(10);
		NotificationSystemButton->SetButtonLabelAlignment(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

		PlaybackPanelButton = new CNNBSSControlButton(p_ServicesPanel, NNBSS_EVT_SHOW_PLAYBACK_PANEL,
			_("Playback"),
			wxDefaultPosition, m_ServicesButtonSize, wxNO_BORDER);
		PlaybackPanelButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		PlaybackPanelButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		PlaybackPanelButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_PLAYBACK_IDLE));
		PlaybackPanelButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_PLAYBACK_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
		PlaybackPanelButton->SetButtonHelpLabel(_("Playback all recorded moving\nvisual media and perform \ndetections."));
		PlaybackPanelButton->SetButtonFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		PlaybackPanelButton->SetButtonRoundness(10);
		PlaybackPanelButton->SetButtonLabelAlignment(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

		ProfilesPanelButton = new CNNBSSControlButton(p_ServicesPanel, NNBSS_EVT_SHOW_PROFILES_PANEL,
			_("Profiles"),
			wxDefaultPosition, m_ServicesButtonSize, wxNO_BORDER);
		ProfilesPanelButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		ProfilesPanelButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		ProfilesPanelButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_PROFILES_IDLE));
		ProfilesPanelButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_PROFILES_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
		ProfilesPanelButton->SetButtonHelpLabel(_("Add/remove access by Face ID."));
		ProfilesPanelButton->SetButtonFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		ProfilesPanelButton->SetButtonRoundness(10);
		ProfilesPanelButton->SetButtonLabelAlignment(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

		DetectionPanelButton = new CNNBSSControlButton(p_ServicesPanel, NNBSS_EVT_SHOW_DETECTIONCONTENT_PANEL,
			_("Detection"),wxDefaultPosition, m_ServicesButtonSize, wxNO_BORDER);
		DetectionPanelButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		DetectionPanelButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		DetectionPanelButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_DETECTION_IDLE));
		DetectionPanelButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_SERVICES_DETECTION_HOVER),CNNBSSControlButton::ButtonStates::BS_HOVER);
		DetectionPanelButton->SetButtonHelpLabel(_("Add ready detection model\nsamples or your own."));
		DetectionPanelButton->SetButtonFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		DetectionPanelButton->SetButtonRoundness(10);
		DetectionPanelButton->SetButtonLabelAlignment(10);
		DetectionPanelButton->SetButtonLabelAlignment(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

		// Contents of services
		ServicesSizer = new wxGridSizer(2, 4, 5, 5);

		ServicesSizer->Add(DevicesButton, wxSizerFlags().Proportion(1));
		ServicesSizer->Add(GridViewButton, wxSizerFlags().Proportion(1));
		ServicesSizer->Add(NotificationSystemButton, wxSizerFlags().Proportion(1));
		ServicesSizer->Add(PlaybackPanelButton, wxSizerFlags().Proportion(1));
		ServicesSizer->Add(ProfilesPanelButton, wxSizerFlags().Proportion(1));
		ServicesSizer->Add(DetectionPanelButton, wxSizerFlags().Proportion(1));

		p_ServicesContentSizerHorizontal = new wxGridSizer(1, 1, 0, 0);

		p_ServicesContentSizerHorizontal->Add(ServicesSizer,
			wxSizerFlags().Proportion(1).CenterHorizontal().CenterVertical());

		p_ServicesContentSizerHorizontal->FitInside(p_ServicesPanel);
		p_ServicesPanel->SetSizer(p_ServicesContentSizerHorizontal);

		//// Bind hover events for the buttons
		//	// devices button
		DevicesButton->Bind(wxEVT_LEFT_UP, &CNNBSSContentPanel::OnAnyButtonClick, this);
		//// grid view button
		GridViewButton->Bind(wxEVT_LEFT_UP, &CNNBSSContentPanel::OnAnyButtonClick, this);
		//// notification button
		NotificationSystemButton->Bind(wxEVT_LEFT_UP, &CNNBSSContentPanel::OnAnyButtonClick, this);
		//// playback button
		PlaybackPanelButton->Bind(wxEVT_LEFT_UP, &CNNBSSContentPanel::OnAnyButtonClick, this);
		//// profiles button
		ProfilesPanelButton->Bind(wxEVT_LEFT_UP, &CNNBSSContentPanel::OnAnyButtonClick, this);
		//// access button
		DetectionPanelButton->Bind(wxEVT_LEFT_UP, &CNNBSSContentPanel::OnAnyButtonClick, this);
		p_ServicesPanel->Show();

		_CurrentContent = NNBSS_CONTENTPAGE_CURRENTCONTENT_SERVICES;
	}
	else
	{
		if (p_ServicesPanel)
		{
			p_ServicesPanel->Hide();
		}
	}

	CNNBSSAuiManagerAddressHandle()->Update();
}

// CNNBSSControlPanel--| 
//                     |Calls|-> _ShowDevicesContent
// CNBSSMouseEvents----|
void CNNBSSContentPanel::_ShowDevicesContent(bool destroy)
{
	if (!destroy)
	{
		_ShowServicesContent(true);

		p_DevicesPanel = new wxPanel(this, wxID_ANY);
		p_DevicesPanel->SetBackgroundColour(RGB_COLOURDEF_PLACEHOLDER);

		p_DevicesPanelGeneralSizer = new wxBoxSizer(wxVERTICAL);
		p_DevicesPanelGeneralSizer->Add(p_DevicesPanel, wxSizerFlags().Proportion(1).Expand());
		SetSizer(p_DevicesPanelGeneralSizer);

		// Create General Sizer to sort items in vertical order
		wxBoxSizer* DevicePanelSizer = new wxBoxSizer(wxVERTICAL);
		// Create Sizer for -Devices Panel Mangement-
		p_DevicesPanelManagementHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);

		// Create -Devices Panel Mangement-
		p_DevicesPanelManagementGround = new wxPanel(p_DevicesPanel, wxID_ANY, wxDefaultPosition,
			wxSize(10, 64));
		p_DevicesPanelManagementGround->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_AddDeviceButton = new CNNBSSControlButton(p_DevicesPanelManagementGround, NNBSS_EVT_ADD_DEVICE_DIALOG, _("Add"),
			wxDefaultPosition, wxSize(70, 32), wxBORDER_NONE);
		p_RemoveDeviceButton = new CNNBSSControlButton(p_DevicesPanelManagementGround, NNBSS_EVT_REMOVE_DEVICE_DIALOG, _("Remove"),
			wxDefaultPosition, wxSize(70, 32), wxBORDER_NONE);
		p_ImportDevicesButton = new CNNBSSControlButton(p_DevicesPanelManagementGround, wxID_ANY, _("Import"),
			wxDefaultPosition, wxSize(70, 32), wxBORDER_NONE);
		p_ExportDevicesButton = new CNNBSSControlButton(p_DevicesPanelManagementGround, wxID_ANY, _("Export"),
			wxDefaultPosition, wxSize(70, 32), wxBORDER_NONE);
		p_DeviceCount = new wxStaticText(p_DevicesPanelManagementGround, wxID_ANY, wxT(""));
		DevicePanelUpdateDeviceCount();// Update device count label after creation of the text

		// Move these into _SetupColours/_SetupDesign
		p_AddDeviceButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_AddDeviceButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		p_RemoveDeviceButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_RemoveDeviceButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		p_ImportDevicesButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_ImportDevicesButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		p_ExportDevicesButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_ExportDevicesButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		p_DeviceCount->SetForegroundColour(RGB_COLOURDEF_WHITE);

		p_DeviceListSearchCtrl = new wxSearchCtrl(p_DevicesPanelManagementGround, NNBSS_EVT_SEARCH_DEVICE_LIST
			, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		p_DeviceListSearchCtrl->ShowCancelButton(true);
		p_DeviceListSearchCtrl->Bind(wxEVT_SEARCH, &CNNBSSContentPanel::OnSearch, this);
		p_DeviceListSearchCtrl->Bind(wxEVT_SEARCHCTRL_CANCEL_BTN, &CNNBSSContentPanel::OnSearch, this);

		p_DevicesPanelManagementHorizontalSizer->AddSpacer(FromDIP(10));
		p_DevicesPanelManagementHorizontalSizer->Add(p_DeviceCount, wxSizerFlags().Proportion(0).CenterVertical());
		p_DevicesPanelManagementHorizontalSizer->AddStretchSpacer();
		p_DevicesPanelManagementHorizontalSizer->Add(p_DeviceListSearchCtrl, wxSizerFlags().Proportion(0).CenterVertical());
		p_DevicesPanelManagementHorizontalSizer->AddSpacer(FromDIP(10));
		p_DevicesPanelManagementHorizontalSizer->Add(p_AddDeviceButton, wxSizerFlags().Proportion(0));
		p_DevicesPanelManagementHorizontalSizer->AddSpacer(FromDIP(10));
		p_DevicesPanelManagementHorizontalSizer->Add(p_RemoveDeviceButton, wxSizerFlags().Proportion(0));
		p_DevicesPanelManagementHorizontalSizer->AddSpacer(FromDIP(10));
		p_DevicesPanelManagementHorizontalSizer->Add(p_ImportDevicesButton, wxSizerFlags().Proportion(0));
		p_DevicesPanelManagementHorizontalSizer->AddSpacer(FromDIP(10));
		p_DevicesPanelManagementHorizontalSizer->Add(p_ExportDevicesButton, wxSizerFlags().Proportion(0));
		p_DevicesPanelManagementHorizontalSizer->AddSpacer(FromDIP(10));
		p_DevicesPanelManagementGround->SetSizerAndFit(p_DevicesPanelManagementHorizontalSizer);

		// Create Device List-
		CNNBSSDeviceListAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel())->_Create(p_DevicesPanel, wxID_ANY);
		DevicePanelSizer->Add(p_DevicesPanelManagementGround, wxSizerFlags().Proportion(0).Expand());
		DevicePanelSizer->AddSpacer(FromDIP(2));
		DevicePanelSizer->Add(CNNBSSDeviceListAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel()),
			wxSizerFlags().Proportion(1).Expand());
		p_DevicesPanel->SetSizer(DevicePanelSizer);

		p_AddDeviceButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_ADDDEVICE_IDLE));
		p_AddDeviceButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_ADDDEVICE_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);

		p_RemoveDeviceButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_REMOVEDEVICE_IDLE));
		p_RemoveDeviceButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_REMOVEDEVICE_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
		p_ImportDevicesButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GLOBAL_IMPORT_16x16p_IDLE));
		p_ImportDevicesButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GLOBAL_IMPORT_16x16p_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
		p_ExportDevicesButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GLOBAL_EXPORT_16x16p_IDLE));
		p_ExportDevicesButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GLOBAL_EXPORT_16x16p_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);

		// Setup event bind
		p_AddDeviceButton->Bind(wxEVT_LEFT_UP, &CNNBSSContentPanel::OnAnyButtonClick, this);
		p_RemoveDeviceButton->Bind(wxEVT_LEFT_UP, &CNNBSSContentPanel::OnAnyButtonClick, this);
		p_ImportDevicesButton->Bind(wxEVT_LEFT_UP, &CNNBSSContentPanel::OnAnyButtonClick, this);
		p_ExportDevicesButton->Bind(wxEVT_LEFT_UP, &CNNBSSContentPanel::OnAnyButtonClick, this);

		_CurrentContent = NNBSS_CONTENTPAGE_CURRENTCONTENT_DEVICES;
	}
	else
	{
		if (p_DevicesPanel)
			p_DevicesPanel->Hide();
	}

	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSContentPanel::DevicePanelUpdateDeviceCount()
{
	wxString deviceCount = wxString::Format(_("Devices: %i"), (int)SCNNBSSMVMParametersList.size());

	p_DeviceCount->SetLabel(deviceCount);
}

void CNNBSSContentPanel::_ShowGridViewContent(bool destroy)
{
	if (!destroy)
	{
		_ShowServicesContent(true);

		p_GridViewGeneralSizer = new wxBoxSizer(wxVERTICAL);

		// Add camera grid to this page
		p_GridViewGeneralSizer->Add(CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage()),
			wxSizerFlags().Proportion(1).Expand());

		SetSizer(p_GridViewGeneralSizer);

		_CurrentContent = NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW;
	}
	else
	{
		if (p_GridViewPanel)
			p_GridViewPanel->Hide();
	}

	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSContentPanel::_ShowNotificationsContent(bool destroy)
{
	if (!destroy)
	{
		_ShowServicesContent(true);

		wxBoxSizer* p_NotificationsGeneralSizer = new wxBoxSizer(wxVERTICAL);

		wxBoxSizer* p_NotificationsManagementHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);

		wxPanel* p_NotificationsManagementGround = new wxPanel(this, wxID_ANY, wxDefaultPosition,
			wxSize(10, 64));

		p_NotificationsSearchCtrl = new wxSearchCtrl(p_NotificationsManagementGround, NNBSS_EVT_SEARCH_NOTIFICATION_LIST,
			wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		p_NotificationsSearchCtrl->Bind(wxEVT_TEXT_ENTER, &CNNBSSContentPanel::OnSearch, this);

		wxPanel* p_NotificationsSeperatorLine = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1, 2));
		p_NotificationsSeperatorLine->SetBackgroundColour(RGB_COLOURDEF_PLACEHOLDER);

		p_NotificationsManagementHorizontalSizer->AddSpacer(FromDIP(5));
		p_NotificationsManagementHorizontalSizer->Add(p_NotificationsSearchCtrl, wxSizerFlags().Proportion(1).Expand());
		p_NotificationsManagementGround->SetSizerAndFit(p_NotificationsManagementHorizontalSizer);

		wxPanel* p_NotificationPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer* p_NotificationGroudnMainSizer = new wxBoxSizer(wxVERTICAL);
		p_NotificationPanel->SetSizer(p_NotificationGroudnMainSizer);

		CNNBSSNotificationListAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel())->_Create(p_NotificationPanel, wxID_ANY);
		p_NotificationGroudnMainSizer->Add(CNNBSSNotificationListAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel()),
			wxSizerFlags().Proportion(1).Expand());

		p_NotificationsGeneralSizer->Add(p_NotificationPanel, wxSizerFlags().Proportion(1).Expand());
		p_NotificationsGeneralSizer->Add(p_NotificationsSeperatorLine, wxSizerFlags().Proportion(0).Expand());
		p_NotificationsGeneralSizer->AddSpacer(FromDIP(5));
		p_NotificationsGeneralSizer->Add(p_NotificationsManagementGround, wxSizerFlags().Proportion(0).Expand());
		p_NotificationsGeneralSizer->AddSpacer(FromDIP(5));
		SetSizer(p_NotificationsGeneralSizer);

		Layout();

		_CurrentContent = NNBSS_CONTENTPAGE_CURRENTCONTENT_NOTIFICATIONS;
	}
	else
	{

	}

	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSContentPanel::_ShowPlaybackContent(bool destroy)
{
	if (!destroy)
	{
		_ShowServicesContent(true);

		wxBoxSizer* p_PlaybackContentMainSizer = new wxBoxSizer(wxVERTICAL);

		// Add detection samples list to this page
		p_PlaybackContentMainSizer->Add(CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage()),
			wxSizerFlags().Proportion(1).Expand());

		SetSizer(p_PlaybackContentMainSizer);


		Layout();

		_CurrentContent = NNBSS_CONTENTPAGE_CURRENTCONTENT_PLAYBACK;
	}
	else
	{

	}

	CNNBSSAuiManagerAddressHandle()->Update();
}
void CNNBSSContentPanel::_ShowProfilesContent(bool destroy)
{
	if (!destroy)
	{
		_ShowServicesContent(true);

		wxBoxSizer* p_ProfileContentMainSizer = new wxBoxSizer(wxVERTICAL);

		// Add detection samples list to this page
		p_ProfileContentMainSizer->Add(CNNBSSProfilesInterfaceAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage()),
			wxSizerFlags().Proportion(1).Expand());

		SetSizer(p_ProfileContentMainSizer);

		Layout();
		
		_CurrentContent = NNBSS_CONTENTPAGE_CURRENTCONTENT_PROFILES;
	}
	else
	{

	}

	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSContentPanel::_ShowDetectionContent(bool destroy)
{
	if (!destroy)
	{
		_ShowServicesContent(true);

		wxBoxSizer* p_DetectionContentMainSizer = new wxBoxSizer(wxVERTICAL);

		// Add detection samples list to this page
		p_DetectionContentMainSizer->Add(CNNBSSDetectionSamplesListAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage()),
			wxSizerFlags().Proportion(1).Expand());

		SetSizer(p_DetectionContentMainSizer);

		Layout();

		_CurrentContent = NNBSS_CONTENTPAGE_CURRENTCONTENT_DETECTION;
	}
	else
	{

	}

	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSContentPanel::UpdateSearchBar()
{
	// sanity check// don't remove !this
	if (!this && !_IsCreated)
		return;

	switch (GetCurrentContent())
	{
	case NNBSS_CONTENTPAGE_CURRENTCONTENT_DEVICES:
	{
		if (p_DeviceListSearchCtrl)
		{
			// Get current text that's set for all DeviceList pages and update
			p_DeviceListSearchCtrl->SetLabelText(wxString::Format("%s", DeviceList::CNNBSSSearchText(GetCurrentContent(),false/*false means get text*/)));
		}
	}
		break;
	case NNBSS_CONTENTPAGE_CURRENTCONTENT_NOTIFICATIONS:
		break;
	default:
		break;
	}
}

void CNNBSSContentPanel::OnAnyButtonClick(wxMouseEvent& evt)
{
	CNNBSSMouseEventsAddressHandle()->OnAnyButtonClick(evt);
	evt.Skip();
}

void CNNBSSContentPanel::OnSearch(wxCommandEvent& evt)
{
	switch (evt.GetId())
	{
	case NNBSS_EVT_SEARCH_DEVICE_LIST:
		evt.SetInt(NNBSS_CONTENTPAGE_CURRENTCONTENT_DEVICES);
		DeviceList::CNNBSSDeviceListSearch(evt);
		break;
	case NNBSS_EVT_SEARCH_NOTIFICATION_LIST:
		evt.SetInt(NNBSS_CONTENTPAGE_CURRENTCONTENT_NOTIFICATIONS);
		break;
	default:
		break;
	};
	evt.Skip();
}
