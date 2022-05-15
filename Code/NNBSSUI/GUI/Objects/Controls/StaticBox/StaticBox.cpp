// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif
#include <wx/dcgraph.h>

#include "StaticBox.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

BEGIN_EVENT_TABLE(CNNBSSControlStaticBox, wxWindow)
EVT_PAINT(CNNBSSControlStaticBox::OnPaint)
EVT_ERASE_BACKGROUND(CNNBSSControlStaticBox::OnEraseBackGround)
END_EVENT_TABLE()

CNNBSSControlStaticBox::CNNBSSControlStaticBox(wxWindow* win, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size, long style)
	: wxPanel(win, id, pos, size, style | wxNO_BORDER | wxFULL_REPAINT_ON_RESIZE)
{
	m_label = label;

	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

CNNBSSControlStaticBox::~CNNBSSControlStaticBox()
{
}

void CNNBSSControlStaticBox::OnPaint(wxPaintEvent& WXUNUSED(event))
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

	// Set Pen to draw borders
	gcdc.GetGraphicsContext()->SetPen(GetForegroundColour());

	if (m_label.empty())
		gcdc.DrawRoundedRectangle(0, 0, GetSize().x, GetSize().y, 2);
	else
	{
		wxRect TextArea(10, gcdc.GetTextExtent(m_label).GetHeight() / 2, gcdc.GetTextExtent(m_label).GetWidth(), gcdc.GetTextExtent(m_label).GetHeight());

		// Draw border
		gcdc.DrawRoundedRectangle(0, gcdc.GetTextExtent(m_label).GetHeight(), GetSize().x, GetSize().y-(gcdc.GetTextExtent(m_label).GetHeight()), 2);

		// Exclude text area
		gcdc.SetBrush(GetBackgroundColour());
		gcdc.SetPen(GetBackgroundColour());
		gcdc.DrawRectangle(TextArea);

		// Draw label
		gcdc.SetTextBackground(GetBackgroundColour());
		gcdc.SetTextForeground(GetForegroundColour());
		gcdc.DrawLabel(m_label,TextArea, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
	}
}