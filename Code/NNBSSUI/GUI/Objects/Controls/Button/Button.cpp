/////////////////////////////////////////////////////////////////////////////
// Name:        Button.cpp
// Purpose:     Button Control
// Author:      Orkhan Aliyev
// Created:     02-04-2022
// Copyright:   (c) Orkhan Aliyev	
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "Button.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/control.h"
#include "wx/settings.h"
#include "wx/dc.h"
#include "wx/dcclient.h"
#endif // WX_PRECOMP
#include "wx/dcgraph.h"

#include "Button.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// Standard C Lib
#include <math.h>

constexpr static int littlePixelSpace = 5;

// Constructor
void CNNBSSControlButton::Create(wxWindow* parent, wxWindowID id,
	const wxString& label,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxValidator& validator,
	const wxString& name)
{
	wxWindow::Create(parent, id, pos, size, style | wxBORDER_NONE | wxFULL_REPAINT_ON_RESIZE);

	if (!label.empty())
		SetButtonLabel(label, m_ButtonCurrentState);

	Bind(wxEVT_LEFT_DOWN, &CNNBSSControlButton::OnMouseLeftDown, this, wxID_ANY);
	Bind(wxEVT_LEFT_UP, &CNNBSSControlButton::OnMouseLeftUp, this, wxID_ANY);
	Bind(wxEVT_RIGHT_UP, &CNNBSSControlButton::OnMouseRightUp, this, wxID_ANY);
	Bind(wxEVT_ENTER_WINDOW, &CNNBSSControlButton::OnMouseEnter, this, wxID_ANY);
	Bind(wxEVT_LEAVE_WINDOW, &CNNBSSControlButton::OnMouseLeave, this, wxID_ANY);
	Bind(wxEVT_MOTION, &CNNBSSControlButton::OnMouseMotion, this, wxID_ANY);
	Bind(wxEVT_MOUSE_CAPTURE_LOST, &CNNBSSControlButton::OnMouseLost, this, wxID_ANY);
	Bind(wxEVT_KEY_DOWN, &CNNBSSControlButton::OnKeyPressed, this, wxID_ANY);
	Bind(wxEVT_KEY_UP, &CNNBSSControlButton::OnKeyReleased, this, wxID_ANY);
	Bind(wxEVT_SET_FOCUS, &CNNBSSControlButton::OnSetFocus, this, wxID_ANY);
	Bind(wxEVT_KILL_FOCUS, &CNNBSSControlButton::OnKillFocus, this, wxID_ANY);
	Bind(wxEVT_PAINT, &CNNBSSControlButton::OnPaint, this, wxID_ANY);
	Bind(wxEVT_SIZE, &CNNBSSControlButton::OnResize, this, wxID_ANY);

	Refresh();
}

void CNNBSSControlButton::SetButtonLabel(const wxString& label, const ButtonStates& state)
{
	m_ButtonLabels[state] = label;

	Refresh();// Update drawing
}

void CNNBSSControlButton::SetButtonHelpLabel(const wxString& label, const ButtonStates& state)
{
	m_ButtonHelpLabels[state] = label;

	Refresh();// Update drawing
}

void CNNBSSControlButton::SetButtonFont(const wxFont& font, const ButtonStates& state)
{
	m_ButtonFonts[state] = font;

	Refresh();
}

void CNNBSSControlButton::SetButtonBitmap(const wxBitmap& bitmap, const ButtonStates& state)
{
	m_ButtonBitmaps[state] = bitmap;

	Refresh();
}

void CNNBSSControlButton::SetButtonTextColour(const wxColour& colour, const ButtonStates& state)
{
	m_ButtonTextColours[state] = colour;

	Refresh();
}

void CNNBSSControlButton::SetButtonBackgroundColour(const wxColour& colour, const ButtonStates& state)
{
	m_ButtonBackgroundColours[state] = colour;

	Refresh();
}

void CNNBSSControlButton::SetButtonLabelAlignment(const long& alignment, const ButtonStates& state)
{
	m_ButtonLabelAlignments[state] = alignment;

	Refresh();
}

void CNNBSSControlButton::SetButtonRoundness(const int& roundness, const ButtonStates& state)
{
	m_ButtonRoundness[state] = roundness;

	Refresh();
}

wxString CNNBSSControlButton::GetCurrentLabel() const
{
	if (auto it{ m_ButtonLabels.find(m_ButtonCurrentState) }; it != std::end(m_ButtonLabels))
	{
		const auto& [key, value] { *it };
		if (!value.empty())
			return value;
	}
	// If current state has no special, try to find the default
	if (auto it{ m_ButtonLabels.find(BS_DEFAULT) }; it != std::end(m_ButtonLabels))
	{
		const auto& [key, value] { *it };
		if (!value.empty())
			return value;
	}

	return wxString();
}

wxString CNNBSSControlButton::GetCurrentHelpLabel() const
{
	if (auto it{ m_ButtonHelpLabels.find(m_ButtonCurrentState) }; it != std::end(m_ButtonHelpLabels))
	{
		const auto& [key, value] { *it };
		if (!value.empty())
			return value;
	}
	// If current state has no special, try to find the default
	if (auto it{ m_ButtonHelpLabels.find(BS_DEFAULT) }; it != std::end(m_ButtonHelpLabels))
	{
		const auto& [key, value] { *it };
		if (!value.empty())
			return value;
	}

	return wxString();
}

wxFont CNNBSSControlButton::GetCurrentFont() const
{
	if (auto it{ m_ButtonFonts.find(m_ButtonCurrentState) }; it != std::end(m_ButtonFonts))
	{
		const auto& [key, value] { *it };
		if (value.IsOk())
			return value;
	}
	// If current state has no special, try to find the default
	if (auto it{ m_ButtonFonts.find(BS_DEFAULT) }; it != std::end(m_ButtonFonts))
	{
		const auto& [key, value] { *it };
		if (value.IsOk())
			return value;
	}

	return wxFont();
}

wxBitmap CNNBSSControlButton::GetCurrentBitmap() const
{
	if (auto it{ m_ButtonBitmaps.find(m_ButtonCurrentState) }; it != std::end(m_ButtonBitmaps))
	{
		const auto& [key, value] { *it };
		if (value.IsOk())
			return value;
	}

	// If current state has no special, try to find the default
	if (auto it{ m_ButtonBitmaps.find(BS_DEFAULT) }; it != std::end(m_ButtonBitmaps))
	{
		const auto& [key, value] { *it };
		if (value.IsOk())
			return value;
	}

	return wxBitmap();
}

wxColour CNNBSSControlButton::GetCurrentTextColour() const
{
	if (auto it{ m_ButtonTextColours.find(m_ButtonCurrentState) }; it != std::end(m_ButtonTextColours))
	{
		const auto& [key, value] { *it };
		if (value.IsOk())
			return value;
	}
	return wxColour();
}

wxColour CNNBSSControlButton::GetCurrentBackgroundColour() const
{
	if (auto it{ m_ButtonBackgroundColours.find(m_ButtonCurrentState) }; it != std::end(m_ButtonBackgroundColours))
	{
		const auto& [key, value] { *it };
		if (value.IsOk())
			return value;
	}
	return wxColour();
}

long CNNBSSControlButton::GetCurrentAlignment() const
{
	if (auto it{ m_ButtonLabelAlignments.find(m_ButtonCurrentState) }; it != std::end(m_ButtonLabelAlignments))
	{
		const auto& [key, value] { *it };
		return value;
	}
	// If current state has no special, try to find the default
	if (auto it{ m_ButtonLabelAlignments.find(BS_DEFAULT) }; it != std::end(m_ButtonLabelAlignments))
	{
		const auto& [key, value] { *it };
		return value;
	}

	return m_DefaultAlignment;
}

int CNNBSSControlButton::GetCurrentRoundness() const
{
	if (auto it{ m_ButtonRoundness.find(m_ButtonCurrentState) }; it != std::end(m_ButtonRoundness))
	{
		const auto& [key, value] { *it };
		return value;
	}
	// If current state has no special, try to find the default
	if (auto it{ m_ButtonRoundness.find(BS_DEFAULT) }; it != std::end(m_ButtonRoundness))
	{
		const auto& [key, value] { *it };
		return value;
	}

	return 0;
}

wxFont CNNBSSControlButton::GetDefaultFont()
{
	return wxFont(*wxNORMAL_FONT);

	//return wxFont(*wxSMALL_FONT);
}

wxFont CNNBSSControlButton::GetItalicFont()
{
	return wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD);
}

wxFont CNNBSSControlButton::GetNormalFont()
{
	return wxFont(*wxNORMAL_FONT);
}

wxFont CNNBSSControlButton::GetBoldFont()
{
	return wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
}

void CNNBSSControlButton::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxBufferedPaintDC pDc(this);
	wxGCDC dc(pDc);
	wxGraphicsRenderer* const renderer = wxGraphicsRenderer::GetDefaultRenderer();
	wxGraphicsContext* context = renderer->CreateContext(pDc);
	dc.SetGraphicsContext(context);
	dc.GetGraphicsContext()->SetBrush(GetBackgroundColour());
	dc.GetGraphicsContext()->SetPen(*wxTRANSPARENT_PEN);

	// fill background
	dc.GetGraphicsContext()->DrawRectangle(0, 0, GetSize().x, GetSize().y);

	auto background_brush = wxBrush(GetBackgroundColour(), wxBRUSHSTYLE_SOLID);
	dc.SetBackground(background_brush);
	dc.Clear();

	bool OkBitmap = GetCurrentBitmap().IsOk(), OkLabel = !GetCurrentLabel().empty(), OkHelpLabel = !GetCurrentHelpLabel().empty();

	wxRect ButtonRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight());

	dc.SetPen(*wxTRANSPARENT_PEN);
	{
		// Button background fill
	
		// Default background colour is the colour of BS_DEFAULT/BS_IDLE for now
		// It means, if the button is rounded, the colour behind it will be what is set for BS_DEFAULT/BS_IDLE state background
		dc.SetBrush(wxBrush(GetButtonBackgroundColour(ButtonStates::BS_DEFAULT)));
		dc.GetGraphicsContext()->DrawRectangle(ButtonRectangle.x, ButtonRectangle.y, ButtonRectangle.width, ButtonRectangle.height);
		
		if (m_ButtonCurrentState == BS_IDLE)
		{

			dc.SetBrush(wxBrush(GetCurrentBackgroundColour().IsOk() ? GetCurrentBackgroundColour() : RGB_COLOURDEF_PLACEHOLDER));

			if (GetCurrentRoundness() <= 0)
			{
				dc.DrawRectangle(ButtonRectangle.x, ButtonRectangle.y, ButtonRectangle.width, ButtonRectangle.height);
			}
			else
			{
				dc.GetGraphicsContext()->DrawRoundedRectangle
				(ButtonRectangle.x, ButtonRectangle.y, ButtonRectangle.width, ButtonRectangle.height, GetCurrentRoundness());
			}
		}
		else if (m_ButtonCurrentState == BS_HOVER)
		{
			dc.SetBrush(wxBrush(GetCurrentBackgroundColour().IsOk() ? GetCurrentBackgroundColour() : RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT));
			if (GetCurrentRoundness() <= 0)
			{
				dc.DrawRectangle(ButtonRectangle.x, ButtonRectangle.y, ButtonRectangle.width, ButtonRectangle.height);
			}
			else
			{
				dc.GetGraphicsContext()->DrawRoundedRectangle(
					ButtonRectangle.x, ButtonRectangle.y, ButtonRectangle.width, ButtonRectangle.height, GetCurrentRoundness());
			}
		}
	}
	dc.SetBrush(*wxTRANSPARENT_BRUSH);

	{
		// Text Colour
		if (m_ButtonCurrentState == BS_IDLE)
			dc.SetTextForeground(GetCurrentTextColour().IsOk() ? GetCurrentTextColour() : (*wxWHITE));
		else if (m_ButtonCurrentState == BS_HOVER)
			dc.SetTextForeground(GetCurrentTextColour().IsOk() ? GetCurrentTextColour() : (*wxWHITE));
	}

	{
		// Text Font
		if (GetCurrentFont().IsOk())
			dc.SetFont(GetCurrentFont());
		else
			dc.SetFont(GetDefaultFont());
	}

	{
		wxRect BitmapPlace;
		if (OkBitmap)
		{
			BitmapPlace = ButtonRectangle;

			if (OkLabel || OkHelpLabel)// otherwise bitmap will be placed at the center of the button
				BitmapPlace.SetWidth(ButtonRectangle.GetWidth() / 3);// 1/3 area belongs to bitmap

			wxBitmap bitmap = GetCurrentBitmap();
			// TODO: what if bitmap is bigger than the place that's reserved for itself?
			const int bitmapXPos = BitmapPlace.x + ((BitmapPlace.width - bitmap.GetWidth()) / 2);
			const int bitmapYPos = BitmapPlace.y + ((BitmapPlace.height - bitmap.GetHeight()) / 2);
			dc.DrawBitmap(GetCurrentBitmap(), wxPoint(bitmapXPos, bitmapYPos));
		}

		wxRect LabelPlace;
		if (OkLabel)
		{
			if (OkBitmap)
			{
				LabelPlace.SetX(BitmapPlace.GetWidth());
			}

#if ___GET_TEXT_EXTEND___// No need for this actually(for now)
			LabelPlace.SetSize(dc.GetTextExtent(GetCurrentLabel()));
#else// This option is used to correct alignment(Like center label). 
			//GetTextExtend would be unnecessary for now, that's why it'sdeactivated with preprocessor block
			LabelPlace.SetWidth(ButtonRectangle.GetWidth() - BitmapPlace.GetWidth());
			LabelPlace.SetHeight(dc.GetTextExtent(GetCurrentLabel()).GetHeight());
#endif

			if (!OkHelpLabel)
				LabelPlace.SetY((ButtonRectangle.GetHeight() / 2) - (LabelPlace.GetHeight() / 2));
			else
				LabelPlace.SetY((ButtonRectangle.GetHeight() / 2) - (LabelPlace.GetHeight()));

			dc.DrawLabel(GetCurrentLabel(), LabelPlace, GetCurrentAlignment());
		}

		wxRect HelpLabelPlace;
		if (OkHelpLabel)
		{
			dc.SetFont(GetDefaultFont());// Set default font for Help Label

			if (OkBitmap)
			{
				HelpLabelPlace.SetX(BitmapPlace.GetWidth());
			}

			HelpLabelPlace.SetSize(dc.GetMultiLineTextExtent(GetCurrentHelpLabel()));

			if (!OkLabel)
			{
				HelpLabelPlace.SetY((ButtonRectangle.GetHeight() / 2) - (HelpLabelPlace.GetHeight() / 2));
			}
			else
			{
				HelpLabelPlace.SetY(LabelPlace.GetY() + LabelPlace.GetHeight());
			}

			dc.DrawLabel(GetCurrentHelpLabel(), HelpLabelPlace, GetCurrentAlignment());
		}

	}
}
void CNNBSSControlButton::OnMouseLeftDown(wxMouseEvent& event)
{
	event.Skip();
}
void CNNBSSControlButton::OnMouseLeftUp(wxMouseEvent& event)
{
	event.Skip();
}
void CNNBSSControlButton::OnMouseRightUp(wxMouseEvent& event)
{
	event.Skip();
}
void CNNBSSControlButton::OnMouseEnter(wxMouseEvent& event)
{
	SetMouseOver(true);

	m_ButtonCurrentState = BS_HOVER;

	Refresh();

	event.Skip();
}
void CNNBSSControlButton::OnMouseLeave(wxMouseEvent& event)
{
	SetMouseOver(false);

	if (!HasFocus())
		m_ButtonCurrentState = BS_IDLE;

	Refresh();

	event.Skip();
}
void CNNBSSControlButton::OnMouseMotion(wxMouseEvent& event)
{
	event.Skip();
}
void CNNBSSControlButton::OnMouseLost(wxMouseCaptureLostEvent& event)
{
	event.Skip();
}
void CNNBSSControlButton::OnKeyPressed(wxKeyEvent& event)
{
	Refresh();

	event.Skip();
}
void CNNBSSControlButton::OnKeyReleased(wxKeyEvent& event)
{
	if (!HasFocus() || !IsEnabled() || !IsShown() || !IsShownOnScreen())
	{
		event.Skip();
		return;
	}

	if (event.GetKeyCode() == 13 || event.GetKeyCode() == 32)// Enter or Space
	{
		wxCommandEvent simulateClick(wxEVT_LEFT_UP, m_windowId);
		simulateClick.SetId(GetId());
		simulateClick.SetEventObject(this);

		GetEventHandler()->ProcessEvent(simulateClick);
	}

	Refresh();

	event.Skip();
}
void CNNBSSControlButton::OnSetFocus(wxFocusEvent& event)
{
	m_ButtonCurrentState = BS_HOVER;

	Refresh();

	event.Skip();
}
void CNNBSSControlButton::OnKillFocus(wxFocusEvent& event)
{
	if (!GetMouseOver())
		m_ButtonCurrentState = BS_DEFAULT;

	Refresh();

	event.Skip();
}
void CNNBSSControlButton::OnResize(wxSizeEvent& event)
{
	Refresh();

	event.Skip();
}