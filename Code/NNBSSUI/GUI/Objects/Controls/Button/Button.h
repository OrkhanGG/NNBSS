/////////////////////////////////////////////////////////////////////////////
// Name:        Button.h
// Purpose:     Button Control
// Author:      Orkhan Aliyev
// Created:     02-04-2022
// Copyright:   (c) Orkhan Aliyev	
/////////////////////////////////////////////////////////////////////////////

#ifndef BUTTON_H
#define BUTTON_H

// Standard C Lib
#include <map>

class CNNBSSControlButton : public wxWindow
{
public:
	CNNBSSControlButton() : wxWindow() { }
	CNNBSSControlButton(wxWindow* parent, wxWindowID id,
		const wxString& label,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxT("DefaultNameKnobControl"))
	{
		Create(parent, id, label, pos, size, style, validator, name);
	}
	void Create(wxWindow* parent, wxWindowID id,
		const wxString& label,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize(30, 30),
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxT("DefaultNameKnobControl"));

	typedef enum
	{
		BS_IDLE = 0,
		BS_HOVER,
		BS_PRESS,
		BS_SELECTED,// Not sure yet

		BS_DISABLED,

		BS_DEFAULT = BS_IDLE
	}ButtonStates;

	void SetButtonLabel(const wxString& label, const ButtonStates& state = BS_IDLE);
	wxString GetButtonLabel(const ButtonStates& state = BS_IDLE) { return m_ButtonLabels[state]; };

	void SetButtonHelpLabel(const wxString& label, const ButtonStates& state = BS_IDLE);
	wxString GetButtonHelpLabel(const ButtonStates& state = BS_IDLE) { return m_ButtonHelpLabels[state]; };

	void SetButtonFont(const wxFont& font, const ButtonStates& state = BS_IDLE);
	wxFont GetButtonFont(const ButtonStates& state = BS_IDLE) { return m_ButtonFonts[state]; };

	void SetButtonBitmap(const wxBitmap& bitmap, const ButtonStates& state = BS_IDLE);
	wxBitmap GetButtonBitmap(const ButtonStates& state = BS_IDLE) { return m_ButtonBitmaps[state]; };

	void SetButtonTextColour(const wxColour& colour, const ButtonStates& state = BS_IDLE);
	wxColour GetButtonTextColour(const ButtonStates& state = BS_IDLE) { return m_ButtonTextColours[state]; };

	void SetButtonBackgroundColour(const wxColour& colour, const ButtonStates& state = BS_IDLE);
	wxColour GetButtonBackgroundColour(const ButtonStates& state = BS_IDLE) { return m_ButtonBackgroundColours[state]; };

	void SetButtonLabelAlignment(const long& alignment, const ButtonStates& state = BS_IDLE);
	long GetButtonLabelAlignment(const ButtonStates& state = BS_IDLE) { return m_ButtonLabelAlignments[state]; };

	void SetButtonRoundness(const int& roundness, const ButtonStates& state = BS_IDLE);
	int GetButtonRoundness(const ButtonStates& state = BS_IDLE) { return m_ButtonRoundness[state]; };

	long GetButtonCurrentState()const {return m_ButtonCurrentState;};
private:
	wxString GetCurrentLabel()const;
	wxString GetCurrentHelpLabel()const;
	wxFont   GetCurrentFont()const;
	wxBitmap GetCurrentBitmap()const;
	wxColour GetCurrentTextColour()const;
	wxColour GetCurrentBackgroundColour()const;
	long	 GetCurrentAlignment()const;
	int      GetCurrentRoundness()const;

	wxFont   GetDefaultFont();
	wxFont   GetItalicFont();
	wxFont   GetNormalFont();
	wxFont   GetBoldFont();

	void SetMouseOver(bool over) { m_MouseOver = over; };
	bool GetMouseOver()const { return m_MouseOver; };

	void OnPaint(wxPaintEvent& event);

	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseRightUp(wxMouseEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseLost(wxMouseCaptureLostEvent& event);
	void OnKeyPressed(wxKeyEvent& event);
	void OnKeyReleased(wxKeyEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnKillFocus(wxFocusEvent& event);
	void OnResize(wxSizeEvent& event);

	std::map<ButtonStates, wxString> m_ButtonLabels				;
	std::map<ButtonStates, wxString> m_ButtonHelpLabels			;
	std::map<ButtonStates, wxFont> m_ButtonFonts				;
	std::map<ButtonStates, wxBitmap> m_ButtonBitmaps			;
	std::map<ButtonStates, wxColour> m_ButtonTextColours		;
	std::map<ButtonStates, wxColour> m_ButtonBackgroundColours	;
	std::map<ButtonStates, long> m_ButtonLabelAlignments		;
	std::map<ButtonStates, int> m_ButtonRoundness				;

	static constexpr long m_DefaultAlignment = wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL;

	ButtonStates m_ButtonCurrentState{ ButtonStates::BS_IDLE };

	bool m_MouseOver{false};
};

#endif