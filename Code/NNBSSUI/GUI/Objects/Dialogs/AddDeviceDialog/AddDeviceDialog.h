#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

class CNNBSSControlButton;
class CNNBSSControlStaticBox;

class CNNBSSAddDeviceDialog : public wxDialog
{
public:
	CNNBSSAddDeviceDialog() {};
	~CNNBSSAddDeviceDialog() {};

	void _Create(wxWindow* parent = NULL, wxWindowID id = wxID_ANY,
		const wxString& title = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCLIP_CHILDREN,
		const wxString& name = wxT("DefaultAddDeviceDialog"))

	{
		Create(parent, id, title, pos, size, style, name);

		_SetupInterface();

		Center();
	};

	void _SetupInterface();

	void _SetupDesign();
private:
	void _BindEvents(bool bind = true);

	bool CheckDialogDemandMeet();

	void OnAnyButtonClick(wxMouseEvent& event);
	void OnDeviceTypeSelect(wxCommandEvent& event);

	int GetIDFromEnum(int id);

	void UpdateUSBCameraList(wxString& message);

	wxArrayString m_list_USBCameraList;
	wxArrayString m_list_DeviceTypes;

	enum DeviceTypeList
	{
		DEVICE_TYPE_USBCAM,
		DEVICE_TYPE_IPCAM,
		DEVICE_TYPE_DVR,
		DEVICE_TYPE_NVR,
		DEVICE_TYPE_MICROPHONE
	};

	void AppendDeviceRow();
	void RemoveDeviceRow();
	void DeviceRowAddressEntry(const int itemIndex);

	wxTextValidator _GetDefaultValidator();
	wxTextValidator _GetIPValidator();

	struct DEVICE_ROW
	{
		wxStaticText* p_DeviceNameLabel = nullptr;
		wxStaticText* p_AddressLabel = nullptr;
		wxStaticText* p_DeviceTypeLabel = nullptr;
		wxStaticText* p_DeviceTypeWarningLabel = nullptr;

		CNNBSSControlStaticBox* p_DeviceNameBox = nullptr;
		wxTextCtrl* p_DeviceName = nullptr;

		CNNBSSControlStaticBox* p_AddressBox = nullptr;
		wxTextCtrl* p_Address = nullptr;

		wxComboBox* p_DeviceType = nullptr;
		wxComboBox* p_USBCameraList = nullptr;

		wxSizer* p_HorizontalSizer = nullptr;
	};
	std::vector<DEVICE_ROW*> m_DeviceRow;
	wxGridSizer* DEVICE_ROW_ITEMS_SIZER = nullptr;
	CNNBSSControlStaticBox* DEVICE_ROW_FILTER_BOX = nullptr;
	CNNBSSControlButton* p_DeviceRowAdd = nullptr;
	CNNBSSControlButton* p_DeviceRowRemove = nullptr;
	wxBoxSizer* p_MainSizer = nullptr;

	CNNBSSControlButton* p_AcceptButton = nullptr;
	CNNBSSControlButton* p_DeclineButton = nullptr;
	wxBoxSizer* p_DialogButtonsHorizontalSizer = nullptr;
};

#endif // ! ADDDEVICEDIALOG_H
