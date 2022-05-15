///////////////////////////////////////////////////////////////////////////////
// Name:        onecameraframe.cpp
// Purpose:     Displays full resolution output from a single camera
// Author:      PB
// Created:     2021-11-18, Edited By Orkhan Aliyev: 2021-12-07
// Copyright:   (c) 2021 PB
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

#include "StandaloneCameraFrame.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// wxWidgets
#include <wx/display.h>
#include <wx/graphics.h>
// OpenCV
#include <opencv2/core.hpp>

// Destroying this class will give us error at application close(not immediately I mean)
// also somehow at destroy of this class just stops the main panel's thread. fix -

CNNBSSBitmapFrameStandalone::CNNBSSBitmapFrameStandalone(wxWindow* parent,
	SNNBSSMVMData* mvmParams)

	: wxFrame(parent, wxID_ANY, mvmParams->m_MVMName),
	m_mvmParams(mvmParams)
{
	if (!GetThumbnailThread())
	{
		NNBSSMessageBoxSimple(_("Error"), _("Camera could not be added! Error Code: C4409 invalid thread!"), wxICON_ERROR);
		return;
	}

	//---------------------------------------------
	m_cameraPanel = new CNNBSSBitmapPanel(this);
	m_cameraPanel->Attach(mvmParams);
	m_cameraPanel->SetProperty<long>(CNNBSSBitmapPanel::PROPERTIES::BPP_STATE, CNNBSSBitmapPanel::STATES::BPS_RECIEVES);

	//---------------------------------------------
	// Bind events
	m_cameraPanel->Bind(wxEVT_LEFT_DOWN, &CNNBSSBitmapFrameStandalone::OnLeftDown, this);
	m_cameraPanel->Bind(wxEVT_LEFT_UP, &CNNBSSBitmapFrameStandalone::OnLeftUp, this);
	m_cameraPanel->Bind(wxEVT_LEFT_DCLICK, &CNNBSSBitmapFrameStandalone::OnDoubleClick, this);
	m_cameraPanel->Bind(wxEVT_MOTION, &CNNBSSBitmapFrameStandalone::OnMouseMove, this);
	m_cameraPanel->Bind(wxEVT_CONTEXT_MENU, &CNNBSSBitmapFrameStandalone::OnContextMenu, this);
	//---------------------------------------------
	wxBoxSizer* p_BaseSizer = new wxBoxSizer(wxVERTICAL);
	p_BaseSizer->Add(m_cameraPanel, wxSizerFlags().Proportion(1).Expand());
	SetSizerAndFit(p_BaseSizer);
	//---------------------------------------------
	m_WindowFlags & wxRESIZE_BORDER ? 0 : m_WindowFlags |= wxRESIZE_BORDER;
	SetWindowStyle(m_WindowFlags);
	SetMinSize(wxSize(150, 150));
	SetSize(wxSize(640, 480));
	Center();

	Show();
}

CNNBSSBitmapFrameStandalone::~CNNBSSBitmapFrameStandalone()
{
}

void CNNBSSBitmapFrameStandalone::SetCameraBitmap(const wxBitmap& bitmap)
{
	m_cameraPanel->SetBitmap(bitmap);

	if (bitmap.IsOk() && !m_clientSizeAdjusted)
	{
		m_cameraPanel->SetMinSize(bitmap.GetSize());
		m_cameraPanel->SetMaxSize(bitmap.GetSize());
		SetClientSize(bitmap.GetSize());
		m_clientSizeAdjusted = true;
		SetTitle(wxString::Format("%s (resolution %dx%d)",
			wxT(""), bitmap.GetWidth(), bitmap.GetHeight()));
	}
}


CNNBSSMovingVisualMediaThread* CNNBSSBitmapFrameStandalone::GetThumbnailThread()
{
	if (!m_mvmParams->m_MVMThread)
	{
		return nullptr;
	}
	else
	{
		return m_mvmParams->m_MVMThread;
	}

	return nullptr;
}

// MouseEvents
void CNNBSSBitmapFrameStandalone::OnLeftDown(wxMouseEvent& evt)
{
	try
	{
		m_cameraPanel->CaptureMouse();

		wxPoint pos = ClientToScreen(evt.GetPosition());
		wxPoint origin = GetPosition();
		int dx = pos.x - origin.x;
		int dy = pos.y - origin.y;
		m_delta = wxPoint(dx, dy);
	}
	catch (std::exception& e)
	{
		NNBSSMessageBoxSimple(_("Error"), _("What are you doing, man?!"), wxICON_ERROR);
	}
	catch (...)
	{
		NNBSSMessageBoxSimple(_("Error"), _("What are you doing, man?!"), wxICON_ERROR);
	}
	evt.Skip();
}

void CNNBSSBitmapFrameStandalone::OnLeftUp(wxMouseEvent& evt)
{
	if (m_cameraPanel->HasCapture())
	{
		m_cameraPanel->ReleaseMouse();
	}

	evt.Skip();
}

void CNNBSSBitmapFrameStandalone::OnDoubleClick(wxMouseEvent& evt)
{

}

void CNNBSSBitmapFrameStandalone::OnMouseMove(wxMouseEvent& evt)
{
	if (evt.Dragging() && evt.LeftIsDown())
	{
		if (IsMaximized())
		{
			Maximize(false);
			CenterOnScreen();
			if (m_cameraPanel->HasCapture())
			{
				m_cameraPanel->ReleaseMouse();
			}
			evt.Skip();
			return;
		}

		const auto screenPosCurrent = ClientToScreen(evt.GetPosition());
		Move(screenPosCurrent - m_delta);
	}
	evt.Skip();
}


void CNNBSSBitmapFrameStandalone::OnContextMenu(wxContextMenuEvent& evt)
{
	if (m_cameraPanel->HasCapture())
	{
		m_cameraPanel->ReleaseMouse();
	}

	// Preferences menu
	if (!m_CM.p_PreferencesSubMenu)
	{
		m_CM.p_PreferencesSubMenu	= new wxMenu;
		m_CM.p_StayOnTopItem		= 
			new wxMenuItem(m_CM.p_PreferencesSubMenu, CMEID_WINDOWSTYLE_STAYONTOP, _("Preferences"), _("Keeps window on top always"));
		m_CM.p_ShowCaptionItem =
			new wxMenuItem(m_CM.p_PreferencesSubMenu, CMEID_WINDOWSTYLE_SHOWCAPTION, _("Show Caption"), _("Display caption above the window"));
		m_CM.p_SetCaptionItem =
			new wxMenuItem(m_CM.p_PreferencesSubMenu, CMEID_WINDOWSTYLE_SETCAPTION, _("Set Caption"), _("If Show Caption is checked, this caption will be displayed"));

		m_CM.p_PreferencesSubMenu->AppendCheckItem(CMEID_WINDOWSTYLE_STAYONTOP, _("Stay on top"));
		m_CM.p_PreferencesSubMenu->AppendCheckItem(CMEID_WINDOWSTYLE_SHOWCAPTION, _("Show Caption"));
		m_CM.p_PreferencesSubMenu->Append(CMEID_WINDOWSTYLE_SETCAPTION, _("Set Caption"));

	}

	m_CM.p_MainContextMenu = new wxMenu;
	int					id = wxID_NONE;
	m_CM.p_MainContextMenu->Append(CMEID_CLOSE, "Close");
	m_CM.p_MainContextMenu->Append(CMEID_MINIMIZE, "Minimize");
	m_CM.p_MainContextMenu->Append(CMEID_RESTORE, "Restore");
	m_CM.p_MainContextMenu->AppendSubMenu(m_CM.p_PreferencesSubMenu, _("Preferences"), _("Keeps window on top always"));

	id = m_cameraPanel->GetPopupMenuSelectionFromUser(*m_CM.p_MainContextMenu);

	switch (id)
	{
	case CMEID_CLOSE:
		Close(true);
		break;
	case CMEID_RESTORE:
		Maximize(!IsMaximized());
		break;
	case CMEID_MINIMIZE:
		Iconize(!IsIconized());
		break;
	case CMEID_WINDOWSTYLE_STAYONTOP:
	{
		// If checked, enable stay on top; otherwise disable if exist
		if (m_CM.p_StayOnTopItem->IsChecked()) { m_WindowFlags& wxSTAY_ON_TOP ? 0 : m_WindowFlags |= wxSTAY_ON_TOP;}
		else { m_WindowFlags& wxSTAY_ON_TOP ? m_WindowFlags &= ~wxSTAY_ON_TOP : 0;}
		SetWindowStyle(m_WindowFlags);
	}	break;
	case CMEID_WINDOWSTYLE_SHOWCAPTION:
	{
		if (m_CM.p_ShowCaptionItem->IsChecked()) {
			m_cameraPanel->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_RIGHT_TOP_TEXT, GetTitle());
		}
		else 
		{ m_cameraPanel->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_RIGHT_TOP_TEXT, wxEmptyString); }
	} break;
	case CMEID_WINDOWSTYLE_SETCAPTION:
	{
		wxTextEntryDialog* p_LabelDialog = new wxTextEntryDialog(GetActiveUXFrame(), _("Change the caption of this window"),
			_("Change caption"), GetTitle());

		if (p_LabelDialog->ShowModal() == wxID_OK)
		{
			// If BPP_RIGHT_TOP_TEXT value is equal to wxEmptyString, it means we can't set a value. 
			// Neither we should set value when user hits enter with no/empty label
			if (!p_LabelDialog->GetValue().IsEmpty()
				&& m_cameraPanel->GetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_RIGHT_TOP_TEXT) != wxEmptyString)
			{
				SetTitle(p_LabelDialog->GetValue());
				m_cameraPanel->SetProperty<wxString>(CNNBSSBitmapPanel::PROPERTIES::BPP_RIGHT_TOP_TEXT, GetTitle());
			}
		}
	} break;
	default:
		break;
	}

	evt.Skip();
}

WXLRESULT CNNBSSBitmapFrameStandalone::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
	/* When we have a custom titlebar in the window, we don't need the non-client area of a normal window
	  * to be painted. In order to achieve this, we handle the "WM_NCCALCSIZE" which is responsible for the
	  * size of non-client area of a window and set the return value to 0. Also we have to tell the
	  * application to not paint this area on activate and deactivation events so we also handle
	  * "WM_NCACTIVATE" message. */
	switch (nMsg)
	{
	case WM_NCACTIVATE:
	{
		/* Returning 0 from this message disable the window from receiving activate events which is not
		desirable. However When a visual style is not active (?) for this window, "lParam" is a handle to an
		optional update region for the nonclient area of the window. If this parameter is set to -1,
		DefWindowProc does not repaint the nonclient area to reflect the state change. */
		lParam = -1;
		break;
	}
	/* To remove the standard window frame, you must handle the WM_NCCALCSIZE message, specifically when
	its wParam value is TRUE and the return value is 0 */
	case WM_NCCALCSIZE:
		if (wParam)
		{
			/* Detect whether window is maximized or not. We don't need to change the resize border when win is
			*  maximized because all resize borders are gone automatically */
			HWND hWnd = (HWND)this->GetHandle();
			WINDOWPLACEMENT wPos;
			// GetWindowPlacement fail if this member is not set correctly.
			wPos.length = sizeof(wPos);
			GetWindowPlacement(hWnd, &wPos);
			if (wPos.showCmd != SW_SHOWMAXIMIZED)
			{
				RECT borderThickness;
				SetRectEmpty(&borderThickness);
				AdjustWindowRectEx(&borderThickness,
					GetWindowLongPtr(hWnd, GWL_STYLE) & ~WS_CAPTION, FALSE, NULL);
				borderThickness.left *= -1;
				borderThickness.top *= -1;
				NCCALCSIZE_PARAMS* sz = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				// Add 1 pixel to the top border to make the window resizable from the top border
				sz->rgrc[0].top += 1;
				sz->rgrc[0].left += borderThickness.left;
				sz->rgrc[0].right -= borderThickness.right;
				sz->rgrc[0].bottom -= borderThickness.bottom;
				return 0;
			}
		}
		break;
	}
	return wxFrame::MSWWindowProc(nMsg, wParam, lParam);
}