///////////////////////////////////////////////////////////////////////////////
// Name:        GridViewPanel.cpp
// Purpose:     Base for all panels that will display multiple cameras with NNBSS standard UX
// Author:      Orkhan Aliyev
// Created:     2021-12-07
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif
#include <wx/richmsgdlg.h>

#include "GridViewPanel.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS UI
#include <NNBSSUI/GUI/Objects/Controls/Button/Button.h>

CNNBSSGridViewPanelBase::CNNBSSGridViewPanelBase(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	p_GridViewGeneralSizer = new wxBoxSizer(wxVERTICAL);

	// GridView GridSizer
	p_GridViewSizer = new wxGridSizer(_GetGridHeight(), _GetGridWidth(), 0, 0);

	// Grid View Sub Menu Start
	p_GridViewMenuBar = new wxPanel(this, wxID_ANY);
	p_GridViewMenuBar2x2Btn = new CNNBSSControlButton(p_GridViewMenuBar, NNBSS_EVT_GRIDVIEW_2x2, wxT("2x2"),
		wxDefaultPosition, wxSize(64, 32), wxBORDER_NONE);
	p_GridViewMenuBar4x4Btn = new CNNBSSControlButton(p_GridViewMenuBar, NNBSS_EVT_GRIDVIEW_4x4, wxT("4x4"),
		wxDefaultPosition, wxSize(64, 32), wxBORDER_NONE);
	p_GridViewMenuBarEditBtn = new CNNBSSControlButton(p_GridViewMenuBar, NNBSS_EVT_GRIDVIEW_CUSTOMIZE, _("Edit"),
		wxDefaultPosition, wxSize(64, 32), wxBORDER_NONE);
	p_GridViewMenuBarFullScreenBtn = new CNNBSSControlButton(p_GridViewMenuBar, NNBSS_EVT_GRIDVIEW_FULLSCREEN, _("Full Screen"),
		wxDefaultPosition, wxSize(96, 32), wxBORDER_NONE);
	// Sizer
	p_GridViewMenuBarHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	p_GridViewMenuBarHorizontalSizer->AddStretchSpacer();
	p_GridViewMenuBarHorizontalSizer->Add(p_GridViewMenuBar2x2Btn, wxSizerFlags().Proportion(0));
	p_GridViewMenuBarHorizontalSizer->AddSpacer(FromDIP(5));
	p_GridViewMenuBarHorizontalSizer->Add(p_GridViewMenuBar4x4Btn, wxSizerFlags().Proportion(0));
	p_GridViewMenuBarHorizontalSizer->AddSpacer(FromDIP(5));
	p_GridViewMenuBarHorizontalSizer->Add(p_GridViewMenuBarEditBtn, wxSizerFlags().Proportion(0));
	p_GridViewMenuBarHorizontalSizer->AddSpacer(FromDIP(5));
	p_GridViewMenuBarHorizontalSizer->Add(p_GridViewMenuBarFullScreenBtn, wxSizerFlags().Proportion(0));
	p_GridViewMenuBarHorizontalSizer->AddSpacer(FromDIP(5));
	p_GridViewMenuBar->SetSizerAndFit(p_GridViewMenuBarHorizontalSizer);
	// Grid View Sub Menu End

	p_GridViewGeneralSizer->Add(p_GridViewSizer, wxSizerFlags().Proportion(1).Expand());
	p_GridViewGeneralSizer->Add(p_GridViewMenuBar, wxSizerFlags().Proportion(0).Expand());

	SetSizer(p_GridViewGeneralSizer);

	SetupDesign();
	ChangeGridSize(wxSize(_GetGridWidth(), _GetGridHeight()));

	if (!m_Thumbnails.empty() && m_Thumbnails[0].m_ThumbnailPanel)
		m_Thumbnails[0].m_ThumbnailPanel->SetFocus();
}

void CNNBSSGridViewPanelBase::SetupDesign()
{
	SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	// TODO: Move these into setup colours/ design
	p_GridViewMenuBar2x2Btn->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GRIDVIEW_2x2_IDLE));
	p_GridViewMenuBar4x4Btn->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GRIDVIEW_4x4_IDLE));
	p_GridViewMenuBarEditBtn->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GRIDVIEW_SETTINGS_IDLE));
	p_GridViewMenuBarFullScreenBtn->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GRIDVIEW_EXPAND_IDLE));

	p_GridViewMenuBar->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_GridViewMenuBar2x2Btn->SetButtonBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_GridViewMenuBar4x4Btn->SetButtonBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_GridViewMenuBarEditBtn->SetButtonBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_GridViewMenuBarFullScreenBtn->SetButtonBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_GridViewMenuBar2x2Btn->SetButtonRoundness(2, CNNBSSControlButton::ButtonStates::BS_HOVER);
	p_GridViewMenuBar4x4Btn->SetButtonRoundness(2, CNNBSSControlButton::ButtonStates::BS_HOVER);
	p_GridViewMenuBarEditBtn->SetButtonRoundness(2, CNNBSSControlButton::ButtonStates::BS_HOVER);
	p_GridViewMenuBarFullScreenBtn->SetButtonRoundness(2, CNNBSSControlButton::ButtonStates::BS_HOVER);
}

void CNNBSSGridViewPanelBase::_BindEvents(bool bind)
{
	if (bind)
	{
		// I'm going to comment some of the events so they may stay as a placeholder
		// May be used if needed.
		/*Bind(EVT_MVM_DEVICE_CONNECT_ATTEMPT, &CNNBSSGridViewPanelBase::OnDeviceAddAttempt, this);
		Bind(EVT_MVM_DEVICE_CONNECT_FAIL, &CNNBSSGridViewPanelBase::OnDeviceConnectionFail, this);*/
		Bind(EVT_MVM_DEVICE_HIGH_CONNECTION, &CNNBSSGridViewPanelBase::OnDeviceConnectionLevelChange, this);
		Bind(EVT_MVM_DEVICE_LOW_CONNECTION, &CNNBSSGridViewPanelBase::OnDeviceConnectionLevelChange, this);
		/*Bind(EVT_MVM_DEVICE_CONNECTED, &CNNBSSGridViewPanelBase::OnDeviceConnected, this);*/
		Bind(EVT_MVM_DEVICE_DISCONNECTED, &CNNBSSGridViewPanelBase::OnDeviceDisconnected, this);

		Bind(EVT_MVM_CAPTURE_STARTED, &CNNBSSGridViewPanelBase::OnCameraCaptureStarted, this);
		Bind(EVT_MVM_ERROR_OPEN, &CNNBSSGridViewPanelBase::OnCameraErrorOpen, this);
		Bind(EVT_MVM_ERROR_EMPTY, &CNNBSSGridViewPanelBase::OnCameraErrorEmpty, this);
		Bind(EVT_MVM_ERROR_EXCEPTION, &CNNBSSGridViewPanelBase::OnCameraErrorException, this);

		// Button Event Binds
		p_GridViewMenuBar2x2Btn->Bind(wxEVT_LEFT_UP, &CNNBSSGridViewPanelBase::OnAnyButtonLeftUp, this);

		p_GridViewMenuBar4x4Btn->Bind(wxEVT_LEFT_UP, &CNNBSSGridViewPanelBase::OnAnyButtonLeftUp, this);

		p_GridViewMenuBarEditBtn->Bind(wxEVT_LEFT_UP, &CNNBSSGridViewPanelBase::OnAnyButtonLeftUp, this);

		p_GridViewMenuBarFullScreenBtn->Bind(wxEVT_LEFT_UP, &CNNBSSGridViewPanelBase::OnAnyButtonLeftUp, this);

		for (const auto& p : m_Thumbnails)
		{
			if (p.second.m_IsCreated && !p.second.m_IsEmpty)
			{
				p.second.m_ThumbnailPanel->Bind(wxEVT_LEFT_UP, &CNNBSSGridViewPanelBase::OnAnyThumbnailClick, this);
				p.second.m_ThumbnailPanel->Bind(wxEVT_LEFT_DCLICK, &CNNBSSGridViewPanelBase::OnAnyThumbnailDoubleClick, this);
				p.second.m_ThumbnailPanel->Bind(wxEVT_CONTEXT_MENU, &CNNBSSGridViewPanelBase::OnContextMenu, this);
			}
			else if (p.second.m_IsCreated && p.second.m_IsEmpty)
			{
				p.second.m_ThumbnailPanel->Bind(wxEVT_LEFT_DCLICK, &CNNBSSGridViewPanelBase::OnAnyThumbnailDoubleClick, this);

				p.second.m_ThumbnailPanel->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(CNNBSSGridViewPanelBase::OnDropCameraOnThumbnail), NULL, this);
				p.second.m_ThumbnailPanel->DragAcceptFiles(true);
			}

			p.second.m_ThumbnailPanel->Bind(wxEVT_SET_FOCUS, &CNNBSSGridViewPanelBase::OnAnyThumbnailSetFocus, this);
			p.second.m_ThumbnailPanel->Bind(wxEVT_KILL_FOCUS, &CNNBSSGridViewPanelBase::OnAnyThumbnailKillFocus, this);
		}
	}
	else
	{
		// I'm going to comment some of the events so they may stay as a placeholder
		// May be used if needed.
		/*Unbind(EVT_MVM_DEVICE_CONNECT_ATTEMPT, &CNNBSSGridViewPanelBase::OnDeviceAddAttempt, this);
		Unbind(EVT_MVM_DEVICE_CONNECT_FAIL, &CNNBSSGridViewPanelBase::OnDeviceConnectionFail, this);*/
		Unbind(EVT_MVM_DEVICE_HIGH_CONNECTION, &CNNBSSGridViewPanelBase::OnDeviceConnectionLevelChange, this);
		Unbind(EVT_MVM_DEVICE_LOW_CONNECTION, &CNNBSSGridViewPanelBase::OnDeviceConnectionLevelChange, this);
		/*Unbind(EVT_MVM_DEVICE_CONNECTED, &CNNBSSGridViewPanelBase::OnDeviceConnected, this);*/
		Unbind(EVT_MVM_DEVICE_DISCONNECTED, &CNNBSSGridViewPanelBase::OnDeviceDisconnected, this);

		Unbind(EVT_MVM_CAPTURE_STARTED, &CNNBSSGridViewPanelBase::OnCameraCaptureStarted, this);
		Unbind(EVT_MVM_ERROR_OPEN, &CNNBSSGridViewPanelBase::OnCameraErrorOpen, this);
		Unbind(EVT_MVM_ERROR_EMPTY, &CNNBSSGridViewPanelBase::OnCameraErrorEmpty, this);
		Unbind(EVT_MVM_ERROR_EXCEPTION, &CNNBSSGridViewPanelBase::OnCameraErrorException, this);

		// Button Event Binds
		p_GridViewMenuBar2x2Btn->Unbind(wxEVT_LEFT_UP, &CNNBSSGridViewPanelBase::OnAnyButtonLeftUp, this);

		p_GridViewMenuBar4x4Btn->Unbind(wxEVT_LEFT_UP, &CNNBSSGridViewPanelBase::OnAnyButtonLeftUp, this);

		p_GridViewMenuBarEditBtn->Unbind(wxEVT_LEFT_UP, &CNNBSSGridViewPanelBase::OnAnyButtonLeftUp, this);

		p_GridViewMenuBarFullScreenBtn->Unbind(wxEVT_LEFT_UP, &CNNBSSGridViewPanelBase::OnAnyButtonLeftUp, this);


		for (const auto& p : m_Thumbnails)
		{
			p.second.m_ThumbnailPanel->Unbind(wxEVT_CONTEXT_MENU, &CNNBSSGridViewPanelBase::OnContextMenu, this);
			p.second.m_ThumbnailPanel->Unbind(wxEVT_LEFT_UP, &CNNBSSGridViewPanelBase::OnAnyThumbnailClick, this);
			p.second.m_ThumbnailPanel->Unbind(wxEVT_LEFT_DCLICK, &CNNBSSGridViewPanelBase::OnAnyThumbnailClick, this);
			p.second.m_ThumbnailPanel->Unbind(wxEVT_LEFT_DCLICK, &CNNBSSGridViewPanelBase::OnAnyThumbnailDoubleClick, this);
			p.second.m_ThumbnailPanel->Disconnect(wxEVT_DROP_FILES, wxDropFilesEventHandler(CNNBSSGridViewPanelBase::OnDropCameraOnThumbnail), NULL, this);
			p.second.m_ThumbnailPanel->DragAcceptFiles(false);

			p.second.m_ThumbnailPanel->Unbind(wxEVT_SET_FOCUS, &CNNBSSGridViewPanelBase::OnAnyThumbnailSetFocus, this);
			p.second.m_ThumbnailPanel->Unbind(wxEVT_KILL_FOCUS, &CNNBSSGridViewPanelBase::OnAnyThumbnailKillFocus, this);
		}

	}
}

CNNBSSBitmapPanel* CNNBSSGridViewPanelBase::FindThumbnailPanel(SNNBSSMVMData* params) const
{
	CNNBSSBitmapPanel* m_Panel = nullptr;

	for (const auto& p : m_Thumbnails)
	{
		if (p.second.m_DeviceName == params->m_MVMName)
		{
			m_Panel = p.second.m_ThumbnailPanel;
		}
	}

	return m_Panel;
}

void CNNBSSGridViewPanelBase::ShowStandaloneFrame(CNNBSSBitmapPanel* BitmapPanel)
{
	wxCHECK_RET(BitmapPanel, "Invalid BitmapPanel attempting to show miniplayer!");

	for (auto& ThumbnailContainer : m_Thumbnails)
		if (ThumbnailContainer.second.m_ThumbnailPanel
			&& ThumbnailContainer.second.m_ThumbnailPanel->GetAttachedMVM()
			/*&& ThumbnailContainer.second.m_ThumbnailPanel->GetProperty<long>(CNNBSSBitmapPanel::PROPERTIES::BPP_STATE) != CNNBSSBitmapPanel::STATES::BPS_RECIEVES*/)
		{
			if (!ThumbnailContainer.second.p_StandalonePanel)
			{
				ThumbnailContainer.second.p_StandalonePanel = new CNNBSSBitmapFrameStandalone(nullptr, ThumbnailContainer.second.m_ThumbnailPanel->GetAttachedMVM());
				ThumbnailContainer.second.p_StandalonePanel->Show();
				return;;
			}
			else
			{
				ThumbnailContainer.second.p_StandalonePanel->Raise();
				return;
			}
		}

	NNBSSErrorShow("Can't display miniplayer of that thumbnail!", 100);
	return;
}

CNNBSSBitmapFrameStandalone* CNNBSSGridViewPanelBase::GetStandaloneFrame()
{
	return nullptr;
}

CNNBSSMovingVisualMediaThread* CNNBSSGridViewPanelBase::GetThumbnailThread(CNNBSSBitmapPanel* panel)const
{
	if (!panel)
		return nullptr;

	SStandardThumbnail ThumbnailContainer;
	for (const auto& p : m_Thumbnails)
	{
		if (p.second.m_ThumbnailPanel == panel)
			ThumbnailContainer = p.second;
	}

	for (const auto& MVM : SCNNBSSMVMParametersList)
	{
		if (!MVM)
			continue;

		if (MVM->m_MVMName == ThumbnailContainer.m_ThumbnailPanel->GetAttachedMVM()->m_MVMName &&
			MVM->m_MVMSourceAddress == ThumbnailContainer.m_ThumbnailPanel->GetAttachedMVM()->m_MVMSourceAddress)
		{
			if (!MVM->m_MVMThread)
			{
				return nullptr;
			}
			else
			{
				return MVM->m_MVMThread;
			}
		}
	}
	return nullptr;
}

CNNBSSMovingVisualMediaThread* CNNBSSGridViewPanelBase::GetThumbnailThread(SNNBSSMVMData* params)const
{
	for (const auto& MVM : SCNNBSSMVMParametersList)
	{
		if (MVM->m_MVMName == params->m_MVMName && MVM->m_MVMSourceAddress == params->m_MVMSourceAddress)
		{
			if (!MVM->m_MVMThread)
			{
				return nullptr;
			}
			else
			{
				return MVM->m_MVMThread;
			}
		}
	}
	return nullptr;
}

SNNBSSMVMData* CNNBSSGridViewPanelBase::GetThumbnailMVMParameters(CNNBSSBitmapPanel* panel)
{
	SStandardThumbnail ThumbnailContainer;
	for (const auto& p : m_Thumbnails)
	{
		if (p.second.m_ThumbnailPanel == panel)
			ThumbnailContainer = p.second;
	}

	for (const auto& p : SCNNBSSMVMParametersList)
	{
		if (!p)
			continue;

		if (p->m_MVMName == ThumbnailContainer.m_ThumbnailPanel->GetAttachedMVM()->m_MVMName
			&& p->m_MVMSourceAddress == ThumbnailContainer.m_ThumbnailPanel->GetAttachedMVM()->m_MVMSourceAddress)
		{
			return p;
		}
	}
	return nullptr;
}

SStandardThumbnail CNNBSSGridViewPanelBase::GetThumbnailContainer(CNNBSSBitmapPanel* panel)
{
	for (const auto& p : m_Thumbnails)
	{
		if (p.second.m_ThumbnailPanel == panel)
			return p.second;
	}

	return SStandardThumbnail();
}

void CNNBSSGridViewPanelBase::ThumbnailHandle()
{

	if (_IsExpanded)
	{
		FullscreenThumbnail(nullptr, false);
		return;
	}

	if (p_GridViewSizer->GetItemCount() >= NNBSS_GRIDVIEW_MAX_CAMERA_COUNT)
		return;

	for (int c = 0; c < p_GridViewSizer->GetEffectiveColsCount() * p_GridViewSizer->GetEffectiveRowsCount(); c++)
	{
		if (m_Thumbnails[c].m_IsCreated || !m_Thumbnails[c].m_IsEmpty)
		{
			continue;
		}

		SStandardThumbnail camView;

		camView.m_IsEmpty = true;
		camView.m_IsCreated = true;
		camView.m_ThumbnailPanel = new CNNBSSBitmapPanel(this);
		// Set Panel State Empty
		camView.m_ThumbnailPanel->SetProperty<long>(CNNBSSBitmapPanel::PROPERTIES::BPP_STATE, CNNBSSBitmapPanel::STATES::BPS_EMPTY);
		// Set Place Holder Text for Thumbnail Panel
		camView.m_ThumbnailPanel->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_PLACEHOLDERTEXT, DefaultThumbnailPlaceholder);

		m_Thumbnails[c] = camView;
	}

	GetSizer()->Layout();
	CNNBSSAuiManagerAddressHandle()->Update();
}

CNNBSSBitmapPanel* CNNBSSGridViewPanelBase::GetSelectedThumbnail()
{
	for (const auto& p : m_Thumbnails)
	{
		if (p.second.m_ThumbnailPanel->GetProperty<bool>(CNNBSSBitmapPanel::PROPERTIES::BPP_SELECTED))
		{
			return p.second.m_ThumbnailPanel;
		}
	}

	return nullptr;
}

void CNNBSSGridViewPanelBase::FullscreenThumbnail(CNNBSSBitmapPanel* cameraPanel, bool fullscreen)
{
	// Force to exit fullscreen
	if (!cameraPanel)
		fullscreen = false;

	// If fullscreen's prefered:
	if (fullscreen)
	{
		if (p_GridViewSizer)
			p_GridViewSizer->Clear();

		//creating sizer is prerequisite for setting sizer. however, i'll create it at first
		m_FullScreenSizer = new wxBoxSizer(wxVERTICAL);

		m_FullScreenSizer->Clear();
		if (!m_Thumbnails.empty())
		{
			for (const auto& p : m_Thumbnails)
			{
				p.second.m_ThumbnailPanel->Hide();
			}
		}
		else
		{
			NNBSSMessageBoxSimple(_("Error"), _("Could not swich to fullscreen. Error code: GV0002"), wxICON_ERROR);
			return;
		}

		cameraPanel->Show(true);
		m_FullScreenSizer->Add(cameraPanel, wxSizerFlags().Proportion(1).Expand());

		GetSizer()->Replace(p_GridViewSizer, m_FullScreenSizer);

		_IsExpanded = true;
	}
	else
	{
		// again we create the sizer. the old does not work anymore?...
		p_GridViewSizer = new wxGridSizer(_GetGridHeight(), _GetGridWidth(), 0, 0);

		if (m_FullScreenSizer)
			m_FullScreenSizer->Clear();

		p_GridViewSizer->Clear();
		for (const auto& p : m_Thumbnails)
		{
			if (p.first >= _GetGridWidth() * _GetGridHeight())
				break;
			p.second.m_ThumbnailPanel->Show();
			p_GridViewSizer->Add(p.second.m_ThumbnailPanel, wxSizerFlags().Proportion(1).Expand().Border());
		}

		GetSizer()->Replace(m_FullScreenSizer, p_GridViewSizer);

		_IsExpanded = false;
	}

	// Commit changes
	GetSizer()->Layout();
	Refresh(); Update();
	CNNBSSAuiManagerAddressHandle()->Update();
}

bool CNNBSSGridViewPanelBase::_IsFullScreen()const
{
	return  _IsExpanded;
}

int CNNBSSGridViewPanelBase::_GetGridWidth()const
{
	return _GridWidth;
}

int CNNBSSGridViewPanelBase::_GetGridHeight()const
{
	return _GridHeight;
}

void CNNBSSGridViewPanelBase::_SetGridWidth(const int& width)
{
	_GridWidth = width;
}

void CNNBSSGridViewPanelBase::_SetGridHeight(const int& height)
{
	_GridHeight = height;
}

void CNNBSSGridViewPanelBase::_SetBalancedGridScale(const int& balancedScale)
{
	_GridWidth = balancedScale;
	_GridHeight = balancedScale;
}

bool CNNBSSGridViewPanelBase::ChangeGridSize(const wxSize& size)
{

	if (size.x < 1 || size.y < 1)
		return false;

	_BindEvents(false);

	// Show dialog before removing
	static bool _ShowBackgroundTaskWarning = true;
	if (_ShowBackgroundTaskWarning)
		for (const auto& p : m_Thumbnails)
		{
			if (!p.second.m_IsEmpty)
			{
				if (p.first > size.x * size.y)
				{
					wxRichMessageDialog dlg(GetActiveUXFrame(), _("Some cameras will stay off the grid and function. You can see them when the grid expands again, so they won't be deleted. If you don't need them and don't want any performance loss, we highly recommend deleting them."), _("Warning"), wxYES_NO);
					dlg.ShowCheckBox(_("Don't show this dialog again"));
					if (dlg.ShowModal() == wxID_YES)
					{
						if (dlg.IsCheckBoxChecked())
							_ShowBackgroundTaskWarning = false;
					}
					else
					{
						_BindEvents(true);

						return false;
					}
				}
			}
		}

	if (_IsExpanded)
	{
		if (!GetSelectedThumbnail())
		{
			_BindEvents(true);

			return false;
		}
		FullscreenThumbnail(GetSelectedThumbnail(), false);
	}

	p_GridViewSizer->Clear();

	{
		_SetGridWidth(size.x);
		_SetGridHeight(size.y);
	}

	p_GridViewSizer->SetCols(_GetGridWidth());
	p_GridViewSizer->SetRows(_GetGridHeight());

	ThumbnailHandle();

	// Hide them all
	for (const auto& p : m_Thumbnails)
	{
		p.second.m_ThumbnailPanel->Hide();
	}

	p_GridViewSizer->Layout();

	//Add thumbnails again till they are about to cross grid size
	for (const auto& p : m_Thumbnails)
	{
		if (p.first > (size.x * size.y) - 1)
			break;
		p.second.m_ThumbnailPanel->Show();
		p_GridViewSizer->Add(p.second.m_ThumbnailPanel, wxSizerFlags().Proportion(1).Expand().Border());
	}

	GetSizer()->Layout();
	Refresh(); Update();
	CNNBSSAuiManagerAddressHandle()->Update();

	_BindEvents(true);// Before exit the function (return) we should bind events again

	return true;
}

void CNNBSSGridViewPanelBase::OnAnyButtonLeftUp(wxMouseEvent& evt)
{
	CNNBSSMouseEventsAddressHandle()->OnAnyButtonClick(evt);

	evt.Skip();
}
//----------------------------------------------------------------------------------

void CNNBSSGridViewPanelBase::OnAnyThumbnailSetFocus(wxFocusEvent& evt)
{
	for (auto& thmb : m_Thumbnails)
	{
		thmb.second.m_ThumbnailPanel->SetProperty<bool>(CNNBSSBitmapPanel::PROPERTIES::BPP_SELECTED, false);
	}

	CNNBSSBitmapPanel* cast = dynamic_cast<CNNBSSBitmapPanel*>(evt.GetEventObject());
	cast->SetProperty<bool>(CNNBSSBitmapPanel::PROPERTIES::BPP_SELECTED, true);

	evt.Skip();
}

void CNNBSSGridViewPanelBase::OnAnyThumbnailKillFocus(wxFocusEvent& evt)
{
	evt.Skip();
}
