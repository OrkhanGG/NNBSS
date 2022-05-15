#ifndef DRAGGABLEHEADER_H
#define DRAGGABLEHEADER_H

class CNNBSSControlDraggableHeader : public wxWindow
{
public:
	CNNBSSControlDraggableHeader(wxWindow* parent, wxWindowID id,
		const wxString& label,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxT("DefaultNameDraggableHeader"));
	~CNNBSSControlDraggableHeader() {};

private:
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnMouseLDown(wxMouseEvent& event);
	void OnMouseLUp(wxMouseEvent& event);
	void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

	wxString m_Label;// Title
	wxPoint m_LastPoint;// Last mouse point

	DECLARE_EVENT_TABLE()
};

#endif