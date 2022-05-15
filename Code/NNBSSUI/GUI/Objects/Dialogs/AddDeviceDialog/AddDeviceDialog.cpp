// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
// wxWidgets
#include <wx/dialog.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/scrolwin.h>
#include <wx/valtext.h>
#endif
#include <wx/richtooltip.h>

#include "AddDeviceDialog.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS UI
#include <NNBSSUI/GUI/Objects/Controls/Button/Button.h>
#include <NNBSSUI/GUI/Objects/Controls/DraggableHeader/DraggableHeader.h>
#include <NNBSSUI/GUI/Objects/Controls/StaticBox/StaticBox.h>

#define NNBSS_EVT_ADD_DEVICE_DIALOG_APPEND_DEVICE 3450
#define NNBSS_EVT_ADD_DEVICE_DIALOG_REMOVE_DEVICE 3451

inline constexpr int m_MaxDeviceRowCount = 10;

void CNNBSSAddDeviceDialog::_SetupInterface()
{
	// List of devices will be shown in p_DeviceType
	if (m_list_DeviceTypes.empty())
	{
		for (int c = 0; c < NNBSS_EVT_DEVICETYPE_COUNT; c++)
		{
			m_list_DeviceTypes.Add(DeviceList::GetTypeFromID(GetIDFromEnum(c)));
		}
	}

	CNNBSSControlDraggableHeader* p_DraggableHeader = new CNNBSSControlDraggableHeader(this, wxID_ANY, _("Add devices")
		, wxDefaultPosition, wxSize(1, 32));
	p_DraggableHeader->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	p_MainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* p_FilterBoxSizer = new wxBoxSizer(wxVERTICAL);

	// add/remove/edit filters
	DEVICE_ROW_FILTER_BOX = new CNNBSSControlStaticBox(this, wxID_ANY, _("Devices"));
	{
		DEVICE_ROW_ITEMS_SIZER = new wxGridSizer(0, 1, 5, 5); // filter list

		p_FilterBoxSizer->Add(0, 17);
		p_FilterBoxSizer->Add(DEVICE_ROW_ITEMS_SIZER, 1, wxALL | wxEXPAND, 10);
		p_FilterBoxSizer->Add(0, 3);
		DEVICE_ROW_FILTER_BOX->SetAutoLayout(true);
		DEVICE_ROW_FILTER_BOX->SetSizer(p_FilterBoxSizer);

		DEVICE_ROW_FILTER_BOX->SetForegroundColour(RGB_COLOURDEF_WHITE);
		DEVICE_ROW_FILTER_BOX->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	}

	wxPanel* p_SubButtonBackground = new wxPanel(this, wxID_ANY);
	p_SubButtonBackground->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	wxBoxSizer* p_SubButtonBackgroundSizer = new wxBoxSizer(wxVERTICAL);

	p_DeviceRowAdd = new CNNBSSControlButton(p_SubButtonBackground, NNBSS_EVT_ADD_DEVICE_DIALOG_APPEND_DEVICE, _("Append"), wxDefaultPosition, wxSize(64,32), wxNO_BORDER);
	p_DeviceRowRemove = new CNNBSSControlButton(p_SubButtonBackground, NNBSS_EVT_ADD_DEVICE_DIALOG_REMOVE_DEVICE, _("Remove"), wxDefaultPosition, wxSize(64, 32), wxNO_BORDER);
	wxBoxSizer* p_DeviceRowButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	p_DeviceRowButtonSizer->Add(p_DeviceRowAdd);
	p_DeviceRowButtonSizer->Add(p_DeviceRowRemove);

	p_AcceptButton = new CNNBSSControlButton(p_SubButtonBackground, NNBSS_EVT_OK, _("Add"), wxDefaultPosition, wxSize(64, 32), wxNO_BORDER);
	p_DeclineButton = new CNNBSSControlButton(p_SubButtonBackground, NNBSS_EVT_CANCEL, _("Cancel"), wxDefaultPosition, wxSize(64, 32), wxNO_BORDER);
	wxBoxSizer* p_DialogButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
	p_DialogButtonsSizer->Add(p_AcceptButton);
	p_DialogButtonsSizer->Add(p_DeclineButton);

	p_SubButtonBackgroundSizer->AddSpacer(FromDIP(5));
	p_SubButtonBackgroundSizer->Add(p_DeviceRowButtonSizer, wxSizerFlags().Left());
	p_SubButtonBackgroundSizer->AddSpacer(FromDIP(10));
	p_SubButtonBackgroundSizer->Add(p_DialogButtonsSizer, wxSizerFlags().CenterHorizontal());
	p_SubButtonBackground->SetSizer(p_SubButtonBackgroundSizer);

	// main sizer
	p_MainSizer->Add(p_DraggableHeader, wxSizerFlags().Expand());
	p_MainSizer->AddSpacer(FromDIP(1));
	p_MainSizer->Add(DEVICE_ROW_FILTER_BOX, 0, wxEXPAND, 10);
	p_MainSizer->Add(p_SubButtonBackground, wxSizerFlags().Expand());
	SetAutoLayout(true);
	SetSizerAndFit(p_MainSizer);

	_SetupDesign();

	//// Setup event binds
	_BindEvents();

	AppendDeviceRow();

	// Set focus on:
	p_AcceptButton->SetFocus();
}

void CNNBSSAddDeviceDialog::_SetupDesign()
{
	//-----------------------------------------------------------------------------
	SetBackgroundColour(RGB_COLOURDEF_PLACEHOLDER);

	p_DeviceRowAdd->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_DeviceRowAdd->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_DeviceRowAdd->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_IDLE));
	p_DeviceRowAdd->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
	p_DeviceRowAdd->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_DISABLED),CNNBSSControlButton::ButtonStates::BS_DISABLED);

	p_DeviceRowRemove->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_DeviceRowRemove->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_DeviceRowRemove->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_IDLE));
	p_DeviceRowRemove->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);

	p_AcceptButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_AcceptButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_AcceptButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_IDLE));
	p_AcceptButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
	p_AcceptButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_DISABLED), CNNBSSControlButton::ButtonStates::BS_DISABLED);

	p_DeclineButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_DeclineButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_DeclineButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_IDLE));
	p_DeclineButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);

	Refresh();
	Update();

	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSAddDeviceDialog::_BindEvents(bool bind)
{
	if (bind)
	{
		p_DeviceRowAdd->Bind(wxEVT_LEFT_UP, &CNNBSSAddDeviceDialog::OnAnyButtonClick, this);

		p_DeviceRowRemove->Bind(wxEVT_LEFT_UP, &CNNBSSAddDeviceDialog::OnAnyButtonClick, this);

		p_AcceptButton->Bind(wxEVT_LEFT_UP, &CNNBSSAddDeviceDialog::OnAnyButtonClick, this);

		p_DeclineButton->Bind(wxEVT_LEFT_UP, &CNNBSSAddDeviceDialog::OnAnyButtonClick, this);

		//p_DeviceType->Bind(wxEVT_COMBOBOX, &CNNBSSAddDeviceDialog::OnDeviceTypeSelect, this);
	}
	else
	{
		p_DeviceRowAdd->Unbind(wxEVT_LEFT_UP, &CNNBSSAddDeviceDialog::OnAnyButtonClick, this);
		p_DeviceRowRemove->Unbind(wxEVT_LEFT_UP, &CNNBSSAddDeviceDialog::OnAnyButtonClick, this);

		p_AcceptButton->Unbind(wxEVT_LEFT_UP, &CNNBSSAddDeviceDialog::OnAnyButtonClick, this);
		p_DeclineButton->Unbind(wxEVT_LEFT_UP, &CNNBSSAddDeviceDialog::OnAnyButtonClick, this);
	}
}

bool CNNBSSAddDeviceDialog::CheckDialogDemandMeet()
{
	// Return value
	// If false, device cannot be added, demands are not met
	bool _return = true;

	auto HaveDuplicate = [&]()
	{
		int counter = 0;
		const int maxCheck = DEVICE_ROW_ITEMS_SIZER->GetChildren().GetCount();// dialog have add/remove device.
						   //add actually adds elements but remove does not and must not remove.
			              // so we have to add maxCheck & counter to prevent to check invisible elements(user assumes that he removed 
						  // but they'll be destroyed once dialog is finished)

		// check whether dialog contains copy-name/copy-address[instances are not allowed!]
		std::vector<std::string>m_Names, m_Adress; std::vector<int>m_USBs;
		for (const auto& p : m_DeviceRow)
		{

			if (!p->p_DeviceName->GetValue().IsEmpty())m_Names.emplace_back(p->p_DeviceName->GetValue().ToStdString());

			if (p->p_HorizontalSizer->GetItem(p->p_Address))
			{
				if (!p->p_Address->GetValue().IsEmpty())
					m_Adress.emplace_back(p->p_Address->GetValue().ToStdString());
			}
			else if (p->p_HorizontalSizer->GetItem(p->p_USBCameraList))
			{
				m_USBs.emplace_back(p->p_USBCameraList->GetSelection());
			}

			//--------
			counter++;
			if (counter >= maxCheck)
				break;
		}

		// if has no element, there's no duplicate
		if (m_Names.empty() && m_Adress.empty() && m_USBs.empty())
			return false;

		auto it = std::adjacent_find(m_Names.begin(), m_Names.end());
		if (it == m_Names.end())
		{
			// no duplicate	
		}
		else {/* duplicate*/NNBSSMessageBoxSimple(_("Error"), _("Devices cannot be added more than once! Do not use the same names!"), wxICON_ERROR); return true; }

		auto it2 = std::adjacent_find(m_Adress.begin(), m_Adress.end());
		if (it2 == m_Adress.end())
		{
			// no duplicate	
		}
		else {/* duplicate*/NNBSSMessageBoxSimple(_("Error"), _("Devices cannot be added more than once! Do not use the same addresses!"), wxICON_ERROR); return true; }

		auto it3 = std::adjacent_find(m_USBs.begin(), m_USBs.end());
		if (it3 == m_USBs.end())
		{
			// no duplicate	
		}
		else {/* duplicate*/NNBSSMessageBoxSimple(_("Error"), _("You can't add the same USB device more than once!"), wxICON_ERROR); return true; }

		return false;
	};
	if (HaveDuplicate()) return false;

	// Error message. If is not empty, this will be popped up so user may fix the mistake he did
	wxString errorMsg;
	// Which window caused error? If nullptr, tooltip will be shown over the accept button
	wxWindow* errorWindow = nullptr;

	UpdateUSBCameraList(errorMsg);

	//// check if we had any problem while updating usb camera list -- DO WE NEED THIS CHECK?
	//if (!errorMsg.IsEmpty()) { NNBSSMessageBoxSimple(_("Error"), errorMsg, wxICON_ERROR); _return = false; }

	// how many devices we're told to add?
	const int currentElementCount = DEVICE_ROW_ITEMS_SIZER->GetChildren().GetCount();
	for (int currentItemIndex = 0; currentItemIndex < currentElementCount; currentItemIndex++)
	{
		wxString DeviceName = m_DeviceRow[currentItemIndex]->p_DeviceName->GetValue();
		wxString Address = m_DeviceRow[currentItemIndex]->p_Address->GetValue();
		wxString DeviceType = m_DeviceRow[currentItemIndex]->p_DeviceType->GetValue();

		if (m_DeviceRow[currentItemIndex]->p_DeviceType->GetSelection() == DEVICE_TYPE_USBCAM)
		{
			// USB CAMERA ENTERANCE CONDITIONs

			{
				// Empty name
				if (DeviceName.empty())
				{
					m_DeviceRow[currentItemIndex]->p_DeviceName->SetHint(_("Enter a name"));

				        errorMsg = _("Device must have a name.\n"
						"Also, you cannot re-enter the name of the device that already exists!\n");
						errorWindow = m_DeviceRow[currentItemIndex]->p_DeviceName;

					_return = false;
					continue;
				}
			}

			{
				// There's no usb camera or suitable one
				if (m_list_USBCameraList.empty())
				{
					NNBSSMessageBoxSimple(_("Error"), _("No suitable USB Camera device is found!"), wxICON_ERROR);
					_return = false;
					continue;
				}
			}

			std::vector<std::string> _Mounted_USBCameraDevices, _Mounted_USBCameraDevicesPath;
			CNNBSSHardwareAddressHandle()->GetConnectedUSBCameraList(_Mounted_USBCameraDevices,
				_Mounted_USBCameraDevicesPath);

			if (!SCNNBSSMVMParametersList.empty() && !_Mounted_USBCameraDevices.empty())
			{
				wxString addrVal = m_DeviceRow[currentItemIndex]->p_DeviceType->GetSelection() == DEVICE_TYPE_USBCAM ?
					wxString::Format("%i", m_DeviceRow[currentItemIndex]->p_USBCameraList->GetSelection()) : m_DeviceRow[currentItemIndex]->p_Address->GetValue();

				for (const auto& p : SCNNBSSMVMParametersList)
				{
					if (p->m_MVMSourceAddress == addrVal)
					{
						wxString _existingDeviceInfo = wxString::Format(_("Device Name:%s\nDevice Address:%s\n"), p->m_MVMName, p->m_MVMSourceAddress);
						errorMsg = wxString::Format(_("This USB device has already been added or a device is using its address currently.\nPlease, remove the device first with given information:\n%s"), _existingDeviceInfo);
						if (m_DeviceRow[currentItemIndex]->p_DeviceType->GetSelection() == DEVICE_TYPE_USBCAM)
							errorWindow = m_DeviceRow[currentItemIndex]->p_USBCameraList;
						else
							errorWindow = m_DeviceRow[currentItemIndex]->p_Address;

						_return = false;
						continue;
					}
				}
			}

			if (!errorMsg.empty())
			{
				//errorMsg = wxString::Format("%s %s", _("Somethings went wrong, reason:"), errorMsg);
				_return = false;
				continue;
			}

		}
		else
		{
			// Other device check demand condition will be here.(By other devices I mean types like IP, NVR, DVR and etc...)

			// If Device Name or Address is empty
			if (DeviceName.empty() || Address.empty())
			{
				if (DeviceName.empty())
				{
					m_DeviceRow[currentItemIndex]->p_DeviceName->SetHint(_("Enter a name"));

					errorMsg = _("Device must have a name.\n"
						"Also, you cannot re-enter the name of the device that already exists!\n");
					errorWindow = m_DeviceRow[currentItemIndex]->p_DeviceName;
				}
				if (Address.empty())
				{
					m_DeviceRow[currentItemIndex]->p_Address->SetHint(_("Enter an address"));

					errorMsg = _("Device must have an address.\n"
						"Also, you cannot re-enter an address of the device that already exists!\n");
					errorWindow = m_DeviceRow[currentItemIndex]->p_Address;
				}
				_return = false;
				continue;
			}

			// If Address Length is less than what we expect and contains special characters that are not allowed:
			if ((int)Address.Length() < 9)
			{
				errorMsg = _("Address must be at least 9 characters long!");
				errorWindow = m_DeviceRow[currentItemIndex]->p_Address;

				_return = false;
				continue;
			}
		}

		// If the entry contains something that's not allowed such as if the same device address exist already
		for (const auto& p : SCNNBSSMVMParametersList)
		{
			wxString _currentDeviceInfo = wxString::Format(_("Device Name:%s\nDevice Address:%s\n"), p->m_MVMName, p->m_MVMSourceAddress);
			if (Address == p->m_MVMSourceAddress && DeviceName == p->m_MVMName)
			{
				errorMsg = wxString::Format(_("This Device Name and Address is in use by another device! Please, change the input values or remove the following device:\n%s"), _currentDeviceInfo);
				errorWindow = m_DeviceRow[currentItemIndex]->p_DeviceName;

				return false;
				continue;
			}
			if (Address == p->m_MVMSourceAddress)
			{
				errorMsg = wxString::Format(_("This Address is in use by another device! Please, change the input Address or remove the following device:\n%s"), _currentDeviceInfo);
				errorWindow = m_DeviceRow[currentItemIndex]->p_Address;

				_return = false;
				continue;
			}
			if (DeviceName == p->m_MVMName)
			{
				errorMsg = wxString::Format(_("This Device Name is in use by another device! Please, change the input Device Name or remove the following device:\n%s"), _currentDeviceInfo);
				errorWindow = m_DeviceRow[currentItemIndex]->p_DeviceName;

				_return = false;
				continue;
			}
		}
	}

	// if error message is not empty, show it then 
	if (!errorMsg.IsEmpty()) 
	{ 
		//NNBSSMessageBoxSimple(_("Error"), errorMsg, wxICON_ERROR); _return = false; 
		wxRichToolTip tip("Wait!",errorMsg);
		tip.SetIcon(wxICON_ERROR);
		tip.ShowFor(errorWindow ? errorWindow : p_AcceptButton);
	}

	return _return;
}

void CNNBSSAddDeviceDialog::OnAnyButtonClick(wxMouseEvent& event)
{
	CNNBSSMouseEventsAddressHandle()->OnAnyButtonClick(event);

	bool destroy = false;

	// Local process
	switch (event.GetId())
	{
	case NNBSS_EVT_ADD_DEVICE_DIALOG_APPEND_DEVICE:
	{
		AppendDeviceRow();
	}
	break;
	case NNBSS_EVT_ADD_DEVICE_DIALOG_REMOVE_DEVICE:
	{
		RemoveDeviceRow();
	}
	break;
	case NNBSS_EVT_OK:
		if (CheckDialogDemandMeet())
		{
			EndModal(NNBSS_EVT_OK);
			destroy = true;

			const int currentElementCount = DEVICE_ROW_ITEMS_SIZER->GetChildren().GetCount();

			for (int currentIndex = 0; currentIndex < currentElementCount; currentIndex++)
			{
				// Address entry may be a bit confusing because it has some conditions such as:
				// If 'USB devices' is the selection of Device Type, we should get a combobox value otherwise textctrl value
				wxString GetAddressEntry = m_DeviceRow[currentIndex]->p_DeviceType->GetSelection() == DEVICE_TYPE_USBCAM
					/*If USB Cameras get Combobox value*/ ? wxString::Format("%i", m_DeviceRow[currentIndex]->p_USBCameraList->GetSelection())
					/*If IP Cameras get TextCtrl value */ : m_DeviceRow[currentIndex]->p_Address->GetValue();

				// USB Special: DeviceNetName, DevicePath
				std::string DevNetName, DevicePath;
				if (m_DeviceRow[currentIndex]->p_DeviceType->GetSelection() == DEVICE_TYPE_USBCAM)
				{
					// by doing these, we save the values(that are special for usb temporarily) so we may not let
					// the user to add the same device for more than once
					if (m_DeviceRow[currentIndex]->p_DeviceType->GetSelection() == DEVICE_TYPE_USBCAM)
					{
						std::vector<std::string> _Mounted_USBCameraDevices, _Mounted_USBCameraDevicesPath;
						CNNBSSHardwareAddressHandle()->GetConnectedUSBCameraList(_Mounted_USBCameraDevices,
							_Mounted_USBCameraDevicesPath);
						for (int c = 0; c < (int)_Mounted_USBCameraDevicesPath.size(); c++)
						{
							if (m_DeviceRow[currentIndex]->p_USBCameraList->GetSelection() == c)
							{
								DevNetName = _Mounted_USBCameraDevices[c];
								DevicePath = _Mounted_USBCameraDevicesPath[c];
							}
						}
					}
				}

				// Add device
				// This pointer will be added into a list, then will be removed when either user removes device or application gets closed
				SNNBSSMVMData* _params = new SNNBSSMVMData;
				_params->m_MVMName = m_DeviceRow[currentIndex]->p_DeviceName->GetValue();
				_params->m_MVMSourceAddress = GetAddressEntry;
				_params->m_MVMType = DeviceList::GetIDFromType(m_DeviceRow[currentIndex]->p_DeviceType->GetValue());
				_params->m_DeviceNetName = DevNetName;
				_params->m_DevicePath = DevicePath;

				wxQueueEvent(CNNBSSUXAddressHandle(), new CNNBSSEventsDeviceList(EVT_DEVICELIST_ON_ADD_REQUEST, _params));
			}
		}
		break;
	case NNBSS_EVT_CANCEL:
		EndModal(NNBSS_EVT_CANCEL);
		destroy = true;
		break;
	default:
		break;
	}

	event.Skip();

	if (destroy)
	{
		if (this->GetChildren().GetCount() > 0)
		{
			for (wxWindowList::iterator it = this->GetChildren().begin();
				it != this->GetChildren().end(); it++) {

				wxWindow* Child = dynamic_cast<wxWindow*>(*it);

				if (Child) {
					Child->Hide();
				}

			}
		}
		// Then Destroy dialog
		this->Destroy();
		// To use this dialog again(Otherwise you'll get crash):
		CNNBSSAddDeviceDialogAddressHandle(true);
	}
}

void CNNBSSAddDeviceDialog::OnDeviceTypeSelect(wxCommandEvent& event)
{
	wxString errmsg;
	UpdateUSBCameraList(errmsg);
	for (int c = 0; c < (int)m_DeviceRow.size(); c++)
	{
		DeviceRowAddressEntry(c);
	}
	event.Skip();
	if (!errmsg.empty()) { NNBSSMessageBoxSimple(_("Error"), errmsg, wxICON_ERROR); }
}

int CNNBSSAddDeviceDialog::GetIDFromEnum(int id)
{
	// we won't declare macros below in .h file. So, that's why we use this function.
	// as for why should we use macros below, it's because they're global constants that events happen according to  
	switch (id)
	{
	case DEVICE_TYPE_USBCAM:
		id = NNBSS_MVM_TYPE_USBCAM;
		break;
	case DEVICE_TYPE_IPCAM:
		id = NNBSS_MVM_TYPE_IPCAM;
		break;
	case DEVICE_TYPE_DVR:
		id = NNBSS_MVM_TYPE_DVR;
		break;
	case DEVICE_TYPE_NVR:
		id = NNBSS_MVM_TYPE_NVR;
		break;
	case DEVICE_TYPE_MICROPHONE:
		id = NNBSS_MVM_TYPE_MICROPHONE;
		break;
	default:
		NNBSSErrorShow("System Critical Error! Found incorret while getting device ID from AddDevice dialog!", 100);
		break;
	}

	return id;
}

void CNNBSSAddDeviceDialog::UpdateUSBCameraList(wxString& message)
{
	// Update USB Camera list updates USB Camera list combobox
	// When this function is called, we get the current mounted usb camera list and check
	// whether it matches the old list or not.
	// if matches, nothing happens, message parameter stays empty.
	// otherwise message parameter gets the value of error message

	auto UpdateUSBCameraListComboBox = [&]()
	{
		const int currentElementCount = DEVICE_ROW_ITEMS_SIZER->GetChildren().GetCount();

		// Update all elements
		for (int currentIndex = 0; currentIndex < currentElementCount; currentIndex++)
		{
			int oldSelection = 0;
			if (!m_DeviceRow[currentIndex]->p_USBCameraList->GetStringSelection().IsEmpty())
				oldSelection = m_DeviceRow[currentIndex]->p_USBCameraList->GetSelection();

			m_DeviceRow[currentIndex]->p_USBCameraList->Clear();
			m_DeviceRow[currentIndex]->p_USBCameraList->Set(m_list_USBCameraList);

			m_list_USBCameraList.empty() ? 0 : m_DeviceRow[currentIndex]->p_USBCameraList->Select(oldSelection);

			DeviceRowAddressEntry(currentIndex);
		}

		Refresh();
		Update();
	};

	// Get the list that contains all USB Cameras mounted to hardware
	std::vector<std::string> _Mounted_USBCameraDevices, _Mounted_USBCameraDevicesPath;
	CNNBSSHardwareAddressHandle()->GetConnectedUSBCameraList(_Mounted_USBCameraDevices,
		_Mounted_USBCameraDevicesPath);

	if (m_list_USBCameraList.size() < _Mounted_USBCameraDevices.size())
	{
		message = _("An USB Camera device's mounted while AddDevice dialog.");
	}
	else if (m_list_USBCameraList.size() > _Mounted_USBCameraDevices.size())
	{
		message = _("An USB device is unmounted while adding an USB Camera device!");
	}

	// if m_list_USBCameraList is empty and we have mounted devices to hardware, 
	// we just make m_list_USBCameraList equal to current connected usb list by copying its elements one-by-one
	wxArrayString tempBuffer;
	for (const auto& p : _Mounted_USBCameraDevices)tempBuffer.Add(p);
	if (m_list_USBCameraList != tempBuffer)
	{
		if (message.empty())message = _("An USB device is unmounted/mounted while adding an USB Camera device!");
		m_list_USBCameraList.clear();
		for (const auto& p : _Mounted_USBCameraDevices) m_list_USBCameraList.Add(wxString::Format("%s", p));

	}

	UpdateUSBCameraListComboBox();
}

wxTextValidator CNNBSSAddDeviceDialog::_GetDefaultValidator()
{
	wxTextValidator validator(wxFILTER_INCLUDE_CHAR_LIST);
	wxArrayString list;
	wxString valid_chars(wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-"));
	size_t len = valid_chars.Length();
	for (size_t i = 0; i < len; i++)
		list.Add(wxString(valid_chars.GetChar(i)));
	validator.SetIncludes(list);

	return validator;
}

wxTextValidator CNNBSSAddDeviceDialog::_GetIPValidator()
{
	wxTextValidator validator(wxFILTER_INCLUDE_CHAR_LIST);
	wxArrayString list;
	wxString valid_chars(wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-:/@?."));
	size_t len = valid_chars.Length();
	for (size_t i = 0; i < len; i++)
		list.Add(wxString(valid_chars.GetChar(i)));
	validator.SetIncludes(list);

	return validator;
}

void CNNBSSAddDeviceDialog::AppendDeviceRow()
{
	auto create = [&](const int c)
	{
		m_DeviceRow.emplace_back(new DEVICE_ROW);

		m_DeviceRow[c]->p_DeviceNameLabel = new wxStaticText(DEVICE_ROW_FILTER_BOX, wxID_ANY, _("Name:"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
		m_DeviceRow[c]->p_DeviceNameLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);

		m_DeviceRow[c]->p_AddressLabel = new wxStaticText(DEVICE_ROW_FILTER_BOX, wxID_ANY, _("Address:"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
		m_DeviceRow[c]->p_AddressLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);

		m_DeviceRow[c]->p_DeviceTypeLabel = new wxStaticText(DEVICE_ROW_FILTER_BOX, wxID_ANY, _("Type:"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
		m_DeviceRow[c]->p_DeviceTypeLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);

		m_DeviceRow[c]->p_DeviceNameBox = new CNNBSSControlStaticBox(DEVICE_ROW_FILTER_BOX, wxID_ANY, wxEmptyString);
		m_DeviceRow[c]->p_DeviceNameBox->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_DeviceRow[c]->p_DeviceNameBox->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
		m_DeviceRow[c]->p_DeviceName = new wxTextCtrl(m_DeviceRow[c]->p_DeviceNameBox, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER,
			_GetDefaultValidator());
		m_DeviceRow[c]->p_DeviceName->SetHint(_("Device name"));
		m_DeviceRow[c]->p_DeviceName->SetMaxLength(20);
		m_DeviceRow[c]->p_DeviceName->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_DeviceRow[c]->p_DeviceName->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		wxBoxSizer* p_DeviceNameBoxSizer = new wxBoxSizer(wxVERTICAL);
		p_DeviceNameBoxSizer->Add(m_DeviceRow[c]->p_DeviceName, wxSizerFlags().Proportion(1).Expand().Border());
		m_DeviceRow[c]->p_DeviceNameBox->SetSizer(p_DeviceNameBoxSizer);

		m_DeviceRow[c]->p_AddressBox = new CNNBSSControlStaticBox(DEVICE_ROW_FILTER_BOX, wxID_ANY, wxEmptyString);
		m_DeviceRow[c]->p_AddressBox->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_DeviceRow[c]->p_AddressBox->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
		m_DeviceRow[c]->p_Address = new wxTextCtrl(m_DeviceRow[c]->p_AddressBox, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_PROCESS_ENTER,
			_GetIPValidator());
		m_DeviceRow[c]->p_Address->SetMaxLength(120);
		m_DeviceRow[c]->p_Address->SetHint(_("Address"));
		m_DeviceRow[c]->p_Address->SetToolTip(wxT("protocol://username:password@IP:port/video?x.mjpeg"));
		m_DeviceRow[c]->p_Address->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_DeviceRow[c]->p_Address->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_DeviceRow[c]->p_Address->SetSize(m_DeviceRow[c]->p_Address->GetSize() + wxSize(100, 0));
		wxBoxSizer* p_AddressBoxSizer = new wxBoxSizer(wxVERTICAL);
		p_AddressBoxSizer->Add(m_DeviceRow[c]->p_Address, wxSizerFlags().Proportion(1).Expand().Border());
		m_DeviceRow[c]->p_AddressBox->SetSizer(p_AddressBoxSizer);

		m_DeviceRow[c]->p_DeviceType = new wxComboBox(DEVICE_ROW_FILTER_BOX, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize, m_list_DeviceTypes, wxNO_BORDER);
		m_DeviceRow[c]->p_DeviceType->Bind(wxEVT_COMBOBOX, &CNNBSSAddDeviceDialog::OnDeviceTypeSelect, this);
		m_DeviceRow[c]->p_DeviceType->SetEditable(false);
		m_DeviceRow[c]->p_DeviceType->Select(0);
		m_DeviceRow[c]->p_DeviceType->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_DeviceRow[c]->p_DeviceType->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

		// usb cam special
		m_DeviceRow[c]->p_DeviceTypeWarningLabel = new wxStaticText(DEVICE_ROW_FILTER_BOX, wxID_ANY, _("No USB Camera!"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
		m_DeviceRow[c]->p_DeviceTypeWarningLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_DeviceRow[c]->p_DeviceTypeWarningLabel->Hide();

		m_DeviceRow[c]->p_USBCameraList = new wxComboBox(DEVICE_ROW_FILTER_BOX, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize, wxArrayString(), wxNO_BORDER);
		m_DeviceRow[c]->p_USBCameraList->SetEditable(false);
		m_DeviceRow[c]->p_USBCameraList->SetForegroundColour(RGB_COLOURDEF_WHITE);
		m_DeviceRow[c]->p_USBCameraList->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		m_DeviceRow[c]->p_USBCameraList->Hide();
		//

		m_DeviceRow[c]->p_HorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
		m_DeviceRow[c]->p_HorizontalSizer->AddSpacer(FromDIP(10));
		m_DeviceRow[c]->p_HorizontalSizer->Add(m_DeviceRow[c]->p_DeviceNameLabel, wxSizerFlags().CenterVertical());
		m_DeviceRow[c]->p_HorizontalSizer->AddSpacer(FromDIP(5));
		m_DeviceRow[c]->p_HorizontalSizer->Add(m_DeviceRow[c]->p_DeviceNameBox);
		m_DeviceRow[c]->p_HorizontalSizer->AddSpacer(FromDIP(5));
		m_DeviceRow[c]->p_HorizontalSizer->Add(m_DeviceRow[c]->p_DeviceTypeLabel, wxSizerFlags().CenterVertical());
		m_DeviceRow[c]->p_HorizontalSizer->AddSpacer(FromDIP(5));
		m_DeviceRow[c]->p_HorizontalSizer->Add(m_DeviceRow[c]->p_DeviceType);
		m_DeviceRow[c]->p_HorizontalSizer->AddSpacer(FromDIP(5));
		m_DeviceRow[c]->p_HorizontalSizer->Add(m_DeviceRow[c]->p_AddressLabel, wxSizerFlags().CenterVertical());
		m_DeviceRow[c]->p_HorizontalSizer->AddSpacer(FromDIP(5));
		m_DeviceRow[c]->p_HorizontalSizer->Add(m_DeviceRow[c]->p_AddressBox, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
		m_DeviceRow[c]->p_HorizontalSizer->AddSpacer(FromDIP(10));

		DEVICE_ROW_ITEMS_SIZER->Add(m_DeviceRow[c]->p_HorizontalSizer, 0, wxALL | wxEXPAND, 0);
	};
	auto show = [&](const int index)
	{
		m_DeviceRow[index]->p_DeviceNameLabel->Show();
		m_DeviceRow[index]->p_DeviceNameBox->Show();
		m_DeviceRow[index]->p_AddressLabel->Show();
		m_DeviceRow[index]->p_DeviceTypeLabel->Show();
		m_DeviceRow[index]->p_DeviceType->Show();
		// DeviceRowAddressEntry will decide which of them will be shown and hidden:
		m_DeviceRow[index]->p_AddressBox->Show();
		m_DeviceRow[index]->p_DeviceTypeWarningLabel->Show();
		m_DeviceRow[index]->p_USBCameraList->Show();

		DEVICE_ROW_ITEMS_SIZER->Add(m_DeviceRow[index]->p_HorizontalSizer, 0, wxALL | wxEXPAND, 0);
	};
	const int currentElementCount = DEVICE_ROW_ITEMS_SIZER->GetChildren().GetCount();
	if (currentElementCount >= 10)return;
	const int ElementCount = m_DeviceRow.size();
	if (ElementCount <= currentElementCount)
	{
		create(currentElementCount);
	}
	else
	{
		show(currentElementCount);
	}
	UpdateUSBCameraList(wxString());
	DeviceRowAddressEntry(currentElementCount);

	// Update dialog window
	p_MainSizer->Layout();
	Fit();
	Center();
	// Move mouse automatically for the sake of UX
	wxPoint moveCursor = p_DeviceRowAdd->GetScreenPosition() + p_DeviceRowAdd->GetSize() / 2;
	SetCursorPos(moveCursor.x, moveCursor.y);
}

void CNNBSSAddDeviceDialog::RemoveDeviceRow()
{
	int c = DEVICE_ROW_ITEMS_SIZER->GetChildren().GetCount() - 1;
	if (c <= 0)return;
	DEVICE_ROW_ITEMS_SIZER->Detach(c);
	{
		m_DeviceRow[c]->p_DeviceNameLabel->Hide();
		m_DeviceRow[c]->p_DeviceNameBox->Hide();
		m_DeviceRow[c]->p_AddressLabel->Hide();
		m_DeviceRow[c]->p_AddressBox->Hide();
		m_DeviceRow[c]->p_DeviceTypeLabel->Hide();
		m_DeviceRow[c]->p_DeviceType->Hide();
		m_DeviceRow[c]->p_DeviceTypeWarningLabel->Hide();
		m_DeviceRow[c]->p_USBCameraList->Hide();
	}
	// Update dialog window
	p_MainSizer->Layout();
	Fit();
	Center();
	// Move mouse automatically for the sake of UX
	wxPoint moveCursor = p_DeviceRowRemove->GetScreenPosition() + p_DeviceRowRemove->GetSize() / 2;
	SetCursorPos(moveCursor.x, moveCursor.y);
}

void CNNBSSAddDeviceDialog::DeviceRowAddressEntry(const int itemIndex)
{
	DEVICE_ROW* RowItem = m_DeviceRow[itemIndex];

	// Simple logic to check which of them(USB LIST/USB ERROR LABEL OR ADDRESS ENTRY) is currently visible
	// We'll replace the sizer item according to currentVisible result
	enum { _NULL_ = 0, USBLISTCOMBOBOX, IPADDRESSENTRY, NOUSBLABEL };
	int currentVisible = _NULL_;
	{
		if (RowItem->p_HorizontalSizer->GetItem(RowItem->p_AddressBox)) { currentVisible = IPADDRESSENTRY; }
		else RowItem->p_AddressBox->Hide();
		if (RowItem->p_HorizontalSizer->GetItem(RowItem->p_USBCameraList)) { currentVisible = USBLISTCOMBOBOX; }
		else RowItem->p_USBCameraList->Hide();
		if (RowItem->p_HorizontalSizer->GetItem(RowItem->p_DeviceTypeWarningLabel)) { currentVisible = NOUSBLABEL; }
		else RowItem->p_DeviceTypeWarningLabel->Hide();
	}
	wxWindow* oldWin = nullptr;
	if (currentVisible == USBLISTCOMBOBOX)
		oldWin = RowItem->p_USBCameraList;
	else if (currentVisible == IPADDRESSENTRY)
		oldWin = RowItem->p_AddressBox;
	else if (currentVisible == NOUSBLABEL)
		oldWin = RowItem->p_DeviceTypeWarningLabel;

	if (!oldWin)
	{
		NNBSSMessageBoxSimple(_("Error"), _("Somethings went wrong. Error: Bad/no cast oldWin is nullptr"), wxICON_ERROR);
		return;
	}
	wxWindow* newWin = nullptr;

	if (RowItem->p_DeviceType->GetSelection() == DEVICE_TYPE_USBCAM/*Display USB Camera List*/)
	{
		std::vector<std::string> _Mounted_USBCameraDevices, _Mounted_USBCameraDevicesPath;
		CNNBSSHardwareAddressHandle()->GetConnectedUSBCameraList(_Mounted_USBCameraDevices,
			_Mounted_USBCameraDevicesPath);
		// Check whether any USB Camera is mounted to hardware or not
		if (_Mounted_USBCameraDevices.empty())
		{
			newWin = RowItem->p_DeviceTypeWarningLabel;
		}
		else
		{
			newWin = RowItem->p_USBCameraList;
		}
		// Update Label
		RowItem->p_AddressLabel->SetLabelText(_("USB Cameras:"));
	}
	else/*IP Camera Address Entry*/
	{
		newWin = RowItem->p_AddressBox;

		// Update Label
		RowItem->p_AddressLabel->SetLabelText(_("Address:"));
	}
	if (!newWin) {
		NNBSSMessageBoxSimple(_("Error"), _("Somethings went wrong. Error: Bad/no cast newWin is nullptr"), wxICON_ERROR);
		return;
	}

	if (oldWin == newWin)
		return;

	newWin->Show();
	RowItem->p_HorizontalSizer->Replace(oldWin, newWin);
	oldWin->Hide();

	// Update dialog window
	p_MainSizer->Layout();
	Fit();
	Center();
}
