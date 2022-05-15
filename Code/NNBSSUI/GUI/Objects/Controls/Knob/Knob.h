/////////////////////////////////////////////////////////////////////////////
// Name:        Knob.h
// Purpose:     Knob Control
// Author:      Orkhan Aliyev
// Additional:  We assume it as it's a different looking slider. Does the same whatever controlSlider does.
// Created:     02-04-2022
// Copyright:   (c) Orkhan Aliyev	
/////////////////////////////////////////////////////////////////////////////

class CNNBSSControlKnob : public wxControl
{
public:
    CNNBSSControlKnob (wxWindow* parent, wxWindowID id, const int& value,
            const int& defaultValue,
            const int& minValue, const int& maxValue,
            unsigned int minAngle = 240, unsigned int range = 300,
            const wxPoint &pos = wxDefaultPosition, 
            const wxSize &size = wxDefaultSize, 
            long style = 0,
            const wxValidator &validator = wxDefaultValidator,
            const wxString &name = wxT ("DefaultNameKnobControl"))
        : m_defaultValue(defaultValue)
    {
        Create (parent, id, value, minValue, maxValue,
                minAngle, range, pos, size, style, validator, name );
    }
    void Create (wxWindow* parent, wxWindowID id, const int& value,
                 const int& minValue,const int& maxValue,
                 unsigned int minAngle = 240, unsigned int range = 300,
                 const wxPoint &pos = wxDefaultPosition, 
                 const wxSize &size = wxSize (30, 30), 
                 long style = 0,
                 const wxValidator &validator = wxDefaultValidator,
                 const wxString &name = wxT ("DefaultNameKnobControl"));

    // retrieve/change the range
    void SetRange (int minValue, int maxValue);
    int GetMin() const { return m_min; }
    int GetMax() const { return m_max; }
    void SetMin (int minValue) { SetRange( minValue , GetMax() ) ; }
    void SetMax (int maxValue) { SetRange( GetMin() , maxValue ) ; }
    unsigned int GetMinAngle() const { return (m_maxAngle - m_range) % 360; }
    int GetMaxAngle() const { return m_maxAngle; }
    int GetValue () const { return m_setting; }
    int SetValue (int value);

    bool Enable(bool enable = true)override;
private:
    void GetCenter (int &x, int &y) const;

    void OnPaint(wxPaintEvent& event);
    void OnMouse(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnKeyPressed(wxKeyEvent& event);
    void OnKeyReleased(wxKeyEvent& event);
    void OnSetFocus(wxFocusEvent& event);
    void OnKillFocus(wxFocusEvent& event);
    void OnEraseBackGround(wxEraseEvent& event) {};

    int m_min;
    int m_max;
    int m_setting;
    const int m_defaultValue;
    unsigned int m_maxAngle,m_minAngleNumber;
    unsigned int m_range;

    DECLARE_EVENT_TABLE()
};