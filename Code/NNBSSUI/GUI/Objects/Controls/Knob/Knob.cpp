/////////////////////////////////////////////////////////////////////////////
// Name:        Knob.cpp
// Purpose:     Knob Control
// Additional:  We assume it as it's a different looking slider. Does the same whatever controlSlider does.
// Author:      Orkhan Aliyev
// Created:     02-04-2022
// Copyright:   (c) Orkhan Aliyev	
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "Knob.h"
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

#include "Knob.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// Standard C Lib
#include <math.h>

constexpr float PI = 3.14159265358969323846;
#define irnd(x) (int (round (x)))

BEGIN_EVENT_TABLE(CNNBSSControlKnob, wxControl)
EVT_PAINT(CNNBSSControlKnob::OnPaint)
EVT_LEFT_DOWN(CNNBSSControlKnob::OnMouse)
EVT_LEFT_UP(CNNBSSControlKnob::OnMouse)
EVT_MOTION(CNNBSSControlKnob::OnMouse)
EVT_MOUSEWHEEL(CNNBSSControlKnob::OnMouseWheel)

EVT_KEY_DOWN(CNNBSSControlKnob::OnKeyPressed)
EVT_KEY_UP(CNNBSSControlKnob::OnKeyReleased)
EVT_SET_FOCUS(CNNBSSControlKnob::OnSetFocus)
EVT_KILL_FOCUS(CNNBSSControlKnob::OnKillFocus)
EVT_ERASE_BACKGROUND(CNNBSSControlKnob::OnEraseBackGround)
END_EVENT_TABLE()

// Constructor
void CNNBSSControlKnob::Create(wxWindow* parent, wxWindowID id, const int& value,
	const int& minValue, const int& maxValue,
	unsigned int minAngle, unsigned int range,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxValidator& validator,
	const wxString& name)
{
	wxControl::Create(parent, id);
	CacheBestSize(size);
	//    SetClientSize (size);

	m_minAngleNumber = minAngle;

	m_min = minValue;
	m_max = maxValue;
	range %= 360;
	minAngle %= 360;
	m_maxAngle = (minAngle + 360 - range) % 360;

	m_range = range;
	SetValue(value);

	SetWindowStyle(style | wxNO_BORDER);

	SetDoubleBuffered(true);// one of the methods used to avoid flickering
}

void CNNBSSControlKnob::SetRange(int minValue, int maxValue)
{
	if (minValue < maxValue)
	{
		m_min = minValue;
		m_max = maxValue;
		SetValue(m_setting);
	}
}

int CNNBSSControlKnob::SetValue(int value)
{
	if (value < m_min)
		value = m_min;
	if (value > m_max)
		value = m_max;

	if (value != m_setting)
	{
		m_setting = value;
		Refresh();
		Update();

	}
	return m_setting;
}

void CNNBSSControlKnob::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	int sizeX, sizeY, cx, cy, r, r2;
	double theta, dx, dy;
	wxSize s = GetSize();

	theta = (PI / 180.) * (m_maxAngle +
		(((double)m_max - m_setting) / (m_max - m_min))
		* m_range);
	dx = cos(theta);
	dy = -sin(theta);      // - because of upside down coordinates

	const int minTheta = (PI / 180.) * (m_maxAngle +
		(((double)m_max - m_min) / (m_max - m_min))
		* m_range),
		minPX = cos(minTheta), minPY = -sin(minTheta);

	GetCenter(cx, cy);
	r = irnd(((s.x < s.y) ? s.x : s.y) * .48);
	r2 = irnd(r * .6);

	//{
	//wxClientDC cDC(this);
	//wxBufferedDC bDc(&cDC, GetSize());
	//wxGCDC gcdc(bDc);
	//} REDRAW

	wxBufferedPaintDC pDc(this);
	wxGCDC gcdc(pDc);
	wxGraphicsRenderer* const renderer = wxGraphicsRenderer::GetDefaultRenderer();
	wxGraphicsContext* context = renderer->CreateContext(pDc);
	gcdc.SetGraphicsContext(context);
	gcdc.GetGraphicsContext()->SetBrush(GetBackgroundColour());
	gcdc.GetGraphicsContext()->SetPen(*wxTRANSPARENT_PEN);

	// fill background
	gcdc.GetGraphicsContext()->DrawRectangle(0, 0, GetSize().x, GetSize().y);

	//----------------------------------------------------------------------

	const wxColour HighlightColour = IsEnabled() ? RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT : RGB_COLOURDEF_TRIPLE35;

	// Draw background circle
	const int indicatorCircleThickness = 2;
	gcdc.GetGraphicsContext()->SetPen(wxPen(RGB_COLOURDEF_TRIPLE35, indicatorCircleThickness));
	gcdc.DrawCircle(cx, cy, r);


	// Draw Arc
	if (GetValue() != GetMin() && IsEnabled())
	{
		gcdc.GetGraphicsContext()->SetPen(wxPen(HighlightColour, indicatorCircleThickness));
		const int minAngle = m_minAngleNumber;
		const int sX = cx + r * cos(minAngle * PI / 180),
			sY = cy + r * (-sin(minAngle * PI / 180));
		gcdc.DrawArc(wxPoint(cx + irnd(r * dx), cy + irnd(r * dy)),
			wxPoint(sX, sY), wxPoint(cx, cy));
	}

	// Draw filled circle to erase arc inside
	gcdc.GetGraphicsContext()->SetPen(*wxTRANSPARENT_PEN);
	gcdc.DrawCircle(cx, cy, r - (indicatorCircleThickness - 1));

	// Indicator point circle
	gcdc.GetGraphicsContext()->SetPen(wxPen(HighlightColour, indicatorCircleThickness));
	gcdc.DrawCircle(wxPoint(cx + irnd(r * 0.8 * dx), cy + irnd(r * 0.8 * dy)), 3);
	/*gcdc.DrawLine (cx + irnd (r * dx), cy + irnd (r * dy),
				 cx + irnd (r2 * dx), cy + irnd (r2 * dy));*/

	gcdc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	const wxString valueString = wxString::Format("%i", GetValue());
	const wxSize TextExtent(gcdc.GetTextExtent(valueString));
	const wxRect boundingBox(cx - r, cy - r, (cx - r) + (cx + r), (cy - r) + (cy + r));

	if (HasFocus())
	{
		gcdc.GetGraphicsContext()->SetBrush(HighlightColour);
		gcdc.DrawCircle(wxPoint(cx + irnd(r * 0.8 * dx), cy + irnd(r * 0.8 * dy)), 1);
	}

	gcdc.SetTextForeground(HighlightColour);
	gcdc.DrawText(valueString, wxPoint((boundingBox.width / 2) - (TextExtent.GetWidth() / 2),
		(boundingBox.height / 2) - (TextExtent.GetHeight() / 2)));
}

void CNNBSSControlKnob::OnMouse(wxMouseEvent& event)
{
	if (event.AltDown() && (event.ButtonDown() || event.ButtonUp()))
	{
		SetValue(m_defaultValue);
		event.Skip();
		return;
	}

	int cx, cy;
	double dx, dy, theta, dt;
	int newval;
	wxEventType scrollEvent = wxEVT_NULL;

	if (event.Moving())
	{
		event.Skip();
		return;
	}

	GetCenter(cx, cy);
	dx = event.m_x - cx;
	dy = cy - event.m_y;
	if (dx == 0. && dy == 0.)
		return;

	theta = atan2(dy, dx) * 180. / PI;
	if (theta < 0.)
		theta += 360.;

	dt = theta - m_maxAngle;
	if (dt < 0.)
		dt += 360;
	if (dt > m_range)
		return;
	newval = int(m_max - (dt / m_range) * (m_max - m_min));

	SetValue(newval);
	if (event.Dragging() || event.ButtonUp())
	{
		if (event.ButtonUp())
		{
			scrollEvent = wxEVT_SCROLL_THUMBRELEASE;
		}
		else
			scrollEvent = wxEVT_SCROLL_THUMBTRACK;

		wxScrollEvent event(scrollEvent, m_windowId);
		event.SetPosition(newval);
		event.SetEventObject(this);
		GetEventHandler()->ProcessEvent(event);

		wxCommandEvent cevent(wxEVT_COMMAND_SLIDER_UPDATED, m_windowId);
		cevent.SetInt(newval);
		cevent.SetEventObject(this);
		GetEventHandler()->ProcessEvent(cevent);

		if (!HasFocus())// Is there any problem with this? Have not tested, yet.
			SetFocus();
	}
}

void CNNBSSControlKnob::OnMouseWheel(wxMouseEvent& event)
{
	int m_wheelRotation = 0;

	m_wheelRotation += event.GetWheelRotation();
	int lines = m_wheelRotation / event.GetWheelDelta();
	//m_wheelRotation -= lines * event.GetWheelDelta();

	SetValue(std::clamp(GetValue() + lines, GetMin(), GetMax()));

	{
		wxScrollEvent event(wxEVT_SCROLL_THUMBRELEASE, m_windowId);
		event.SetPosition(GetValue());
		event.SetEventObject(this);
		GetEventHandler()->ProcessEvent(event);

		wxCommandEvent cevent(wxEVT_COMMAND_SLIDER_UPDATED, m_windowId);
		cevent.SetInt(GetValue());
		cevent.SetEventObject(this);
		GetEventHandler()->ProcessEvent(cevent);
	}

	event.Skip();
}

void CNNBSSControlKnob::OnKeyPressed(wxKeyEvent& event)
{
	if (!HasFocus() || !IsEnabled())
	{
		event.Skip();
		return;
	}

	if (event.GetKeyCode() == WXK_NUMPAD6)
	{
		SetValue(std::clamp(GetValue() + 1, GetMin(), GetMax()));
	}
	else if (event.GetKeyCode() == WXK_NUMPAD4)
	{
		SetValue(std::clamp(GetValue() + -1, GetMin(), GetMax()));
	}

	{
		wxScrollEvent event(wxEVT_SCROLL_THUMBTRACK, m_windowId);
		event.SetPosition(GetValue());
		event.SetEventObject(this);
		GetEventHandler()->ProcessEvent(event);

		wxCommandEvent cevent(wxEVT_COMMAND_SLIDER_UPDATED, m_windowId);
		cevent.SetInt(GetValue());
		cevent.SetEventObject(this);
		GetEventHandler()->ProcessEvent(cevent);
	}

	Refresh();
	event.Skip();
}

void CNNBSSControlKnob::OnKeyReleased(wxKeyEvent& event)
{
	{
		wxScrollEvent event(wxEVT_SCROLL_THUMBRELEASE, m_windowId);
		event.SetPosition(GetValue());
		event.SetEventObject(this);
		GetEventHandler()->ProcessEvent(event);

		wxCommandEvent cevent(wxEVT_COMMAND_SLIDER_UPDATED, m_windowId);
		cevent.SetInt(GetValue());
		cevent.SetEventObject(this);
		GetEventHandler()->ProcessEvent(cevent);
	}

	event.Skip();
}

void CNNBSSControlKnob::OnSetFocus(wxFocusEvent& event)
{
	Refresh();
	event.Skip();
}

void CNNBSSControlKnob::OnKillFocus(wxFocusEvent& event)
{
	Refresh();
	event.Skip();
}

bool CNNBSSControlKnob::Enable(bool enable)
{
	bool val = wxWindowBase::Enable(enable);
	Refresh();
	return val;
}

void CNNBSSControlKnob::GetCenter(int& x, int& y) const
{
	wxSize s = GetSize();
	x = s.x / 2;
	y = s.y / 2;
}