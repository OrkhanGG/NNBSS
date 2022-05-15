#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>

#include "CheckBox.h"

BEGIN_EVENT_TABLE(CNNBSSControlCheckBox, wxControl)
EVT_PAINT(CNNBSSControlCheckBox::OnPaint)
EVT_LEFT_DOWN(CNNBSSControlCheckBox::OnMouse)
EVT_LEFT_UP(CNNBSSControlCheckBox::OnMouse)
EVT_MOTION(CNNBSSControlCheckBox::OnMouse)

EVT_KEY_DOWN(CNNBSSControlCheckBox::OnKeyPressed)
EVT_KEY_UP(CNNBSSControlCheckBox::OnKeyReleased)
EVT_SET_FOCUS(CNNBSSControlCheckBox::OnSetFocus)
EVT_KILL_FOCUS(CNNBSSControlCheckBox::OnKillFocus)
EVT_ERASE_BACKGROUND(CNNBSSControlCheckBox::OnEraseBackGround)

EVT_SIZE(CNNBSSControlCheckBox::OnSize)
END_EVENT_TABLE()

void CNNBSSControlCheckBox::_Create(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
{
	wxWindow::Create(parent, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE);
	m_label = label;

	SetSize(size);

	SetWindowStyle(style | wxNO_BORDER);

	SetDoubleBuffered(true);// one of the methods used to avoid flickering
}

void CNNBSSControlCheckBox::Check(bool check)
{
	m_checked = check;

	wxScrollEvent event(wxEVT_CHECKBOX, m_windowId);
	event.SetPosition(GetValue());
	event.SetEventObject(this);
	GetEventHandler()->ProcessEvent(event);
}

void CNNBSSControlCheckBox::CheckSilent(bool check)
{
	m_checked = check;
}

bool CNNBSSControlCheckBox::Enable(bool enable)
{
	bool val = wxWindowBase::Enable(enable);
	Refresh();
	return val;
}

void CNNBSSControlCheckBox::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC pDc(this);
	wxGCDC gcdc(pDc);
	wxGraphicsRenderer* const renderer = wxGraphicsRenderer::GetDefaultRenderer();
	wxGraphicsContext* context = renderer->CreateContext(pDc);
	gcdc.SetGraphicsContext(context);
	gcdc.GetGraphicsContext()->SetBrush(GetBackgroundColour());
	gcdc.GetGraphicsContext()->SetPen(*wxTRANSPARENT_PEN);

	// fill background
	gcdc.GetGraphicsContext()->DrawRectangle(0, 0, GetSize().x, GetSize().y);

	//---------------------------------------------------------------------------
	// draw check box
	gcdc.GetGraphicsContext()->SetBrush(RGB_COLOURDEF_PLACEHOLDER);
	const wxRect checkBoxRect(0, 0, 16, 16);
	gcdc.GetGraphicsContext()->DrawRoundedRectangle(checkBoxRect.x, checkBoxRect.y, checkBoxRect.width, checkBoxRect.height, 5);
	if (m_checked)
	{
		const wxRect markRect = checkBoxRect.Deflate(2, 2);
		gcdc.GetGraphicsContext()->SetBrush(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
		gcdc.GetGraphicsContext()->SetPen(*wxTRANSPARENT_PEN);
		gcdc.GetGraphicsContext()->DrawRoundedRectangle(markRect.x, markRect.y, markRect.width, markRect.height, 2);
	}

	// draw label
	if (HasFocus())
	{
		/*Draw focus Border*/
		gcdc.GetGraphicsContext()->SetPen(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
		 gcdc.DrawRoundedRectangle(checkBoxRect.x, checkBoxRect.y, checkBoxRect.width, checkBoxRect.height, 5);

		// Change colour if has focus
		gcdc.SetTextForeground(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	}
	else
	{
		gcdc.SetTextForeground(GetForegroundColour());
	}
	const wxRect labelRect(checkBoxRect.width + 5, 0, gcdc.GetTextExtent(m_label).GetWidth(), gcdc.GetTextExtent(m_label).GetHeight());
	gcdc.DrawLabel(m_label, labelRect, wxALIGN_CENTER);

	event.Skip();
}

void CNNBSSControlCheckBox::OnMouse(wxMouseEvent& event)
{
	if (event.GetEventType() == wxEVT_LEFT_UP)
	{
		Check(!GetValue());

		Refresh();
	}

	event.Skip();
}

void CNNBSSControlCheckBox::OnKeyPressed(wxKeyEvent& event)
{
	event.Skip();
}

void CNNBSSControlCheckBox::OnKeyReleased(wxKeyEvent& event)
{
	if (!HasFocus() || !IsEnabled() || !IsShown() || !IsShownOnScreen())
	{
		event.Skip();
		return;
	}

	if (event.GetKeyCode() == 13 || event.GetKeyCode() == 32)// Enter or Space
	{
		Check(!GetValue());
		Refresh();
	}
	event.Skip();
}

void CNNBSSControlCheckBox::OnSetFocus(wxFocusEvent& event)
{
	Refresh();
	event.Skip();
}

void CNNBSSControlCheckBox::OnKillFocus(wxFocusEvent& event)
{
	Refresh();
	event.Skip();
}

void CNNBSSControlCheckBox::OnSize(wxSizeEvent& event)
{
	event.Skip();
}
