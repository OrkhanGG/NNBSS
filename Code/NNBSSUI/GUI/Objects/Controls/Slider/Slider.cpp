/////////////////////////////////////////////////////////////////////////////
// Name:        Button.cpp
// Purpose:     Button Control
// Author:      Orkhan Aliyev
// Created:     02-04-2022
// Copyright:   (c) Orkhan Aliyev	
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "Slider.h"
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
#include <wx/dcbuffer.h>
#include "wx/dcclient.h"
#endif // WX_PRECOMP
#include <wx/dcgraph.h>

#include "Slider.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

float fraction_to_value(const float& fraction, const int& min_value, const int& max_value)
{
	return (max_value - min_value) * fraction + min_value;
}

float value_to_fraction(const float& value, const int& min_value, const int& max_value)
{
	return float(value - min_value) / (max_value - min_value);
}

BEGIN_EVENT_TABLE(CNNBSSControlSlider, wxWindow)
EVT_ERASE_BACKGROUND(CNNBSSControlSlider::OnEraseBackground)
EVT_LEFT_UP(CNNBSSControlSlider::OnMouseUp)
EVT_LEFT_DOWN(CNNBSSControlSlider::OnMouseDown)
EVT_MOTION(CNNBSSControlSlider::OnMouseMotion)
EVT_MOUSE_CAPTURE_LOST(CNNBSSControlSlider::OnMouseLost)
EVT_ENTER_WINDOW(CNNBSSControlSlider::OnMouseEnter)
EVT_LEAVE_WINDOW(CNNBSSControlSlider::OnMouseLeave)
EVT_RIGHT_UP(CNNBSSControlSlider::OnMouseRightUp)
EVT_MOUSEWHEEL(CNNBSSControlSlider::OnMouseWheel)
EVT_KEY_DOWN(CNNBSSControlSlider::OnKeyPressed)
EVT_KEY_UP(CNNBSSControlSlider::OnKeyReleased)
EVT_SET_FOCUS(CNNBSSControlSlider::OnSetFocus)
EVT_KILL_FOCUS(CNNBSSControlSlider::OnKillFocus)
EVT_PAINT(CNNBSSControlSlider::PaintEvent)
EVT_SIZE(CNNBSSControlSlider::OnResize)
END_EVENT_TABLE()

CNNBSSControlSlider::CNNBSSControlSlider(wxWindow* parent, wxWindowID id, const int& sliderType,
	const long& highValue, 
	const long& lowValue,
	const long& min, const long& max,
	const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	// Initializer list
	: wxWindow(parent, id, pos, size, style | wxBORDER_NONE | wxFULL_REPAINT_ON_RESIZE),
	m_slider_type(sliderType), m_defaultHighValue(highValue), m_defaultLowValue(lowValue), min_value(min), max_value(max),

	/*colours*/slider_background_color(RGB_COLOURDEF_TRIPLE20), slider_outline(RGB_COLOURDEF_PLACEHOLDER),
	selected_range_color(RGB_COLOURDEF_NNBSSBLUE), selected_range_outline(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT)
{
	SetDoubleBuffered(true);

	SetMinSize(wxSize(std::max({ 50, size.GetX() }), std::max({ 26, size.GetY() })));

	{
		// Thumbs
		thumbs[0] = SliderThumb(*this, min_value, ThumbType::LOW);
		thumbs[1] = SliderThumb(*this, min_value, ThumbType::HIGH);

		if (GetSliderType() != SliderType::MONO)
		{
			thumbs[0].SetVisible(true); // low indicator thumb will only be visible with RANGE_BBASED slider type
		}
		thumbs[1].SetVisible(true);
	}

	{
		// Set Tooltip for mouse/keyboard control
		const wxString m_tooltipAddition = _("Use mouse wheel or Numpad4/Numpad6 to change the value");
		const wxString m_tooltipKeyCombInfo = _("Use Alt + Numpad4/Numpad6 to change highest value.");
		if (GetToolTip())
		{
			if (GetToolTip()->GetTip().empty())
			{
				if (GetSliderType() == SliderType::MONO)
					SetToolTip(m_tooltipAddition);
				else
					SetToolTip(wxString::Format("%s\n%s", m_tooltipAddition, m_tooltipKeyCombInfo));
			}
			else
			{
				if (GetSliderType() == SliderType::MONO)
					SetToolTip(wxString::Format("%s\n%s", GetToolTip()->GetTip(), m_tooltipAddition));
				else
					SetToolTip(wxString::Format("%s\n%s\n%s", GetToolTip()->GetTip(), m_tooltipAddition, m_tooltipKeyCombInfo));
			}
		}
		else
		{
			if (GetSliderType() == SliderType::MONO)
				SetToolTip(m_tooltipAddition);
			else
				SetToolTip(wxString::Format("%s\n%s", m_tooltipAddition, m_tooltipKeyCombInfo));
		}
	}

	// Init values
	SetValue(m_defaultLowValue, ThumbType::LOW);
	SetValue(m_defaultHighValue, ThumbType::HIGH);
}

bool CNNBSSControlSlider::Enable(bool enable)
{
	bool val = wxWindowBase::Enable(enable);
	Refresh();
	return val;
}

void CNNBSSControlSlider::Reset()
{
	if (GetSliderType() == SliderType::MONO)
	{
		SetValue(0, ThumbType::HIGH);
	}
	else
	{
		SetValue(0, ThumbType::LOW);
		SetValue(1, ThumbType::HIGH);
	}
	SetMin(0);
	SetMax(1);

	Refresh();
}

void CNNBSSControlSlider::PaintEvent(wxPaintEvent& event)
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

	Render(dc);

	event.Skip();
}

void CNNBSSControlSlider::Render(wxGCDC& dc)
{
	const wxBrush background_brush = wxBrush(GetBackgroundColour(), wxBRUSHSTYLE_SOLID);
	dc.SetBackground(background_brush);
	dc.Clear();

	if (!m_noValueIndicator)
	{
		dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

		dc.SetTextForeground(RGB_COLOURDEF_WHITE);
		{
			// left side text
			wxString leftValueAS = wxString::Format("%i", GetValue(GetSliderType() == CNNBSSControlSlider::SliderType::MONO ? CNNBSSControlSlider::ThumbType::HIGH : CNNBSSControlSlider::ThumbType::LOW));
			wxRect l_textRect = wxRect(0, 0, dc.GetTextExtent(leftValueAS).GetWidth(), GetSize().GetHeight());
			left_border_width = 4 + l_textRect.GetWidth();
			dc.DrawLabel(leftValueAS, l_textRect, wxALIGN_CENTER);
		}

		if (GetSliderType() == CNNBSSControlSlider::SliderType::RANGE_BASED)
		{
			// right side text
			constexpr static int _littleSpace = 4;
			wxString rightValueAS = wxString::Format("%i", GetValue(GetSliderType() == CNNBSSControlSlider::SliderType::MONO ? CNNBSSControlSlider::ThumbType::HIGH : CNNBSSControlSlider::ThumbType::HIGH));
			right_border_width = _littleSpace + dc.GetTextExtent(rightValueAS).GetWidth();
			wxRect r_textRect = wxRect(GetSize().x - (right_border_width - _littleSpace), 0, dc.GetTextExtent(rightValueAS).GetWidth(), GetSize().GetHeight());
			dc.DrawLabel(rightValueAS, r_textRect, wxALIGN_CENTER);
		}
	}

	const int track_height = 12;
	const int w = GetSize().GetX();
	const int h = GetSize().GetY();

	dc.SetPen(wxPen(slider_outline, 1, wxPENSTYLE_SOLID));
	dc.SetBrush(wxBrush(slider_background_color, wxBRUSHSTYLE_SOLID));
	dc.DrawRectangle(left_border_width, (h - track_height) / 2, w - left_border_width - right_border_width, track_height);

	if (IsEnabled())
	{
		dc.SetPen(wxPen(selected_range_outline, 1, wxPENSTYLE_SOLID));
		dc.SetBrush(wxBrush(selected_range_color, wxBRUSHSTYLE_SOLID));
	}
	else
	{
		dc.SetPen(wxPen(slider_outline, 1, wxPENSTYLE_SOLID));
		dc.SetBrush(wxBrush(slider_outline, wxBRUSHSTYLE_SOLID));
	}

	const int low_thumb_pos = thumbs[0].GetPosition().x;
	const int high_thumb_pos = thumbs[1].GetPosition().x;
	// Draw bar
	if (GetSliderType() == SliderType::RANGE_BASED)
		dc.DrawRectangle(low_thumb_pos, h / 2 - track_height / 4, high_thumb_pos - low_thumb_pos, track_height / 2);
	else
		dc.DrawRectangle(left_border_width, h / 2 - track_height / 4, high_thumb_pos - low_thumb_pos, track_height / 2);

	if (!m_noThumb)// if thumbs are allowed to be shown
		for (auto& thumb : thumbs)
		{
			thumb.Render(dc);
		}

	// Draw focus border
	//if (HasFocus() && IsEnabled())
	//{
	//	// focus rectangle
	//	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	//	dc.DrawRectangle(0, 0, GetSize().x, GetSize().y);
	//}
}

void CNNBSSControlSlider::OnMouseDown(wxMouseEvent& event)
{

	if (!IsEnabled())
	{
		return;
	}

	const wxPoint click_pos = event.GetPosition();
	for (auto& thumb : thumbs)
	{
		if (thumb.IsMouseOver(click_pos))
		{
			selected_thumb = &thumb;
			break;
		}
	}

	CaptureMouse();
	Refresh();
}

void CNNBSSControlSlider::OnMouseUp(wxMouseEvent& event)
{

	if (!IsEnabled())
	{
		return;
	}

	selected_thumb = nullptr;

	if (HasCapture())
	{
		ReleaseMouse();
	}

	if (!selected_thumb && GetSliderType() == SliderType::MONO)
	{
		selected_thumb = &thumbs[1];// hardcoded

		// If alt is pressed, reset value and don't let the value to be set something else than default
		if (event.AltDown() && (event.ButtonDown() || event.ButtonUp()))
		{
			selected_thumb = nullptr;

			SetValue(m_defaultHighValue, ThumbType::HIGH);
			event.Skip();
			return;
		}

		// Only for mono typed sliders: set position on click
		thumbs[1].SetPosition(event.GetPosition());
	}
	else if (!selected_thumb && GetSliderType() == SliderType::RANGE_BASED)
	{
		const wxPoint click_pos = event.GetPosition();
		for (auto& thumb : thumbs)
		{
			if (thumb.IsMouseOver(click_pos))
			{
				selected_thumb = &thumb;
				break;
			}
		}
	}

	// If Alt pressed, reset value for selected thumb(only for range based; look above for mono typed sliders)
	if (selected_thumb != nullptr)
		if (event.AltDown() && (event.ButtonDown() || event.ButtonUp()))
		{
			SetValue(selected_thumb->GetType() == ThumbType::LOW ? m_defaultLowValue : m_defaultHighValue, selected_thumb->GetType());

			// importantly, this should be set to nullptr!
			selected_thumb = nullptr;

			event.Skip();
			return;
		}

	if (selected_thumb && GetSliderType() == SliderType::MONO)
	{
		// just send one thumb value
		CNNBSSEventsControlSlider onChangeEvent(EVT_CONTROLSLIDER_ON_CHANGE);

		onChangeEvent.SetSliderType(SliderType::MONO);
		onChangeEvent.SetThumbType(ThumbType::HIGH);
		onChangeEvent.SetSliderValue(selected_thumb->GetThumbValue());

		wxPostEvent(this, onChangeEvent);
	}
	else if (selected_thumb && GetSliderType() == SliderType::RANGE_BASED)
	{
		CNNBSSEventsControlSlider onChangeEvent(EVT_CONTROLSLIDER_ON_CHANGE);

		onChangeEvent.SetSliderType(SliderType::RANGE_BASED);

		if (selected_thumb->GetType() == ThumbType::LOW)
			onChangeEvent.SetThumbType(ThumbType::LOW);
		else
			onChangeEvent.SetThumbType(ThumbType::HIGH);

		onChangeEvent.SetSliderValue(selected_thumb->GetThumbValue());

		wxPostEvent(this, onChangeEvent);
	}

	if (!HasFocus())// Is there any problem with this? Have not tested, yet.
		SetFocus();

	selected_thumb = nullptr;

	Refresh();
}

void CNNBSSControlSlider::OnMouseRightUp(wxMouseEvent& event)
{
	event.Skip();
}

void CNNBSSControlSlider::OnMouseLost(wxMouseCaptureLostEvent& event)
{
	event.Skip();
}

void CNNBSSControlSlider::OnMouseMotion(wxMouseEvent& event)
{

	if (!IsEnabled())
	{
		return;
	}

	bool refresh_needed = false;
	wxPoint mouse_position = event.GetPosition();

	if (selected_thumb)
	{
#if 0
		if (GetSliderType() != SliderType::MONO)
			if (selected_thumb->GetType() == ThumbType::LOW)
			{
				if (mouse_position.x + selected_thumb->GetSize().x > thumbs[1].GetPosition().x)
				{
					// No need for this(?)
					// Then Mouse position equals the last known good thumb pos.
					//mouse_position = selected_thumb->GetPosition();
				}
			}
			else
			{
				if (mouse_position.x - selected_thumb->GetSize().x < thumbs[0].GetPosition().x)
				{
					// Then Mouse position equals the last known good thumb pos.
					mouse_position = selected_thumb->GetPosition();
}
			}
#endif// TODO: Remove if is not needed
		//-----------------

		selected_thumb->SetPosition(mouse_position);

		refresh_needed = true;
	}
	else
	{

		for (auto& thumb : thumbs)
		{
			bool old_mouse_over = thumb.mouse_over;
			thumb.mouse_over = thumb.IsMouseOver(mouse_position);
			if (old_mouse_over != thumb.mouse_over)
			{
				refresh_needed = true;
			}
		}
	}

	if (refresh_needed)
	{
		Refresh();
	}

}

void CNNBSSControlSlider::OnMouseEnter(wxMouseEvent& event)
{
	event.Skip();
}

void CNNBSSControlSlider::OnMouseLeave(wxMouseEvent& event)
{
	if (!IsEnabled())
	{
		return;
	}

	for (auto& thumb : thumbs)
	{
		thumb.mouse_over = false;
	}

	Refresh();

}

void CNNBSSControlSlider::OnMouseWheel(wxMouseEvent& event)
{
	int m_wheelRotation = 0;

	m_wheelRotation += event.GetWheelRotation();
	int lines = m_wheelRotation / event.GetWheelDelta();
	//m_wheelRotation -= lines * event.GetWheelDelta();

	if (GetSliderType() == SliderType::MONO)
		SetValue(std::clamp((long long)thumbs[1].GetThumbValue() + lines, GetMinValue(), GetMaxValue()), ThumbType::HIGH);
	else
	{
		SetValue(std::clamp((long long)thumbs[event.AltDown() ? 1 : 0].GetThumbValue() + lines, GetMinValue(), GetMaxValue()),
			event.AltDown() ? ThumbType::HIGH : ThumbType::LOW);
	}

	event.Skip();
}

void CNNBSSControlSlider::OnKeyPressed(wxKeyEvent& event)
{
	if (!HasFocus() || !IsEnabled())
	{
		event.Skip();
		return;
	}

	bool m_reqKeyPressed = event.AltDown();

	bool m_mono = false;
	if (GetSliderType() == SliderType::MONO)
	{
		m_mono = true;
	}

	if (event.GetKeyCode() == WXK_NUMPAD6)
	{
		if (m_mono)
		{
			SetValue(std::clamp((long long)thumbs[1].GetThumbValue() + 1, GetMinValue(), GetMaxValue()), ThumbType::HIGH);
		}
		else
		{
			if (m_reqKeyPressed)
				SetValue(std::clamp((long long)thumbs[1].GetThumbValue() + 1, (long long)thumbs[0].GetThumbValue() + 1/*SHOULD NOT OVERLAP*/, GetMaxValue()), ThumbType::HIGH);
			else
				SetValue(std::clamp((long long)thumbs[0].GetThumbValue() + 1, GetMinValue(), (long long)thumbs[1].GetThumbValue() - 1/*SHOULD NOT OVERLAP*/), ThumbType::LOW);
		}
	}
	else if (event.GetKeyCode() == WXK_NUMPAD4)
	{
		if (m_mono)
		{
			SetValue(std::clamp((long long)thumbs[1].GetThumbValue() - 1, GetMinValue(), GetMaxValue()), ThumbType::HIGH);
		}
		else
		{
			if (m_reqKeyPressed)
				SetValue(std::clamp((long long)thumbs[1].GetThumbValue() - 1, (long long)thumbs[0].GetThumbValue() + 1/*SHOULD NOT OVERLAP*/, GetMaxValue()), ThumbType::HIGH);
			else
				SetValue(std::clamp((long long)thumbs[0].GetThumbValue() - 1, GetMinValue(), (long long)thumbs[1].GetThumbValue() - 1/*SHOULD NOT OVERLAP*/), ThumbType::LOW);
		}
	}
	Refresh();
	event.Skip();
}

void CNNBSSControlSlider::OnKeyReleased(wxKeyEvent& event)
{
	Refresh();
	event.Skip();
}

void CNNBSSControlSlider::OnSetFocus(wxFocusEvent& event)
{
	Refresh();
	event.Skip();
}

void CNNBSSControlSlider::OnKillFocus(wxFocusEvent& event)
{
	Refresh();
	event.Skip();
}

void CNNBSSControlSlider::OnResize(wxSizeEvent& event)
{
	Refresh();
}

int CNNBSSControlSlider::GetValue(const ThumbType& thumb) const
{
	return thumbs[thumb == ThumbType::LOW ? 0 : 1].GetThumbValue();
}

void CNNBSSControlSlider::SetValue(const long& value, const ThumbType& thumb)
{
	for (auto& p : thumbs)
		if (p.GetType() == thumb)
			p.SetValue(value);
}

//----------------------------------------------------------------------------------

CNNBSSControlSlider::SliderThumb::SliderThumb(CNNBSSControlSlider& parent, const int& value, const ThumbType& type) :
	parent_(&parent), value_(value), visible(false), type_(type),

	/*colours*/ idle_color(RGB_COLOURDEF_NNBSSBLUE), idle_shadow(RGB_COLOURDEF_PLACEHOLDER), dragged_color(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT),
	dragged_shadow(RGB_COLOURDEF_PLACEHOLDER), mouse_over_color(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT), mouse_over_shadow(RGB_COLOURDEF_PLACEHOLDER)
{
	size_ = wxSize(10, 18);
}

void CNNBSSControlSlider::SliderThumb::Render(wxGCDC& dc)
{
	if (!visible)
		return;

	wxColour thumb_color;
	wxColour thumb_shadow_color;
	if (!parent_->IsEnabled())
	{
		thumb_color = dragged_color;
		thumb_shadow_color = dragged_shadow;
	}
	else if (mouse_over)
	{
		thumb_color = mouse_over_color;
		thumb_shadow_color = mouse_over_shadow;
	}
	else
	{
		thumb_color = idle_color;
		thumb_shadow_color = idle_shadow;
	}

	const wxPoint thumb_pos = GetPosition();

	dc.SetBrush(wxBrush(thumb_shadow_color, wxBRUSHSTYLE_SOLID));
	dc.SetPen(wxPen(thumb_shadow_color, 1, wxPENSTYLE_SOLID));

	// Draw shadow
	if (!parent_->HasFocus())
		dc.DrawPolygon(WXSIZEOF(thumb_shadow_poly), thumb_shadow_poly, thumb_pos.x - size_.x / 2, thumb_pos.y - size_.y / 2);
	else
		dc.DrawCircle(wxPoint((thumb_pos.x - size_.x / 2) + 2, (thumb_pos.y - size_.y / 2) + 7), 6);

	if (parent_->IsEnabled())
	{
		dc.SetBrush(wxBrush(thumb_color, wxBRUSHSTYLE_SOLID));
		dc.SetPen(wxPen(thumb_color, 1, wxPENSTYLE_SOLID));
	}

	// Draw differently if focused
	if (!parent_->HasFocus())
		dc.DrawPolygon(WXSIZEOF(thumb_poly), thumb_poly, thumb_pos.x - size_.x / 2, thumb_pos.y - size_.y / 2);
	else
		dc.DrawCircle(wxPoint((thumb_pos.x - size_.x / 2) + 2, (thumb_pos.y - size_.y / 2) + 7), 6);
}

wxPoint CNNBSSControlSlider::SliderThumb::GetPosition()
{
	const long long min_x = GetMin();
	const long long max_x = GetMax();
	const wxSize parent_size = parent_->GetSize();
	const long long min_value = parent_->GetMinValue();
	const long long max_value = parent_->GetMaxValue();
	const float fraction = value_to_fraction(value_, min_value, max_value);

	const wxPoint pos(fraction_to_value(fraction, min_x, max_x), parent_size.GetY() / 2 + 1);

	return pos;
}

bool CNNBSSControlSlider::SliderThumb::IsMouseOver(const wxPoint& click_pos)
{
	if (!visible)
		return false;

	const wxPoint pos = GetPosition();

	const int boundary_low = pos.x - size_.x / 2;
	const int boundary_high = pos.x + size_.x / 2;
	mouse_over = (click_pos.x >= boundary_low && click_pos.x <= boundary_high);

	return mouse_over;
}

void CNNBSSControlSlider::SliderThumb::SetPosition(const wxPoint& mouse_position)
{
	if (parent_->GetSliderType() == SliderType::RANGE_BASED)
	{
		SliderThumb lowT = parent_->thumbs[0];
		SliderThumb highT = parent_->thumbs[1];

		if (type_ == ThumbType::HIGH)
		{
			if ((mouse_position.x - lowT.GetPosition().x) <= size_.x)
			{
				return;
			}
		}
		else
		{
			// if we're dragging low thumb
			if ((highT.GetPosition().x - mouse_position.x) <= size_.x)
			{
				return;
			}
		}
	}
	int pos_x = mouse_position.x;
	pos_x = std::min(std::max(pos_x, GetMin()), GetMax());
	const float fraction = value_to_fraction(pos_x, GetMin(), GetMax());
	value_ = fraction_to_value(fraction, parent_->GetMinValue(), parent_->GetMaxValue());
}

void CNNBSSControlSlider::SliderThumb::SetValue(const long& value)
{
	/*const int currentPosX = parent_->GetScreenPosition().x + GetPosition().x;
	const int currentPosY = parent_->GetScreenPosition().y + GetPosition().y;*/

	//SetCursorPos(currentPosX,currentPosY);

	if (parent_->GetSliderType() == SliderType::RANGE_BASED)
	{
		SliderThumb lowT = parent_->thumbs[0];
		SliderThumb highT = parent_->thumbs[1];

		if (type_ == ThumbType::HIGH)
		{
			if (value < value_)
				if (highT.GetPosition().x - (size_.x / 2) <= lowT.GetPosition().x + (size_.x / 2))
				{
					return;
				}
		}
		else
		{
			if (value > value_)
				if (lowT.GetPosition().x + (size_.x / 2) >= highT.GetPosition().x - (size_.x / 2))
				{
					return;
				}
		}
	}

	value_ = value;

	parent_->Refresh();
}

int CNNBSSControlSlider::SliderThumb::GetMin()
{
	if (parent_->m_noThumb)
		return parent_->left_border_width - size_.x / 2;
	else
	{
		// is not different but i keep so we may change our mind
		if (parent_->GetSliderType() == SliderType::MONO)
			return parent_->left_border_width + size_.x / 2;
		else
			return parent_->left_border_width + size_.x / 2;
	}
}

int CNNBSSControlSlider::SliderThumb::GetMax()
{
	if (parent_->m_noThumb)
		return (parent_->GetSize().GetX() + (size_.x / 2) - 1);
	else
	{
		if (parent_->GetSliderType() == SliderType::MONO)
			return (parent_->GetSize().GetX() - (size_.x / 2));
		else
			return ((parent_->GetSize().GetX() - (size_.x / 2)) - parent_->right_border_width);
	}
}