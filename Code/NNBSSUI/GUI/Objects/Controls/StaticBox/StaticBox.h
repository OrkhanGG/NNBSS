#ifndef STATICBOX_H
#define STATICBOX_H

class CNNBSSControlStaticBox : public wxPanel
{
public:
	CNNBSSControlStaticBox() : wxPanel() {}
	CNNBSSControlStaticBox(wxWindow* win, wxWindowID id, const wxString& label,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0L);
	~CNNBSSControlStaticBox();

private:
	bool AcceptsFocus() const override{ return false; }
    // This window should not have been focused!
    //bool AcceptsFocusRecursively() const override { return false; }
    //bool AcceptsFocusFromKeyboard() const override {return false;}
    //bool CanBeFocused() const override { return false; }


	void OnPaint(wxPaintEvent& event);
	void OnEraseBackGround(wxEraseEvent& event) {};

	wxString m_label;

	DECLARE_EVENT_TABLE()
};

#endif // ! STATICBOX_H