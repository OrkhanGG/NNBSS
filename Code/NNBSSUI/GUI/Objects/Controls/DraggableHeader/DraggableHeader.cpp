// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
    // Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "DraggableHeader.h"

BEGIN_EVENT_TABLE(CNNBSSControlDraggableHeader, wxWindow)
EVT_MOUSE_CAPTURE_LOST(CNNBSSControlDraggableHeader::OnMouseCaptureLost)
//EVT_LEFT_DOWN(MyTitleWnd::OnMouseLDown)
EVT_LEFT_UP(CNNBSSControlDraggableHeader::OnMouseLUp)
EVT_MOUSE_EVENTS(CNNBSSControlDraggableHeader::OnMouseMove)
EVT_LEAVE_WINDOW(CNNBSSControlDraggableHeader::OnMouseLeave)
END_EVENT_TABLE()

CNNBSSControlDraggableHeader::CNNBSSControlDraggableHeader(wxWindow* parent, wxWindowID id,
    const wxString& label, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, 
    const wxString& name)
    : wxWindow(parent,id,pos,size,style), m_Label(label)
{
    if (!m_Label.empty())
    {
        wxStaticText* p_Label = new wxStaticText(this,wxID_ANY, m_Label);
        p_Label->SetForegroundColour(RGB_COLOURDEF_WHITE);

        wxBoxSizer* p_LabelSizer = new wxBoxSizer(wxVERTICAL);
        p_LabelSizer->AddStretchSpacer();
        p_LabelSizer->Add(p_Label, wxSizerFlags().Proportion(1).CenterHorizontal());
        p_LabelSizer->AddStretchSpacer();
        SetSizer(p_LabelSizer);
    }
}

void CNNBSSControlDraggableHeader::OnMouseMove(wxMouseEvent& event)
{
    if (event.LeftIsDown() && event.Dragging())
    {
        wxPoint pt = event.GetPosition();
        wxPoint wndLeftTopPt = GetParent()->GetPosition();
        int distanceX = pt.x - m_LastPoint.x;
        int distanceY = pt.y - m_LastPoint.y;

        wxPoint desPt;
        desPt.x = distanceX + wndLeftTopPt.x;
        desPt.y = distanceY + wndLeftTopPt.y;
        GetParent()->Move(desPt);

    }

    if (event.LeftDown())
    {
        CaptureMouse();
        m_LastPoint = event.GetPosition();
    }
}

void CNNBSSControlDraggableHeader::OnMouseLeave(wxMouseEvent& event)
{
    if (event.LeftIsDown() && event.Dragging())
    {
        wxPoint pt = event.GetPosition();
        wxPoint wndLeftTopPt = GetParent()->GetPosition();
        int distanceX = pt.x - m_LastPoint.x;
        int distanceY = pt.y - m_LastPoint.y;

        wxPoint desPt;
        desPt.x = distanceX + wndLeftTopPt.x;
        desPt.y = distanceY + wndLeftTopPt.y;
        GetParent()->Move(desPt);

    }
}

void CNNBSSControlDraggableHeader::OnMouseLDown(wxMouseEvent& event)
{
    CaptureMouse();
}

void CNNBSSControlDraggableHeader::OnMouseLUp(wxMouseEvent& event)
{
    if (HasCapture())
        ReleaseMouse();
}

void CNNBSSControlDraggableHeader::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (HasCapture())
        ReleaseMouse();
}