/////////////////////////////////////////////////////////////////////////////
// Name:        DeviceStatusBar.cpp
// Purpose:     Displays Hardware Performance
// Author:      Orkhan Aliyev
// Created:     02-04-2022
// Copyright:   (c) Orkhan Aliyev	
/////////////////////////////////////////////////////////////////////////////
#ifndef DEVICESTATUSBAR_H
#define DEVICESTATUSBAR_H

class CNNBSSDeviceStatusBar : public wxPanel
{
public:
	CNNBSSDeviceStatusBar() {};
	~CNNBSSDeviceStatusBar() {};

	void _Create(wxWindow* parent,
		wxWindowID winid = -1,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0L,
		const wxString& name = wxT("DefaultDeviceStatusBarName"))
	{
		Create(parent,winid,pos,size,style,name);

		_SetupInterface();
	};

	wxPanel* GetPanel()
	{
		return this;
	};

	void _SetupInterface();
};

class CNNBSSStatusProgressBar : public wxPanel
{

public:
	CNNBSSStatusProgressBar(wxWindow* parent);

	void _SetMinMax(double min, double max)
	{
		_min = min;
		_max = max;
	}

	void _SetCurrent(double current)
	{
		_current = current;
		paintNow();
	}

	void _SetBackgroundColour(wxColour backgroundColour)
	{
		_backgroundColour = backgroundColour;
		paintNow();
	};
	void _SetBorderColour(wxColour borderColour)
	{
		_borderColour = borderColour;
		paintNow();
	};
	void _SetIndicatorColour(wxColour indicatorColour)
	{
		_indicatorColour = indicatorColour;
		paintNow();
	};
	void _SetTextColour(wxColour textColour)
	{
		_textColour = textColour;
		paintNow();
	};

	void _SetTextFont(wxFont textFont)
	{
		_textFont = textFont;
		paintNow();
	};

	// Show/Hide
	void _ShowText(bool show)
	{
		_boolShowText = show;
		paintNow();
	};

	void SetLabel(const wxString& string) { _label = string; };
private:
	double _min = 0, _max = 100, _current = 0;

	void paintEvent(wxPaintEvent& evt);
	void paintNow();

	wxColour _backgroundColour = wxColour(0, 0, 0);
	wxColour _borderColour = wxColour(0, 0, 0);
	wxColour _indicatorColour = wxColour(0, 0, 0);
	wxColour _textColour = wxColour(0,0,0);

	wxString _label;

	wxFont _textFont;

	bool _boolShowText;
};

#endif // ! DEVICESTATUSBAR_H