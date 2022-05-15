///////////////////////////////////////////////////////////////////////////////
// Name:        Cameragrid.cpp
// Purpose:     Displays thumbnails from multiple cameras
// Author:      Orkhan Aliyev
// Created:     2021-12-07
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#include "cameragrid.h"
// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// wxWidgets
#include <wx/arrstr.h>
#include <wx/busyinfo.h>
#include <wx/dir.h>
#include <wx/dnd.h>
#include <wx/richmsgdlg.h>
#include <wx/utils.h>
#include <wx/wupdlock.h>
// OpenCV
#include <opencv2/videoio.hpp>

CNNBSSCameraGrid::CNNBSSCameraGrid(wxWindow* parent)
	: CNNBSSGridViewPanelBase(parent)
{
}

CNNBSSCameraGrid::~CNNBSSCameraGrid()
{
	for (const auto& p : m_Thumbnails)
	{
		for (const auto& t : SCNNBSSMVMParametersList)
		{
			if (p.second.m_DeviceName == t->m_MVMName)
			{
				ResetPanel(t);
			}
		}
	}
}

void CNNBSSCameraGrid::LinkPanel(SNNBSSMVMData* params, const  int& rowNumber)
{
	// Check whether thread is available
	CNNBSSMovingVisualMediaThread* p_Thread = GetThumbnailThread(params);
	if (!p_Thread)
	{
		NNBSSMessageBoxSimple(_("Error"), _("Camera could not be added! Error Code: GV0001 invalid thread!"), wxICON_ERROR);
		return;
	}


	SStandardThumbnail candidate = m_Thumbnails[rowNumber];

	candidate.m_DeviceName = params->m_MVMName;
	candidate.m_DeviceAddress = params->m_MVMSourceAddress;
	candidate.m_IsEmpty = false;

	candidate.m_ThumbnailPanel = m_Thumbnails[rowNumber].m_ThumbnailPanel;
	// Place holder text that will be shown when the panel has no -ok-bitmap
	candidate.m_ThumbnailPanel->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_PLACEHOLDERTEXT, _("Connecting... This won't take long..."));

	// Set State to Recieve that means working without a problem
	candidate.m_ThumbnailPanel->SetProperty<long>(CNNBSSBitmapPanel::PROPERTIES::BPP_STATE, CNNBSSBitmapPanel::STATES::BPS_RECIEVES);
	// Set Panel Name
	candidate.m_ThumbnailPanel->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_NAME, params->m_MVMName);
	// Attach Panel to Thread to display the frames
	candidate.m_ThumbnailPanel->Attach(params);

	// and our candidate deserved to be appointed
	m_Thumbnails[rowNumber] = candidate;

	// Rebind events
	_BindEvents(false);
	_BindEvents(true);

	// Update UI
	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSCameraGrid::ResetPanel(SNNBSSMVMData* params)
{
	for (auto& element : m_Thumbnails) {
		if (element.second.m_DeviceName == params->m_MVMName)
		{
			// switch from fullscreen to gridview mode
			if (_IsFullScreen())
			{
				FullscreenThumbnail(element.second.m_ThumbnailPanel, false);
			}

			// Panel now is empty
			element.second.m_ThumbnailPanel->SetProperty<long>(CNNBSSBitmapPanel::PROPERTIES::BPP_STATE, CNNBSSBitmapPanel::STATES::BPS_EMPTY);
			element.second.m_IsCreated = true;
			element.second.m_IsEmpty = true;
			element.second.m_ThumbnailPanel->Detach(params);
			element.second.m_DeviceName = wxString();
			element.second.m_ThumbnailPanel->SetBitmap(wxBitmap());
			//delete p.second.properties;
			element.second.m_DeviceAddress = wxString();
			// Set placeholder text for empty thumbnail panel
			element.second.m_ThumbnailPanel->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_PLACEHOLDERTEXT, DefaultThumbnailPlaceholder);
			element.second.m_ThumbnailPanel->Refresh();

			break;
		}
	}

	_BindEvents(false);
	_BindEvents(true);

	Layout();
	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSCameraGrid::OnAnyThumbnailDoubleClick(wxMouseEvent& evt)
{
	CNNBSSBitmapPanel* cast = dynamic_cast<CNNBSSBitmapPanel*>(evt.GetEventObject());

	bool empty = true;
	for (const auto& p : m_Thumbnails)
	{
		if (p.second.m_ThumbnailPanel == cast && !p.second.m_IsEmpty)
			empty = false;
	}

	if (empty)
	{
		if (SCNNBSSMVMParametersList.size() < 1)
		{
			NNBSSErrorShow("You have no device to add!", 100);
			evt.Skip();
			return;
		}
		else
		{
			bool _continue = false;
			for (const auto& p : SCNNBSSMVMParametersList)
				if (
					(p->m_MVMType != NNBSS_MVM_TYPE_VIDEO
					&& p->m_MVMType != NNBSS_MVM_TYPE_MICROPHONE)

					&& (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE
						|| p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_LOW_CONNECTION))
				{
					_continue = true;
					break;
				}
			if (!_continue)
			{
				NNBSSErrorShow("You have no valid device to add!", 100);
				evt.Skip();
				return;
			}
		}

		int rowNumber = -1;

		for (auto& p : m_Thumbnails)
		{
			if (p.second.m_ThumbnailPanel == cast)
			{
				rowNumber = p.first;// Get rowNumber

				static int called = 0;
				called++;

				CNNBSSAddCameraDialogAddressHandle()->_Create(GetActiveUXFrame());
				int DialogAnswer = CNNBSSAddCameraDialogAddressHandle()->ShowModal();

				if (DialogAnswer != NNBSS_EVT_CANCEL)
				{
					int numberOfCopies = 0;
					for (const auto& p : m_Thumbnails)
					{
						if (p.second.m_DeviceAddress == SCNNBSSMVMParametersList[DialogAnswer]->m_MVMSourceAddress)
						{
							NNBSSErrorShow("Selected device is already in use!", 100);
							evt.Skip();
							return;
						}

					}

					LinkPanel(SCNNBSSMVMParametersList[DialogAnswer], rowNumber);
				}
				break;
			}
		}

		CNNBSSAuiManagerAddressHandle()->Update();

		if (rowNumber == -1)
		{
			NNBSSErrorShow("Panel could not be selected", 100);
			evt.Skip();
			return;
		}
	}
	else// not empty
	{
		FullscreenThumbnail(cast, !_IsFullScreen());
	}

	evt.Skip();
}

void CNNBSSCameraGrid::OnAnyThumbnailClick(wxMouseEvent& evt)
{
	evt.Skip();
}

void CNNBSSCameraGrid::OnDropCameraOnThumbnail(wxDropFilesEvent& event)
{
	if (event.GetNumberOfFiles() > 0) {

		wxString* dropped = event.GetFiles();
		wxASSERT(dropped);

		wxString name;
		wxArrayString files;

		for (int i = 0; i < event.GetNumberOfFiles(); i++) {
			name = dropped[i];
			files.push_back(name);
		}

		wxString _FirstSelection = files[0];

		CNNBSSBitmapPanel* p_Target = dynamic_cast<CNNBSSBitmapPanel*>(event.GetEventObject());
		int rowNumber = -1;
		for (const auto& p : m_Thumbnails)
		{
			if (p.second.m_ThumbnailPanel == p_Target)
			{
				rowNumber = p.first;
			}

			if (p.second.m_DeviceName == _FirstSelection)
			{
				wxString msg = _("Selected device is in use!");

				NNBSSErrorShow("%s", 100, msg.ToStdString().c_str());
				event.Skip();
				return;
			}
		}

		if (rowNumber < 0)
		{
			NNBSSErrorShow("Device has some problems that NNBSS can't detect!", 100);

			event.Skip();
			return;
		}

		bool _found = false;
		for (const auto& p : SCNNBSSMVMParametersList)
		{
			if (p->m_MVMName == _FirstSelection && p->m_MVMType != NNBSS_MVM_TYPE_MICROPHONE
				&& (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE
					|| p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_LOW_CONNECTION))
			{
				LinkPanel(p, rowNumber);
				_found = true;
				break;
			}
		}
		if (!_found)
		{
			NNBSSErrorShow("The device is not online!", 100);
		}
	}

	CNNBSSAuiManagerAddressHandle()->Update();

	event.Skip();
}

void CNNBSSCameraGrid::OnContextMenu(wxContextMenuEvent& evt)
{
	CNNBSSBitmapPanel* m_bitmapPanel = dynamic_cast<CNNBSSBitmapPanel*>(evt.GetEventObject());

	wxCHECK_RET(m_bitmapPanel, "CameraGridFrame::OnCameraContextMenu() invalid thumbnail panel");

	int RowNumber = wxID_ANY;
	for (const auto& p : m_Thumbnails)
	{
		if (p.second.m_ThumbnailPanel == m_bitmapPanel)
		{
			RowNumber = p.first;
		}
	}

	auto  it = m_Thumbnails.find(RowNumber);

	if (!GetThumbnailThread(m_bitmapPanel) || !GetThumbnailThread(m_bitmapPanel)->IsCapturing())
	{
		evt.Skip();
		return;
	}

	m_ContextMenu.p_DisplaySubMenu = new wxMenu();
	m_ContextMenu.HorizontalFlipMenuItem = new wxMenuItem(m_ContextMenu.p_DisplaySubMenu, CMEID_HORIZONTAL_FLIP, _("Horizontal Flip"), _("Flip thumbnail horizontally"), wxITEM_CHECK);
	m_ContextMenu.p_DisplaySubMenu->AppendCheckItem(CMEID_HORIZONTAL_FLIP, _("Horizontal Flip"));
	m_ContextMenu.HorizontalFlipMenuItem->Check(GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipHorizontal);
	m_ContextMenu.VerticalFlipMenuItem = new wxMenuItem(m_ContextMenu.p_DisplaySubMenu, CMEID_VERTICAL_FLIP, _("Vertical Flip"), _("Flip thumbnail vertically"), wxITEM_CHECK);
	m_ContextMenu.p_DisplaySubMenu->AppendCheckItem(CMEID_VERTICAL_FLIP, _("Vertical Flip"));
	m_ContextMenu.VerticalFlipMenuItem->Check(GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipVertical);
	m_ContextMenu.HistogramToggle = new wxMenuItem(m_ContextMenu.p_DisplaySubMenu, CMEID_DISPLAY_HISTOGRAM, _("Display Histogram"), _("Display Histogram"), wxITEM_CHECK);
	m_ContextMenu.p_DisplaySubMenu->AppendCheckItem(CMEID_DISPLAY_HISTOGRAM, _("Display Histogram"));
	m_ContextMenu.HistogramToggle->Check(GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingShowHistogram);

	m_ContextMenu.p_ContextMenu = new wxMenu();

	m_ContextMenu.StandaloneFrameMenuItem = new wxMenuItem(m_ContextMenu.p_ContextMenu, CMEID_STANDALONEFRAME_SHOW, _("Show Miniplayer"), _("Shows this panel apart main frame"));

	m_ContextMenu.FullscreenMenuItem = new wxMenuItem(m_ContextMenu.p_ContextMenu, CMEID_FULLSCREEN, _("Fullscreen"), _("Switch to fullscreen"), wxITEM_CHECK);

	m_ContextMenu.RemoveMenuItem = new wxMenuItem(m_ContextMenu.p_ContextMenu, CMEID_REMOVE, _("Remove"), _("Remove this camera from this grid view panel."));

	// TODO: Destroy wxMenu & wxMenuItems at the end?

	// Append---------------------
	m_ContextMenu.p_ContextMenu->Append(m_ContextMenu.StandaloneFrameMenuItem);
	m_ContextMenu.p_ContextMenu->AppendCheckItem(CMEID_FULLSCREEN, _("Fullscreen"));
	m_ContextMenu.FullscreenMenuItem->Check(_IsFullScreen() && GetSelectedThumbnail() == m_bitmapPanel);
	m_ContextMenu.p_ContextMenu->AppendSubMenu(m_ContextMenu.p_DisplaySubMenu, _("Display"));
	m_ContextMenu.p_ContextMenu->Append(m_ContextMenu.RemoveMenuItem);
	//----------------------------

	SNNBSSMVMData* params = nullptr;
	for (const auto& p : SCNNBSSMVMParametersList)
		if (p->m_MVMName == it->second.m_DeviceName && p->m_MVMSourceAddress == it->second.m_DeviceAddress)
			params = p;

	wxCHECK_RET(params, "Error code: CG0004");

	int    id = m_bitmapPanel->GetPopupMenuSelectionFromUser(*m_ContextMenu.p_ContextMenu);

	switch (id)
	{
		break;
	case CMEID_STANDALONEFRAME_SHOW:
		ShowStandaloneFrame(m_bitmapPanel);
		break;
	case CMEID_FULLSCREEN:
		FullscreenThumbnail(m_bitmapPanel, !_IsFullScreen());
		break;
	case CMEID_REMOVE:
		ResetPanel(params);
		break;
	case CMEID_HORIZONTAL_FLIP:
		GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipHorizontal = !GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipHorizontal;
		break;
	case CMEID_VERTICAL_FLIP:
		GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipVertical = !GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipVertical;
		break;
	case CMEID_DISPLAY_HISTOGRAM:
		GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingShowHistogram = !GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingShowHistogram;
		break;
	default:
		break;
	}

	evt.Skip();
}

void CNNBSSCameraGrid::OnDeviceListEvent(CNNBSSEventsDeviceList& evt)
{
	if (evt.GetEventType() == EVT_DEVICELIST_ON_REMOVE)
	{
		ResetPanel(evt.GetMVMParams());
	}
	evt.Skip();
}

void CNNBSSCameraGrid::OnDeviceAddAttempt(MovingVisualMediaEvent& evt)
{
	evt.Skip();
}

void CNNBSSCameraGrid::OnDeviceConnectionFail(MovingVisualMediaEvent& evt)
{
	evt.Skip();
}

void CNNBSSCameraGrid::OnDeviceConnectionLevelChange(MovingVisualMediaEvent& evt)
{
	if (evt.GetEventType() == EVT_MVM_DEVICE_LOW_CONNECTION)
	{
		FindThumbnailPanel(evt.GetMVMParams())->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_RIGHT_TOP_TEXT, _("Connection is satisfactory"));
	}
	else if (evt.GetEventType() == EVT_MVM_DEVICE_HIGH_CONNECTION)
	{
		FindThumbnailPanel(evt.GetMVMParams())->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_RIGHT_TOP_TEXT, _("Low connection"));
	}
	else
	{
		FindThumbnailPanel(evt.GetMVMParams())->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_RIGHT_TOP_TEXT, wxEmptyString);
	}
	evt.Skip();
}

void CNNBSSCameraGrid::OnDeviceConnected(MovingVisualMediaEvent& evt)
{
	evt.Skip();
}

void CNNBSSCameraGrid::OnDeviceDisconnected(MovingVisualMediaEvent& evt)
{
	// If device is disconnected, consider resetting the thumbnail
	evt.Skip();
}

void CNNBSSCameraGrid::OnCameraCaptureStarted(MovingVisualMediaEvent& evt)
{
	evt.Skip();
}

void CNNBSSCameraGrid::OnCameraCommandResult(MovingVisualMediaEvent& evt)
{
}

void CNNBSSCameraGrid::OnCameraErrorOpen(MovingVisualMediaEvent& evt)
{
}

void CNNBSSCameraGrid::OnCameraErrorEmpty(MovingVisualMediaEvent& evt)
{
}

void CNNBSSCameraGrid::OnCameraErrorException(MovingVisualMediaEvent& evt)
{
}