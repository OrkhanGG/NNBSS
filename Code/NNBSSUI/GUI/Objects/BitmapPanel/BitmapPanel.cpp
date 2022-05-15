///////////////////////////////////////////////////////////////////////////////
// Name:        BitmapPanel.cpp
// Purpose:     Displays a bitmap on a wxWindow
// Author:      Orkhan Aliyev
// Created:     2021-12-07
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "BitmapPanel.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "BitmapPanel.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

/***********************************************************************************************

	CNNBSSThumbnailFrameData

***********************************************************************************************/

bool CNNBSSThumbnailFrameData::IsOk()
{
	if (!p_Thumbnail ||
		/*p_Thumbnail->GetProperty<long>(CNNBSSBitmapPanel::PROPERTIES::BPP_STATE) != CNNBSSBitmapPanel::STATES::BPS_RECIEVES*/
		m_MVMName.empty() || m_MVMSourceAddress.empty()
		|| !m_EventSink || !p_Thumbnail)
		return false;

	return true;
}

/***********************************************************************************************

	CNNBSSBitmapPanel

***********************************************************************************************/

CNNBSSBitmapPanel::CNNBSSBitmapPanel(wxWindow* parent)
	: wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxFULL_REPAINT_ON_RESIZE)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	Bind(wxEVT_PAINT, &CNNBSSBitmapPanel::OnPaint, this);

	Bind(wxEVT_SET_FOCUS, &CNNBSSBitmapPanel::OnFocusSet, this);
	Bind(wxEVT_KILL_FOCUS, &CNNBSSBitmapPanel::OnFocusKill, this);
	Bind(wxEVT_KEY_UP, &CNNBSSBitmapPanel::OnAnyKeyRelease, this);
}

CNNBSSBitmapPanel::~CNNBSSBitmapPanel()
{
	Detach(p_MVM);
}

void CNNBSSBitmapPanel::Reset()
{
	m_PlaceHolderText = DefaultThumbnailPlaceholder;
	SetBitmap(wxBitmap());

	Refresh(); Update();
}

void CNNBSSBitmapPanel::Attach(SNNBSSMVMData* MVM)
{
	if (std::find(GetBitmapPanelPool().begin(), GetBitmapPanelPool().end(), this) != GetBitmapPanelPool().end())
	{
		return;
	}

	wxCriticalSectionLocker locker(m_CriticalSection);

	m_processFrameDataTimer.Start(m_processFrameDataInterval);
	m_processFrameDataTimer.Bind(wxEVT_TIMER, &CNNBSSBitmapPanel::OnProcessFrameData, this);

	SetAttachedMVM(MVM);
	GetBitmapPanelPool().emplace_back(this);
}

void CNNBSSBitmapPanel::Detach(SNNBSSMVMData* MVM)
{
	if (std::find(GetBitmapPanelPool().begin(), GetBitmapPanelPool().end(), this) == GetBitmapPanelPool().end())
	{
		return;
	}

	wxCriticalSectionLocker locker(m_CriticalSection);

	m_processFrameDataTimer.Stop();
	m_processFrameDataTimer.Unbind(wxEVT_TIMER, &CNNBSSBitmapPanel::OnProcessFrameData, this);

	SetAttachedMVM(nullptr);
	GetBitmapPanelPool().erase(std::find(GetBitmapPanelPool().begin(), GetBitmapPanelPool().end(), this));

	SetProperty<bool>(PROPERTIES::BPP_EVENT_INIT_SENT, false);// Send events for once

	SetAttachedMVM(nullptr);
}

void CNNBSSBitmapPanel::SetBitmap(const wxBitmap bitmap)
{
	m_Bitmap = bitmap;

	Refresh(); Update();
}

/* TODO: Function name change : */ void CNNBSSBitmapPanel::OnSelect(wxDC* dc)
{
	if (_IsSelected)
	{
		const wxSize clientSize(GetClientSize());

		// USE A MEMORY DC TO DRAW DIRECTLY ONTO THE BITMAP
		// DRAW A BOX (WITH BLACK OUTLINE) AT THE BOTTOM OF THE PANEL.
		const wxRect bannerRect(wxPoint(0, 0),
			wxPoint(clientSize.GetWidth(), clientSize.GetHeight()));
		dc->SetBrush(*wxTRANSPARENT_BRUSH);
		dc->SetPen(wxPen(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT, 3));
		dc->DrawRectangle(bannerRect);

		if (m_Bitmap.IsOk())
		{
			// draw name of the panel
			dc->SetTextForeground(*wxWHITE);
			dc->DrawLabel(m_PanelName.empty() ? GetAttachedMVM()->m_MVMName : m_PanelName, bannerRect.Deflate(5, 5), wxALIGN_BOTTOM | wxALIGN_LEFT);

			// draw date and time
			if (m_flags & FLAGS::BPF_DATETIME)
			{
				wxLongLong  rawt = (wxLongLong)wxGetUTCTimeMillis();
				double epoch_time = rawt.ToDouble() / 1e+3;
				std::time_t   time = static_cast<time_t>(epoch_time);
				std::tm tm = *std::localtime(&time);
				char Buf[80];
				std::strftime(Buf, sizeof(Buf), "Date: %Y-%m-%d\nTime: %H:%M:%S", &tm);
				wxString finaltime(Buf);

				//long long postfix = rawt.operator%(1000).ToLong();
				//finaltime.append(".").append(std::to_string(postfix));

				// draw the copyright label on the right side
				dc->SetFont(wxFontInfo(8));
				dc->DrawLabel(finaltime, bannerRect.Deflate(5, 5), wxALIGN_BOTTOM | wxALIGN_RIGHT);
			}
		}
	}
}

void CNNBSSBitmapPanel::OnFocusSet(wxFocusEvent& evt)
{
	Refresh();

	evt.Skip();
}

void CNNBSSBitmapPanel::OnFocusKill(wxFocusEvent& evt)
{
	Refresh();

	evt.Skip();
}

void CNNBSSBitmapPanel::OnProcessFrameData(wxTimerEvent&)
{
	NNBSSThumbnailFrameDataUPTRList frameData;
	wxStopWatch         stopWatch;

	stopWatch.Start();
	{
		wxCriticalSectionLocker locker(m_CriticalSection);

		if (m_FrameData.empty())
			return;

		frameData = std::move(m_FrameData);
	}

	for (const auto& fd : frameData)
	{
		//if (it == m_cameras.end() || !it->second.thread->IsCapturing())
			//continue; // ignore yet-unprocessed frames from removed or errored cameras

		const wxBitmap* cameraFrame = fd->GetFrame();
		const wxBitmap* cameraFrameThumbnail = fd->GetThumbnail();

		if (!cameraFrame || !cameraFrame->IsOk())
		{
			continue;
		}

		if (this)
		{
			if (cameraFrameThumbnail && cameraFrameThumbnail->IsOk())
				SetBitmap(*cameraFrameThumbnail);
			else
				SetBitmap(wxBitmap());
		}
	}


	frameData.clear();
}

// On MSW, displaying 4k bitmaps from 60 fps camera with
// wxAutoBufferedPaintDC in some scenarios meant the application
// after while started for some reason lagging very badly,
// even unable to process the camera frames. This did not happen
// when using wxPaintDC instead. However, drawing the same way with
// wxPaintDC instead of wxAutoBufferedPaintDC meant the panels were flashing.
// In the end, the old-fashioned way with wxMemoryDC
// is used, even if it means that drawing cannot be timed (easily).

void CNNBSSBitmapPanel::OnPaint(wxPaintEvent&)
{
	wxDC* paintDC{ nullptr };

#if CAMERAPANEL_USE_AUTOBUFFEREDPAINTDC
	wxAutoBufferedPaintDC dc(this);
	wxStopWatch           stopWatch;

	paintDC = &dc;
	stopWatch.Start();
#else
	const wxSize clientSize(GetClientSize());

	wxPaintDC dc(this);

	if (clientSize.GetWidth() < 1 || clientSize.GetHeight() < 1)
		return;

	wxBitmap   memDCBitmap(clientSize);
	wxMemoryDC memDC(&dc);

	memDC.SelectObject(memDCBitmap);
	paintDC = &memDC;
#endif
	paintDC->SetBackground(RGB_COLOURDEF_BLACK);
	paintDC->Clear();

	if (m_Bitmap.IsOk())
		paintDC->DrawBitmap(m_Bitmap, 0, 0, false);

	const wxRect MainRect(wxPoint(0, 0),
		wxPoint(clientSize.GetWidth(), clientSize.GetHeight()));

	OnSelect(paintDC);

	// draw bitmap at 0,0 pos
	if (m_Bitmap.IsOk())
	{
		if (!m_RightTopText.IsEmpty())
		{
			paintDC->SetPen(wxPen(RGB_COLOURDEF_WHITE, 3));
			wxDCTextColourChanger tcChanger(*paintDC, RGB_COLOURDEF_WHITE);
			paintDC->DrawLabel(m_RightTopText, MainRect.Deflate(5, 5), wxALIGN_TOP | wxALIGN_RIGHT);
		}
	}
	else
	{
		wxDCTextColourChanger tcChanger(*paintDC, RGB_COLOURDEF_WHITE);

		paintDC->DrawLabel(m_PlaceHolderText, MainRect, wxALIGN_CENTRE_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
	}

#if CAMERAPANEL_USE_AUTOBUFFEREDPAINTDC
	if (m_drawPaintTime && m_status == Receiving)
		infoText.Printf("%s\nFrame painted in %ld ms", infoText, stopWatch.Time());
#endif

#if !CAMERAPANEL_USE_AUTOBUFFEREDPAINTDC
	dc.Blit(wxPoint(0, 0), clientSize, &memDC, wxPoint(0, 0));
#endif
}

void CNNBSSBitmapPanel::OnAnyKeyRelease(wxKeyEvent& evt)
{
	if (!HasFocus() || !IsEnabled() || !IsShown() || !IsShownOnScreen())
	{
		evt.Skip();
		return;
	}

	if (evt.GetKeyCode() == 13)// On press Enter key
	{
		// Simulate double click
		wxCommandEvent simulateClick(wxEVT_LEFT_DCLICK, m_windowId);
		simulateClick.SetEventObject(this);
		GetEventHandler()->ProcessEvent(simulateClick);
	}
	else if (evt.GetKeyCode() == 395)// On press Menu key
	{
		// Simulate double click
		wxCommandEvent simulateClick(wxEVT_RIGHT_UP, m_windowId);
		simulateClick.SetEventObject(this);
		GetEventHandler()->ProcessEvent(simulateClick);
	}
	evt.Skip();
}

