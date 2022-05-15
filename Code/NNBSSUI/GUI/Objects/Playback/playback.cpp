///////////////////////////////////////////////////////////////////////////////
// Name:        Playback.cpp
// Purpose:     Playback Widget; Derived from CNNBSSGridViewPanelBase 
//              Helps user to play recorded videos with handy functionalities
// Author:      Orkhan Aliyev
// Created:     2022-02-13
// Copyright:   (c) 2022 Orkhan Aliyev
//////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif
// wxWidgets
#include <wx/utils.h>
#include <wx/wupdlock.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>

// Main
#include "playback.h"
// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS UI
#include <NNBSSUI/GUI/Objects/Controls/Slider/Slider.h>
#include <NNBSSUI/GUI/Objects/Controls/Button/Button.h>
// Boost
#include <boost/algorithm/string.hpp>
// OpenCV
#include <opencv2/videoio.hpp>

// Special Definitions
#define NNBSS_EVT_PLAYBACK_PANEL_PLAY 17000
#define NNBSS_EVT_PLAYBACK_PANEL_PAUSE 17001
#define NNBSS_EVT_PLAYBACK_PANEL_STOP 17002
#define NNBSS_EVT_PLAYBACK_PANEL_PREVIOUS 17003
#define NNBSS_EVT_PLAYBACK_PANEL_NEXT 17004
#define NNBSS_EVT_PLAYBACK_PANEL_FORWARD 17005
#define NNBSS_EVT_PLAYBACK_PANEL_BACKWARD 17006

CNNBSSPlaybackPanel::CNNBSSPlaybackPanel(wxWindow* parent)
	: CNNBSSGridViewPanelBase(parent)
{

	// Create Slider
	p_VideoSlider = new CNNBSSControlSlider(this, wxID_ANY, CNNBSSControlSlider::SliderType::MONO, 0, 0, 0, 1000);
	p_VideoSlider->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_VideoSlider->ShowValueLabel(false);
	p_VideoSlider->ShowThumb(false);
	wxBoxSizer* videoSliderSizer = new wxBoxSizer(wxHORIZONTAL);
	videoSliderSizer->AddSpacer(FromDIP(10));
	videoSliderSizer->Add(p_VideoSlider, wxSizerFlags().Proportion(1).Expand());
	videoSliderSizer->AddSpacer(FromDIP(10));

	wxPanel* m_VideoControlMenuBar = new wxPanel(this, wxID_ANY);
	m_VideoPreviousFrameButton = new CNNBSSControlButton(m_VideoControlMenuBar, NNBSS_EVT_PLAYBACK_PANEL_PREVIOUS, wxEmptyString,
		wxDefaultPosition, wxSize(50, 50), wxNO_BORDER);
	m_VideoPreviousFrameButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_PLAYBACK_PREVIOUS_IDLE));
	m_VideoPreviousFrameButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_PLAYBACK_PREVIOUS_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);

	m_VideoPlayButton = new CNNBSSControlButton(m_VideoControlMenuBar, NNBSS_EVT_PLAYBACK_PANEL_PLAY, wxEmptyString,
		wxDefaultPosition, wxSize(50, 50), wxNO_BORDER);
	m_VideoPlayButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_PLAYBACK_PLAY_IDLE));
	m_VideoPlayButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_PLAYBACK_PLAY_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);

	m_VideoPauseButton = new CNNBSSControlButton(m_VideoControlMenuBar, NNBSS_EVT_PLAYBACK_PANEL_PAUSE, wxEmptyString,
		wxDefaultPosition, wxSize(50, 50), wxNO_BORDER);
	m_VideoPauseButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_PLAYBACK_PAUSE_IDLE));
	m_VideoPauseButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_PLAYBACK_PAUSE_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);

	m_VideoStopButton = new CNNBSSControlButton(m_VideoControlMenuBar, NNBSS_EVT_PLAYBACK_PANEL_STOP, wxEmptyString,
		wxDefaultPosition, wxSize(50, 50), wxNO_BORDER);
	m_VideoStopButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_PLAYBACK_STOP_IDLE));
	m_VideoStopButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_PLAYBACK_STOP_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);

	m_VideoNextFrameButton = new CNNBSSControlButton(m_VideoControlMenuBar, NNBSS_EVT_PLAYBACK_PANEL_NEXT, wxEmptyString,
		wxDefaultPosition, wxSize(50, 50), wxNO_BORDER);
	m_VideoNextFrameButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_PLAYBACK_NEXT_IDLE));
	m_VideoNextFrameButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_PLAYBACK_NEXT_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);

	// SetDefaultStyle
	const int _roundness_ = 20;
	m_VideoPlayButton->SetButtonBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	m_VideoPlayButton->SetButtonRoundness(_roundness_, CNNBSSControlButton::ButtonStates::BS_HOVER);
	m_VideoPauseButton->SetButtonBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	m_VideoPauseButton->SetButtonRoundness(_roundness_, CNNBSSControlButton::ButtonStates::BS_HOVER);
	m_VideoStopButton->SetButtonBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	m_VideoStopButton->SetButtonRoundness(_roundness_, CNNBSSControlButton::ButtonStates::BS_HOVER);
	m_VideoPreviousFrameButton->SetButtonBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	m_VideoPreviousFrameButton->SetButtonRoundness(_roundness_, CNNBSSControlButton::ButtonStates::BS_HOVER);
	m_VideoNextFrameButton->SetButtonBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	m_VideoNextFrameButton->SetButtonRoundness(_roundness_, CNNBSSControlButton::ButtonStates::BS_HOVER);

	wxSizer* p_VideoControlMenuBarSizer = new wxBoxSizer(wxHORIZONTAL);
	p_VideoControlMenuBarSizer->AddStretchSpacer();
	p_VideoControlMenuBarSizer->Add(m_VideoPreviousFrameButton, wxSizerFlags().Proportion(0));
	p_VideoControlMenuBarSizer->Add(m_VideoPlayButton, wxSizerFlags().Proportion(0));
	p_VideoControlMenuBarSizer->Add(m_VideoPauseButton, wxSizerFlags().Proportion(0));
	p_VideoControlMenuBarSizer->Add(m_VideoStopButton, wxSizerFlags().Proportion(0));
	p_VideoControlMenuBarSizer->Add(m_VideoNextFrameButton, wxSizerFlags().Proportion(0));
	p_VideoControlMenuBarSizer->AddStretchSpacer();
	m_VideoControlMenuBar->SetSizerAndFit(p_VideoControlMenuBarSizer);

	GetSizer()->AddSpacer(FromDIP(10));
	GetSizer()->Add(videoSliderSizer, wxSizerFlags().Proportion(0).Expand());
	GetSizer()->AddSpacer(FromDIP(10));
	GetSizer()->Add(m_VideoControlMenuBar, wxSizerFlags().Proportion(0).Expand());
	GetSizer()->AddSpacer(FromDIP(10));

	ChangeGridSize(wxSize(_GetGridWidth(), _GetGridHeight()));

	m_SliderUpdateTimer.Bind(wxEVT_TIMER, &CNNBSSPlaybackPanel::OnTimelineUpdate, this);
	m_SliderUpdateTimer.Start(250); // 1/4 second(Update Slider)
}

CNNBSSPlaybackPanel::~CNNBSSPlaybackPanel()
{
	m_SliderUpdateTimer.Stop();
}

void CNNBSSPlaybackPanel::AddVideo(SNNBSSMVMData* params, const  int& rowNumber)
{
	// Check whether thread is available
	CNNBSSMovingVisualMediaThread* p_Thread = GetThumbnailThread(params);
	if (!p_Thread)
	{
		NNBSSMessageBoxSimple(_("Error"), _("Camera could not be added! Error Code: GV0001 invalid thread!"), wxICON_ERROR);
		return;
	}

	params->m_MVMName = wxString::Format("%s_playback", params->m_MVMName);

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

	// and our candidate deserved to be appointed
	m_Thumbnails[rowNumber] = candidate;

	// Rebind events
	_BindEvents(false);
	_BindEvents(true);

	// Update UI
	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSPlaybackPanel::RemoveVideo(SNNBSSMVMData* params)
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
			element.second.m_DeviceName = wxString();
			element.second.m_ThumbnailPanel->Detach(params);
			element.second.m_ThumbnailPanel->SetBitmap(wxBitmap());
			//delete p.second.properties;
			element.second.m_DeviceAddress = wxString();
			// Set placeholder text for empty thumbnail panel
			element.second.m_ThumbnailPanel->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_PLACEHOLDERTEXT, DefaultThumbnailPlaceholder);

			// Refresh panel
			element.second.m_ThumbnailPanel->Refresh();

			break;
		}
	}

	Layout();
	CNNBSSAuiManagerAddressHandle()->Update();

	_BindEvents(false);
	_BindEvents(true);
}

void CNNBSSPlaybackPanel::_BindEvents(bool bind)
{
	if (bind)
	{
		// playback specials
		m_VideoPlayButton->Bind(wxEVT_LEFT_UP, &CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial, this);
		m_VideoPauseButton->Bind(wxEVT_LEFT_UP, &CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial, this);
		m_VideoStopButton->Bind(wxEVT_LEFT_UP, &CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial, this);
		m_VideoPreviousFrameButton->Bind(wxEVT_LEFT_UP, &CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial, this);
		m_VideoNextFrameButton->Bind(wxEVT_LEFT_UP, &CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial, this);
		// Slider
		p_VideoSlider->Bind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSPlaybackPanel::OnTimelineChange, this);
	}
	else
	{
		// playback specials
		m_VideoPlayButton->Unbind(wxEVT_LEFT_UP, &CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial, this);
		m_VideoPauseButton->Unbind(wxEVT_LEFT_UP, &CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial, this);
		m_VideoStopButton->Unbind(wxEVT_LEFT_UP, &CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial, this);
		m_VideoPreviousFrameButton->Unbind(wxEVT_LEFT_UP, &CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial, this);
		m_VideoNextFrameButton->Unbind(wxEVT_LEFT_UP, &CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial, this);
		// Slider
		p_VideoSlider->Unbind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSPlaybackPanel::OnTimelineChange, this);
	}

	CNNBSSGridViewPanelBase::_BindEvents(bind);
}

void CNNBSSPlaybackPanel::OnContextMenu(wxContextMenuEvent& evt)
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
	m_ContextMenu.VerticalFlipMenuItem = new wxMenuItem(m_ContextMenu.p_DisplaySubMenu, CMEID_VERTICAL_FLIP, _("Vertical Flip"), _("Flip thumbnail vertically"), wxITEM_CHECK);
	m_ContextMenu.p_DisplaySubMenu->AppendCheckItem(CMEID_HORIZONTAL_FLIP, _("Horizontal Flip"));
	m_ContextMenu.HorizontalFlipMenuItem->Check(GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipHorizontal);
	m_ContextMenu.p_DisplaySubMenu->AppendCheckItem(CMEID_VERTICAL_FLIP, _("Vertical Flip"));
	m_ContextMenu.VerticalFlipMenuItem->Check(GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipVertical);

	m_ContextMenu.p_ContextMenu = new wxMenu();

	m_ContextMenu.FullscreenMenuItem = new wxMenuItem(m_ContextMenu.p_ContextMenu, CMEID_FULLSCREEN, _("Fullscreen"), _("Switch to fullscreen"), wxITEM_CHECK);

	m_ContextMenu.RemoveMenuItem = new wxMenuItem(m_ContextMenu.p_ContextMenu, CMEID_REMOVE, _("Remove"), _("Remove this camera from this grid view panel."));

	m_ContextMenu.PropertiesMenuItem = new wxMenuItem(m_ContextMenu.p_ContextMenu, CMEID_PROPERTIES, _("Properties"), _("All possible properties."));
	// TODO: Destroy wxMenu & wxMenuItems at the end?

	// Append---------------------
	m_ContextMenu.p_ContextMenu->AppendCheckItem(CMEID_FULLSCREEN, _("Fullscreen"));
	m_ContextMenu.FullscreenMenuItem->Check(_IsFullScreen() && GetSelectedThumbnail() == m_bitmapPanel);
	m_ContextMenu.p_ContextMenu->AppendSubMenu(m_ContextMenu.p_DisplaySubMenu, _("Flip"));
	m_ContextMenu.p_ContextMenu->Append(m_ContextMenu.RemoveMenuItem);
	m_ContextMenu.p_ContextMenu->Append(m_ContextMenu.PropertiesMenuItem);
	//----------------------------

	SNNBSSMVMData* params = nullptr;
	for (const auto& p : SCNNBSSMVMParametersList)
		if (p->m_MVMName == it->second.m_DeviceName && p->m_MVMSourceAddress == it->second.m_DeviceAddress)
			params = p;

	wxCHECK_RET(params, "Error code: CG0004");

	int    id = m_bitmapPanel->GetPopupMenuSelectionFromUser(*m_ContextMenu.p_ContextMenu);

	switch (id)
	{
	case CMEID_FULLSCREEN:
		FullscreenThumbnail(m_bitmapPanel, !_IsFullScreen());
		break;
	case CMEID_REMOVE:
		RemoveVideo(params);
		break;
	case CMEID_HORIZONTAL_FLIP:
		GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipHorizontal = !GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipHorizontal;
		break;
	case CMEID_VERTICAL_FLIP:
		GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipVertical = !GetThumbnailMVMParameters(m_bitmapPanel)->m_ImageProcessingFlipVertical;
		break;
	case CMEID_PROPERTIES:
		CNNBSSDevicePropertiesDialogAddressHandle(params, GetActiveUXFrame());
		CNNBSSDevicePropertiesDialogAddressHandle(params, GetActiveUXFrame())->ShowModal();
		break;
	default:
		break;
	}

	evt.Skip();
}

void CNNBSSPlaybackPanel::OnDropCameraOnThumbnail(wxDropFilesEvent& event)
{
	// TODO:
	// Playback panel accepts drops and gets all device records. Then displays list of the videos with asking which one you'd like to open?
#if 0
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
			if (p.second.thumbnailPanel == p_Target)
			{
				rowNumber = p.first;
			}

			if (p.second.cameraName == _FirstSelection)
			{
				wxString msg = _("Selected device is in use!");

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
		for (const auto& p : SCNNBSSMVMParametersList)
		{
			if (p.DeviceName == _FirstSelection && p.DeviceType != NNBSS_MVM_TYPE_MICROPHONE)
			{
				AddCamera(p.Address, p.DeviceName, rowNumber);
				break;
			}
		}
	}

	CNNBSSAuiManagerAddressHandle()->Update();
#endif
	event.Skip();
}

void CNNBSSPlaybackPanel::OnAnyThumbnailDoubleClick(wxMouseEvent& evt)
{
	CNNBSSBitmapPanel* _thumbPanel = dynamic_cast<CNNBSSBitmapPanel*>(evt.GetEventObject());

	bool empty = true;
	for (const auto& p : m_Thumbnails)
	{
		if (p.second.m_ThumbnailPanel == _thumbPanel && !p.second.m_IsEmpty)
			empty = false;
	}

	if (!empty)
	{
		FullscreenThumbnail(_thumbPanel, !_IsFullScreen());

		evt.Skip();
	}
	else
	{
		short RowNumber = -1;

		for (auto& p : m_Thumbnails)
		{
			if (p.second.m_ThumbnailPanel == _thumbPanel)
			{
				RowNumber = p.first;// Get rowNumber

				wxFileDialog openFileDialog(this, _("Open Video File"),
					NNBSS_PROJECT_DIRECTORY + NNBSS_DIR__PLAYBACK_DIR,// DefaultDirectory
					""/*Default File*/, "Video files (*.avi;*.mp4)|*.avi;*.mp4",// Only XMLs are allowed
					wxFD_OPEN | wxFD_FILE_MUST_EXIST);// Flags

				if (openFileDialog.ShowModal() == wxID_CANCEL)
				{
					evt.Skip();
					return; // if user changed idea
				}

				wxFileInputStream input_stream(openFileDialog.GetPath());
				if (!input_stream.IsOk())
				{
					evt.Skip();
					NNBSSErrorShow("Cannot open file!", 100);
					return;
				}
				else
				{
					// Check file size
					if (NNBSSFileSystem::NNBSS_GETFILESIZE(std::string(openFileDialog.GetPath().mb_str())) < 1)
					{
						evt.Skip();
						NNBSSErrorShow("The file is empty!", 100);
						return;
					}

					SNNBSSMVMData* p_MVMParams = new SNNBSSMVMData();
					p_MVMParams->m_MVMSourceAddress = openFileDialog.GetPath();
					p_MVMParams->m_MVMName = openFileDialog.GetFilename();
					p_MVMParams->m_MVMType = NNBSS_MVM_TYPE_VIDEO;
					DeviceList::CNNBSSDeviceListAddDevice(p_MVMParams);

					AddVideo(p_MVMParams, RowNumber);
				}
				break;
			}
		}

		CNNBSSAuiManagerAddressHandle()->Update();

		if (RowNumber == -1)
		{
			NNBSSErrorShow("Panel could not be selected", 100);
			evt.Skip();
			return;
		}

		evt.Skip();
	}
}
void CNNBSSPlaybackPanel::OnAnyThumbnailClick(wxMouseEvent& evt)
{
	evt.Skip();
}

void CNNBSSPlaybackPanel::OnAnyButtonLeftUPSpecial(wxMouseEvent& evt)
{
	switch (evt.GetId())
	{
	case NNBSS_EVT_PLAYBACK_PANEL_PLAY:
	{
		if (!p_VideoSlider)
		{
			evt.Skip();  return;
		}

		if (GetThumbnailContainer(GetSelectedThumbnail()).m_IsEmpty
			|| !GetSelectedThumbnail()
			|| !GetThumbnailThread(GetSelectedThumbnail()))
		{
			p_VideoSlider->Reset();
			evt.Skip();  return;
		}

		GetThumbnailThread(GetSelectedThumbnail())->_Play();
	}
	break;
	case NNBSS_EVT_PLAYBACK_PANEL_PAUSE:
	{
		if (!p_VideoSlider)
		{
			evt.Skip();  return;
		}

		if (GetThumbnailContainer(GetSelectedThumbnail()).m_IsEmpty
			|| !GetSelectedThumbnail()
			|| !GetThumbnailThread(GetSelectedThumbnail()))
		{
			p_VideoSlider->Reset();
			evt.Skip();  return;
		}

		GetThumbnailThread(GetSelectedThumbnail())->_PauseAt(GetThumbnailThread(GetSelectedThumbnail())->_GetCurrentFrame());
	}
	break;
	case NNBSS_EVT_PLAYBACK_PANEL_STOP:
	{
		if (!p_VideoSlider)
		{
			evt.Skip();  return;
		}

		if (GetThumbnailContainer(GetSelectedThumbnail()).m_IsEmpty
			|| !GetSelectedThumbnail()
			|| !GetThumbnailThread(GetSelectedThumbnail()))
		{
			p_VideoSlider->Reset();
			evt.Skip();  return;
		}

		GetThumbnailThread(GetSelectedThumbnail())->_Stop();
	}
	break;
	case NNBSS_EVT_PLAYBACK_PANEL_PREVIOUS:
	{
		if (!p_VideoSlider)
		{
			evt.Skip();  return;
		}

		if (GetThumbnailContainer(GetSelectedThumbnail()).m_IsEmpty
			|| !GetSelectedThumbnail()
			|| !GetThumbnailThread(GetSelectedThumbnail()))
		{
			p_VideoSlider->Reset();
			evt.Skip();  return;
		}

		GetThumbnailThread(GetSelectedThumbnail())->_PreviousFrame();
	}
	break;
	case NNBSS_EVT_PLAYBACK_PANEL_NEXT:
	{
		if (!p_VideoSlider)
		{
			evt.Skip();  return;
		}
		if (GetThumbnailContainer(GetSelectedThumbnail()).m_IsEmpty
			|| !GetSelectedThumbnail()
			|| !GetThumbnailThread(GetSelectedThumbnail()))
		{
			p_VideoSlider->Reset();
			evt.Skip();  return;
		}

		GetThumbnailThread(GetSelectedThumbnail())->_NextFrame();
	}
		break;
	default:
		break;
	}

	// Update Video Slider
	p_VideoSlider->SetValue(GetThumbnailThread(GetSelectedThumbnail())->_GetCurrentFrame(), CNNBSSControlSlider::ThumbType::HIGH);

	CNNBSSMouseEventsAddressHandle()->OnAnyButtonClick(evt);

	evt.Skip();
}

void CNNBSSPlaybackPanel::OnTimelineUpdate(wxTimerEvent&)
{
	if (!p_VideoSlider)
		return;

	if (GetThumbnailContainer(GetSelectedThumbnail()).m_IsEmpty
		|| !GetSelectedThumbnail()
		|| !GetThumbnailThread(GetSelectedThumbnail()))
	{
		p_VideoSlider->Reset();
		return;
	}

	if (p_VideoSlider->GetMaxValue() != GetThumbnailThread(GetSelectedThumbnail())->_GetFrameCount())
		p_VideoSlider->SetMax(GetThumbnailThread(GetSelectedThumbnail())->_GetFrameCount());

	p_VideoSlider->SetValue(GetThumbnailThread(GetSelectedThumbnail())->_GetCurrentFrame(), CNNBSSControlSlider::ThumbType::HIGH);
}

void CNNBSSPlaybackPanel::OnTimelineChange(CNNBSSEventsControlSlider& evt)
{
	if (!p_VideoSlider)
	{
		evt.Skip();  return;
	}

	if (GetThumbnailContainer(GetSelectedThumbnail()).m_IsEmpty
		|| !GetSelectedThumbnail()
		|| !GetThumbnailThread(GetSelectedThumbnail()))
	{
		p_VideoSlider->Reset();
		evt.Skip();  return;
	}

	unsigned long long value = evt.GetSliderValue();

	if (evt.GetSliderType() == CNNBSSControlSlider::SliderType::MONO)// This means, slider has only one thumb
	{
		for (auto& p : m_Thumbnails)
		{
			if (p.second.m_ThumbnailPanel == GetSelectedThumbnail()) {
				GetThumbnailThread(GetSelectedThumbnail())->_SetFrameNumber(value);
			}
		}
	}

	GetThumbnailThread(GetSelectedThumbnail())->_PauseAt(value);

	evt.Skip();
}