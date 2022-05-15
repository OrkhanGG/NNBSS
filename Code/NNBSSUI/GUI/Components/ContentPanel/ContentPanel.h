///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#ifndef CONTENTPANEL_H
#define CONTENTPANEL_H

class CNNBSSControlButton;

class wxSearchCtrl;

class CNNBSSContentPanel : public wxPanel
{
public:
	CNNBSSContentPanel();
	~CNNBSSContentPanel();

	void _Create(wxWindow* parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxT("DefaultNameControlPanel"))
	{
		// TODO: REMOVE _IsCreated variable
		if (_IsCreated)
		{
			if (this)
			{
				if (GetChildren().GetCount() > 0)
					DestroyChildren();
				Destroy();
				_IsCreated = false;
			}
		}
		else 
		{
			Create(parent, GetId(), pos, size, style, name);
			_IsCreated = true;
		}
	}

	CNNBSSContentPanel* GetPanel();

	void _SetupColours(const wxColour& mBackgroundColour);

	// TODO: Remove destroy parameters
	void _ShowServicesContent(bool destroy = false);
	void _ShowDevicesContent(bool destroy = false);
	void DevicePanelUpdateDeviceCount();
	void _ShowGridViewContent(bool destroy = false);
	void _ShowNotificationsContent(bool destroy = false);
	void _ShowPlaybackContent(bool destroy = false);
	void _ShowProfilesContent(bool destroy = false);
	void _ShowDetectionContent(bool destroy = false);

	void SetCurrentContent(const long& _CurCont) { _CurrentContent = _CurCont; };
	long GetCurrentContent() const { return _CurrentContent; };

	void UpdateSearchBar();
private:
	void OnAnyButtonClick(wxMouseEvent& evt);

	void OnSearch(wxCommandEvent& evt);

	// Services Panel Contents
	wxPanel* p_ServicesPanel = nullptr;
	wxSizer* p_ServicesPanelGeneralSizer = nullptr;
	wxGridSizer* ServicesSizer = nullptr;
	wxSizer* p_ServicesContentSizerHorizontal = nullptr;
	CNNBSSControlButton* DevicesButton = nullptr;
	CNNBSSControlButton* GridViewButton = nullptr;
	CNNBSSControlButton* NotificationSystemButton = nullptr;
	CNNBSSControlButton* PlaybackPanelButton = nullptr;
	CNNBSSControlButton* ProfilesPanelButton = nullptr;
	CNNBSSControlButton* DetectionPanelButton = nullptr;
	wxBitmapButton* LOAM_LOGO = nullptr;

	// Devices Panel Contents
	wxPanel* p_DevicesPanel = nullptr;
	wxPanel* p_DevicesPanelManagementGround = nullptr;
	CNNBSSControlButton* p_AddDeviceButton = nullptr;
	CNNBSSControlButton* p_RemoveDeviceButton = nullptr;
	CNNBSSControlButton* p_ImportDevicesButton = nullptr;
	CNNBSSControlButton* p_ExportDevicesButton = nullptr;
	wxStaticText* p_DeviceCount = nullptr;
	wxSizer* p_DevicesPanelGeneralSizer = nullptr;
	wxSearchCtrl* p_DeviceListSearchCtrl = nullptr;
	// Management ground of Devices Panel
	wxSizer* p_DevicesPanelManagementHorizontalSizer = nullptr;

	// Grid View Contents
	wxPanel* p_GridViewPanel = nullptr;
	wxSizer* p_GridViewGeneralSizer = nullptr;

	// Notifications
	wxSearchCtrl* p_NotificationsSearchCtrl = nullptr;
    
	// Content Panel private prim-vars
	bool _IsCreated;
	long _CurrentContent;
};

#endif // ! CONTENTPANEL_H