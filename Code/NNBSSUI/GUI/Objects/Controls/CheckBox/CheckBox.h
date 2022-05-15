#ifndef CHECKBOX_H
#define CHECKBOX_H

class CNNBSSControlCheckBox : public wxWindow
{
public:
	CNNBSSControlCheckBox() : wxWindow() {}
    CNNBSSControlCheckBox(wxWindow* parent, wxWindowID id, const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxT("DefaultNameCheckBoxControl"))
    {
        _Create(parent, id, label, pos, size, style, validator, name);
    }
    void _Create(wxWindow* parent, wxWindowID id, const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(30, 30),
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxT("DefaultNameCheckBoxControl"));

    bool GetValue()const { return m_checked; };
    void Check(bool check);
    // CheckSilent() does not fire any event. Can be used for veto kind of purposes
    void CheckSilent(bool check);
private:
    bool Enable(bool enable = true)override;

    void OnPaint(wxPaintEvent& event);
    void OnMouse(wxMouseEvent& event);
    void OnKeyPressed(wxKeyEvent& event);
    void OnKeyReleased(wxKeyEvent& event);
    void OnSetFocus(wxFocusEvent& event);
    void OnKillFocus(wxFocusEvent& event);
    void OnEraseBackGround(wxEraseEvent& event) {};
    void OnSize(wxSizeEvent& event);

    wxString m_label;
    bool m_checked{false};

    DECLARE_EVENT_TABLE()
};

#endif // ! CHECKBOX_H
