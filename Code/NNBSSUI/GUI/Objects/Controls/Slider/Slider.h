///////////////////////////////////////////////////////////////////////////////
// Name:        slider.h
// Purpose:     Helps developer to have customized slider(Includes RangeBased and MonoType slider)
// Author:      Orkhan Aliyev
// Created:     2022-02-13
// Copyright:   (c) 2022 Orkhan Aliyev
//////////////////////////////////////////////////////////////////////////////

#ifndef SLIDER_H
#define SLIDER_H

class wxGCDC;

class CNNBSSControlSlider : public wxWindow
{
public:
	
	enum SliderType
	{
		MONO,
		RANGE_BASED,
	};

	enum ThumbType
	{
		LOW,
		HIGH,
	};

	class SliderThumb
	{
	public:
		SliderThumb() {};
		SliderThumb(CNNBSSControlSlider& parent, const int& value, const ThumbType& type);

		bool IsMouseOver(const wxPoint& click_pos);
		void Render(wxGCDC& dc);

		void SetVisible(bool _visible = true) { visible = _visible;};
		wxPoint GetPosition();
		void SetPosition(const wxPoint& mouse_position);
		void SetValue(const long& value);
		const long GetThumbValue() const { return value_; };
		const ThumbType GetType() const { return type_; };
		const wxSize GetSize() const { return size_; };
		int GetMin();
		int GetMax();

		bool mouse_over = false;
	private:
		CNNBSSControlSlider* parent_;
		long value_;
		ThumbType type_;
		wxSize size_;
		bool visible;

		wxPoint thumb_poly[5] = { wxPoint(0,0), wxPoint(0,13), wxPoint(5,18), wxPoint(10,13), wxPoint(10,0) };
		wxPoint thumb_shadow_poly[4] = { wxPoint(0,14), wxPoint(4,18), wxPoint(6,18), wxPoint(10,14) };

		wxColour idle_color;
		wxColour idle_shadow;
		wxColour dragged_color;
		wxColour dragged_shadow;
		wxColour mouse_over_color;
		wxColour mouse_over_shadow;

	};

	CNNBSSControlSlider(wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const int& sliderType = SliderType::MONO,
		const long& highValue = 0,
		const long& lowValue = 0,
		const long& min = 0,
		const long& max = 0,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxSL_HORIZONTAL,
		const wxString& name = wxPanelNameStr);

	bool Enable(bool enable = true)override;

	int GetValue(const ThumbType& thumb)const;
	void SetValue(const long& value, const ThumbType& thumb);

	void SetMax(const long& maxValue) { max_value = maxValue; };
	void SetMin(const long& minValue) { min_value = minValue; };
	long long GetMaxValue() const { return max_value; };
	long long GetMinValue() const { return min_value; };

	void ShowValueLabel(const bool& show) { m_noValueIndicator = !show; };
	bool IsValueLabelVisible() const { return !m_noValueIndicator; };

	void ShowThumb(const bool& show) { m_noThumb = !show; };
	bool IsThumbVisible() const { return !m_noThumb; };

	int GetSliderType() const { return m_slider_type; }

	void Reset();

private:
	void PaintEvent(wxPaintEvent& event);
	void Render(wxGCDC& dc);

	void OnMouseDown(wxMouseEvent& event);
	void OnMouseUp(wxMouseEvent& event);
	void OnMouseRightUp(wxMouseEvent& event);
	void OnMouseLost(wxMouseCaptureLostEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnKeyPressed(wxKeyEvent& event);
	void OnKeyReleased(wxKeyEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnKillFocus(wxFocusEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event) {};


	SliderThumb thumbs[2];
	SliderThumb* selected_thumb = nullptr;
	const long				m_defaultHighValue;// this variable is used for both range based slider's high thumb and mono typed slider's thumb
	const long				m_defaultLowValue; // this variable only used for range based slider's low thumb
	long					min_value;
	long					max_value;

	const int		m_slider_type; // Range based or Mono

	int left_border_width{ 4 };// The left distance/space/padding that might be necessary to take into account when value is visible
	int right_border_width{ 0 };// The right distance/space/padding that might be necessary to take into account when value is visible

	// Styles (I believe it would be better with flags instead of variables)
	bool              m_noValueIndicator{false};
	bool              m_noThumb{false};

	// Colours
	wxColour slider_background_color;
	wxColour slider_outline;
	wxColour selected_range_color;
	wxColour selected_range_outline;

	friend class SliderThumb;

	DECLARE_EVENT_TABLE()
};

#endif // ! SLIDER_H