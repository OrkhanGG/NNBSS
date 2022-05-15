#ifndef ADDCAMERADIALOG_H
#define ADDCAMERADIALOG_H

class CNNBSSControlButton;

class CNNBSSAddCameraDialog : public wxDialog
{
public:
								CNNBSSAddCameraDialog() {};
								~CNNBSSAddCameraDialog() {};

	void						_Create(wxWindow* parent = NULL, wxWindowID id = wxID_ANY,
		const wxString& title = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxSTATIC_BORDER,
		const wxString& name = wxT("DefaultAddDeviceDialog"))

	{
		Create(parent, id, title, pos, size, style, name);

		_SetupInterface();

		Center();
	};

	void						_SetupInterface();

	void						_SetupDesign();

private:
	void						_BindEvents(const bool& bind = true);

	bool						CheckDialogDemandMeet();

	void						OnAnyButtonClick(wxMouseEvent& event);

	void						OnDeviceTypeSelect(wxCommandEvent& event);

	int							GetIDFromEnum(const int& id)const;

	enum DeviceTypeList
	{
		DEVICE_TYPE_USBCAM = 0,
		DEVICE_TYPE_IPCAM,
		DEVICE_TYPE_DVR,
		DEVICE_TYPE_NVR,
		DEVICE_TYPE_MICROPHONE
	};

	wxArrayString				m_list_CameraList;

	wxSizer*					p_GeneralSizerVertical			= nullptr;
	wxSizer*					p_GeneralSizerHorizontal		= nullptr;
	wxSizer*					p_ElementsVerticalSizer			= nullptr;
	wxSizer*					p_DialogButtonsHorizontalSizer  = nullptr;

	CNNBSSControlButton*		p_AcceptButton					= nullptr;
	CNNBSSControlButton*		p_DeclineButton					= nullptr;

	wxComboBox*					p_DeviceType					= nullptr;
};

#endif // ! ADDCAMERADIALOG_H
