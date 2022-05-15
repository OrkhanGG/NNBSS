// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

#include "AddCameraDialog.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS UI
#include <NNBSSUI/GUI/Objects/Controls/Button/Button.h>
#include <NNBSSUI/GUI/Objects/Controls/DraggableHeader/DraggableHeader.h>

void CNNBSSAddCameraDialog::_SetupInterface()
{
	// List of devices will be shown in p_DeviceType
	int deviceCount = (int)SCNNBSSMVMParametersList.size();
	if (deviceCount > 0)
	{
		for (int c = 0; c < deviceCount; c++)
		{
			if (SCNNBSSMVMParametersList[c]->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE
				&& SCNNBSSMVMParametersList[c]->m_MVMType != NNBSS_MVM_TYPE_VIDEO
				&& SCNNBSSMVMParametersList[c]->m_MVMType != NNBSS_MVM_TYPE_MICROPHONE)
				m_list_CameraList.Add(SCNNBSSMVMParametersList[c]->m_MVMName);
		}
	}

	p_GeneralSizerVertical = new wxBoxSizer(wxVERTICAL);
	p_GeneralSizerHorizontal = new wxBoxSizer(wxHORIZONTAL);

	p_ElementsVerticalSizer = new wxBoxSizer(wxVERTICAL);
	p_DialogButtonsHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	CNNBSSControlDraggableHeader* p_DraggableHeader = new CNNBSSControlDraggableHeader(this, wxID_ANY, _("Add Camera")
		, wxDefaultPosition, wxSize(1, 32));
	p_DraggableHeader->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	p_DeviceType = new wxComboBox(this, wxID_ANY, m_list_CameraList[0],
		wxDefaultPosition, wxDefaultSize, m_list_CameraList, wxNO_BORDER);
	p_DeviceType->SetEditable(false);
	p_DeviceType->Select(0);

	wxPanel* p_SubButtonsBackground = new wxPanel(this, wxID_ANY);
	p_SubButtonsBackground->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	p_AcceptButton = new CNNBSSControlButton(p_SubButtonsBackground, NNBSS_EVT_OK, _("Add"), wxDefaultPosition, wxSize(96, 32), wxNO_BORDER);
	p_DeclineButton = new CNNBSSControlButton(p_SubButtonsBackground, NNBSS_EVT_CANCEL, _("Cancel"), wxDefaultPosition, wxSize(96, 32), wxNO_BORDER);

	p_DialogButtonsHorizontalSizer->AddStretchSpacer();
	p_DialogButtonsHorizontalSizer->Add(p_AcceptButton, wxSizerFlags().CenterVertical());
	p_DialogButtonsHorizontalSizer->Add(p_DeclineButton, wxSizerFlags().CenterVertical());
	p_DialogButtonsHorizontalSizer->AddStretchSpacer();
	p_SubButtonsBackground->SetSizer(p_DialogButtonsHorizontalSizer);

	p_ElementsVerticalSizer->Add(p_DraggableHeader, wxSizerFlags().Expand());
	p_ElementsVerticalSizer->AddSpacer(FromDIP(1));
	p_ElementsVerticalSizer->Add(p_DeviceType, wxSizerFlags().Expand());
	p_ElementsVerticalSizer->AddSpacer(FromDIP(1));
	p_ElementsVerticalSizer->Add(p_SubButtonsBackground, wxSizerFlags().Expand());

	p_GeneralSizerVertical->Add(p_ElementsVerticalSizer, wxSizerFlags().Proportion(1).CenterHorizontal());
	p_GeneralSizerHorizontal->Add(p_ElementsVerticalSizer, wxSizerFlags().Proportion(1));
	SetSizer(p_GeneralSizerHorizontal);
	SetSize(p_GeneralSizerHorizontal->CalcMin() + wxSize(40, 0));

	// Tooltips
	p_DeviceType->SetToolTip(_("All available cameras will be shown here individually. No other devices expect the cameras is present since GridView only can display cameras."));
	p_AcceptButton->SetToolTip(_("Add camera"));
	p_DeclineButton->SetToolTip(_("Close this dialog"));

	_SetupDesign();

	// Setup event binds
	_BindEvents();

	// Set focus on:
	p_AcceptButton->SetFocus();
}

void CNNBSSAddCameraDialog::_SetupDesign()
{
	this->SetBackgroundColour(RGB_COLOURDEF_PLACEHOLDER);

	p_DeviceType->SetBackgroundColour(RGB_COLOURDEF_TRIPLE30);
	p_DeviceType->SetForegroundColour(RGB_COLOURDEF_WHITE);


	p_AcceptButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_AcceptButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_AcceptButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_IDLE), CNNBSSControlButton::ButtonStates::BS_IDLE);
	p_AcceptButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
	p_AcceptButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_DISABLED), CNNBSSControlButton::ButtonStates::BS_DISABLED);

	p_DeclineButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_DeclineButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_DeclineButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_IDLE), CNNBSSControlButton::ButtonStates::BS_IDLE);
	p_DeclineButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);

	Refresh();
	Update();

	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSAddCameraDialog::_BindEvents(const bool& bind)
{
	if (bind)
	{
		p_AcceptButton->Bind(wxEVT_LEFT_UP, &CNNBSSAddCameraDialog::OnAnyButtonClick, this);
		p_DeclineButton->Bind(wxEVT_LEFT_UP, &CNNBSSAddCameraDialog::OnAnyButtonClick, this);

		p_DeviceType->Bind(wxEVT_COMBOBOX, &CNNBSSAddCameraDialog::OnDeviceTypeSelect, this);
	}
	else
	{
		p_AcceptButton->Unbind(wxEVT_LEFT_UP, &CNNBSSAddCameraDialog::OnAnyButtonClick, this);
		p_DeclineButton->Unbind(wxEVT_LEFT_UP, &CNNBSSAddCameraDialog::OnAnyButtonClick, this);

		p_DeviceType->Unbind(wxEVT_COMBOBOX, &CNNBSSAddCameraDialog::OnDeviceTypeSelect, this);
	}
}

bool CNNBSSAddCameraDialog::CheckDialogDemandMeet()
{
	wxString CameraType = p_DeviceType->GetValue();

	int deviceType = SCNNBSSMVMParametersList[p_DeviceType->GetSelection()]->m_MVMType;

	int convertedDeviceType = GetIDFromEnum(p_DeviceType->GetSelection());
	if (convertedDeviceType != NNBSS_MVM_TYPE_DVR && convertedDeviceType != NNBSS_MVM_TYPE_NVR &&
		convertedDeviceType != NNBSS_MVM_TYPE_IPCAM && convertedDeviceType != NNBSS_MVM_TYPE_USBCAM)
	{
		NNBSSMessageBoxSimple(_("Error"), _("Only devices that sends visual output can be selected such as DVRs, NVRs and USB cameras!"), wxICON_ERROR);
		return false;
	}

	return true;
}

void CNNBSSAddCameraDialog::OnAnyButtonClick(wxMouseEvent& event)
{
	CNNBSSMouseEventsAddressHandle()->OnAnyButtonClick(event);

	bool destroy = false;

	// Local process
	switch (event.GetId())
	{
	case NNBSS_EVT_OK:
		if (CheckDialogDemandMeet())
		{
			EndModal(p_DeviceType->GetSelection());
			destroy = true;
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
				it != this->GetChildren().end(); it++)
			{

				wxWindow* Child = dynamic_cast<wxWindow*>(*it);

				if (Child)
				{
					Child->Hide();
				}

			}
		}
		// Then Destroy dialog
		this->Destroy();
		// To use this dialog again(Otherwise you'll get crash):
		CNNBSSAddCameraDialogAddressHandle(true);
	}
}

void CNNBSSAddCameraDialog::OnDeviceTypeSelect(wxCommandEvent& event)
{
	event.Skip();
}

int CNNBSSAddCameraDialog::GetIDFromEnum(const int& id)const
{
	switch (id)
	{
	case DEVICE_TYPE_USBCAM:
		return NNBSS_MVM_TYPE_USBCAM;
		break;
	case DEVICE_TYPE_IPCAM:
		return NNBSS_MVM_TYPE_IPCAM;
		break;
	case DEVICE_TYPE_DVR:
		return NNBSS_MVM_TYPE_DVR;
		break;
	case DEVICE_TYPE_NVR:
		return NNBSS_MVM_TYPE_NVR;
		break;
	case DEVICE_TYPE_MICROPHONE:
		return NNBSS_MVM_TYPE_MICROPHONE;
		break;
	default:
		NNBSSErrorShow("System Critical Error! Found incorret while getting device ID from AddDevice dialog!", 100);
		break;
	}

	return 0;
}
