///////////////////////////////////////////////////////////////////////////////
// Name:        BitmapPanel.h
// Purpose:     Displays a bitmap on a wxWindow
// Author:      Orkhan Aliyev
// Created:     2021-12-07
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#ifndef CAMERAPANEL_H
#define CAMERAPANEL_H

const wxString DefaultThumbnailPlaceholder = wxString("Either click to add a device or drag from Device Manager panel");

//wxWidgets
#include <wx/msgqueue.h>// I can't avoid including this

/***********************************************************************************************

	CNNBSSThumbnailFrameData:   a struct containing information about a captured camera frame,
								created in a camera thread and processed in the GUI thread

***********************************************************************************************/

class CNNBSSThumbnailFrameData
{
public:
	CNNBSSThumbnailFrameData() {};
	~CNNBSSThumbnailFrameData() {};

	wxString GetMVMName() const { return m_MVMName; }
	wxString GetMVMSourceAddress() const { return m_MVMSourceAddress; }
	// captured camera frame
	wxBitmap* GetFrame() { return m_Frame; }
	// optional thumbnail, created when thumbnailSize passed to CameraThread is not empty
	wxBitmap* GetThumbnail() { return m_Thumbnail; }
	// Get Thumbnail Panel
	CNNBSSBitmapPanel* GetThumbnailPanel() const { return p_Thumbnail; };
	// Get Event Sink(Which object that all events will be sent to)
	wxEvtHandler* GetEventSinkObject() const { return m_EventSink; };

	void SetMVMName(const wxString& MVMName) { m_MVMName = MVMName; }
	void SetMVMSourceAddress(const wxString& MVMSourceAddress) { m_MVMSourceAddress = MVMSourceAddress; }
	void SetFrame(wxBitmap* frame) { m_Frame = frame; }
	void SetThumbnail(wxBitmap* thumbnail) { m_Thumbnail = thumbnail; }
	void SetThumbnailPanel(CNNBSSBitmapPanel* ThumbnailPanel) { p_Thumbnail = ThumbnailPanel; };
	void SetEventSinkObject(wxEvtHandler* evtSink) { m_EventSink = evtSink; };

	bool                IsOk();

	private:
	wxString            m_MVMName;
	wxString            m_MVMSourceAddress;
	CNNBSSBitmapPanel*	p_Thumbnail = nullptr;

	wxBitmap*			m_Frame{ nullptr };
	wxBitmap*			m_Thumbnail{ nullptr };

	wxEvtHandler*		m_EventSink = nullptr;
	bool                m_Initialized{ false };

	auto operator==(const CNNBSSThumbnailFrameData& otherData) const
	{
		if (m_MVMName != otherData.m_MVMName || m_MVMSourceAddress != otherData.m_MVMSourceAddress || m_EventSink != otherData.m_EventSink
			|| p_Thumbnail != otherData.p_Thumbnail || m_Initialized != otherData.m_Initialized)
			return false;
		else
			return true;
	}

};

typedef std::unique_ptr<CNNBSSThumbnailFrameData> NNBSSThumbnailFrameDataUPTR;
typedef std::vector<NNBSSThumbnailFrameDataUPTR>  NNBSSThumbnailFrameDataUPTRList;

/***********************************************************************************************

	SNNBSSThumbnailCommandData: a struct used by the main thread to communicate with CameraThread.

***********************************************************************************************/

struct SNNBSSThumbnailCommandData
{
	struct SNNBSSOpenCVVideoCaptureProperties
	{
		int    id{ 0 };
		double value{ 0. };
		// only for SetVCProp, result of cv::VideoCapture::set()
		bool   succeeded{ false };
	};
	typedef std::vector<SNNBSSOpenCVVideoCaptureProperties> SNNBSSOpenCVVideoCapturePropertiesList;

	struct InputParams
	{
		bool	m_MVMUseDefaultInput{ false };
		int		m_MVMInputWidth{ 0 },
				m_MVMInputHeight{ 0 },
				m_MVMInputFPS{ 0 }; // set 0 to set default
	};

	struct ImageProcessingParams
	{
		// MVM Visual Params ----------------------
		bool   m_ImageProcessingShowHistogram{ false };
		bool   m_ImageProcessingActivate{ false };
		static constexpr int	m_ImageProcessingBrightnessDefault = 0, 
								m_ImageProcessingContrastDefault = 1;
		int    m_ImageProcessingBrightness{ m_ImageProcessingBrightnessDefault };
		double m_ImageProcessingContrast{ static_cast<double>(m_ImageProcessingContrastDefault) };

		bool m_ImageProcessingFlipHorizontal{ false }, m_ImageProcessingFlipVertical{ false };
		//--------------------------------------------
	};

	struct RecordParams
	{
	   // MVM Record Params ----------------------
		bool m_Record{ false };
		int  m_RecordResolutionWidth{ 640 },
			m_RecordResolutionHeight{ 480 },
			m_RecordFPS{ 25 };
		std::string m_RecordEncoderName{};
		//-----------------------------------------
	};

	struct StreamParams
	{
		// MVM Streaming Params -------------------
		bool m_Streaming{ false };
		std::string m_StreamAddress, m_StreamAddressPort;
		int m_StreamResolutionWidth{ 640 }, m_StreamResolutionHeight{ 480 }, m_StreamFPS{ 25 }, m_StreamCompressPercentage{ 50 }/*compression between 0-100, lower better performance, lack of quality*/;
		//--------------------------------------------
	};

	enum Commands
	{
		// parameter is SNNBSSOpenCVVideoCapturePropertiesList
		GetVCProp = 0,
		SetVCProp,

		GetInputParams,
		SetInputParams,

		GetRecordParams,
		SetRecordParams,

		GetStreamParams,
		SetStreamParams,

		GetImageProcessingParams,
		SetImageProcessingParams
	};
	Commands command;

	SNNBSSMVMData* p_MVM = nullptr;
	wxAny    parameter;
};
typedef wxMessageQueue<SNNBSSThumbnailCommandData> NNBSSThumbnailCommandDatas;

/***********************************************************************************************

	CNNBSSBitmapPanel: Displays a bitmap on a wxWindow

***********************************************************************************************/

class CNNBSSBitmapPanel : public wxWindow
{
public:
	CNNBSSBitmapPanel(wxWindow* parent);

	~CNNBSSBitmapPanel();

	enum STATES
	{
		BPS_EMPTY = 100,
		BPS_RECIEVES,
		BPS_PAUSED,
		BPS_ERROR
	};

	enum FLAGS
	{
		BPF_NOTHING = 1 << 0,// [ don't use ]
		BPF_DATETIME = 1 << 1,
		BPF_DISPLAYSELECTION = 1 << 2,
	};

	enum PROPERTIES
	{
		BPP_STATE = 200,
		BPP_FLAGS,
		BPP_NAME,
		BPP_SELECTED,
		BPP_PLACEHOLDERTEXT,
		BPP_RIGHT_TOP_TEXT,

		BPP_EVENT_INIT_SENT
	};

	void Attach(SNNBSSMVMData* MVM);
	void Detach(SNNBSSMVMData* MVM);
	SNNBSSMVMData* GetAttachedMVM()const { return p_MVM; };// returns nullptr if is not attached or detached
	void SetAttachedMVM(SNNBSSMVMData* MVM) { p_MVM = MVM; };

	void Reset();
	void SetBitmap(const wxBitmap bitmap);

	// Get property value; CNNBSSBitmapPanel::PROPERTIES
	template < typename ReturnType >
	ReturnType GetProperty(const long property/*PROPERTIES::X*/) const
	{
		if (!this)
			return {};

		if constexpr (std::is_same<ReturnType, wxString>::value) {

			if (property == PROPERTIES::BPP_NAME)
				return m_PanelName;
			else if (property == PROPERTIES::BPP_PLACEHOLDERTEXT)
				return m_PlaceHolderText;
			else if (property == PROPERTIES::BPP_RIGHT_TOP_TEXT)
				return m_RightTopText;

			return {};
		}
		else if constexpr (std::is_same<ReturnType, bool>::value) {

			if (property == PROPERTIES::BPP_SELECTED)
				return _IsSelected;
			else if (property == PROPERTIES::BPP_EVENT_INIT_SENT)
				return m_EventInitSent;

			return {};
		}
		else if constexpr (std::is_same<ReturnType, long>::value) {
			if (property == PROPERTIES::BPP_FLAGS)
			{
				return m_flags;
			}
			else if (property == PROPERTIES::BPP_STATE)
				return m_state;

			return {};
		}
		return {};
	};

	// Set property value
	template < typename PType >
	void SetProperty(const long property, PType _value)
	{
		if (!this)
			return;
		if constexpr (std::is_same<PType, wxString>::value) {

			if (property == PROPERTIES::BPP_NAME)
				m_PanelName = _value;
			else if (property == PROPERTIES::BPP_PLACEHOLDERTEXT)
				m_PlaceHolderText = _value;
			else if (property == PROPERTIES::BPP_RIGHT_TOP_TEXT)
				m_RightTopText = _value;
		}
		if constexpr (std::is_same<PType, bool>::value) {

			if (property == PROPERTIES::BPP_SELECTED)
				_IsSelected = _value;
			else if (property == PROPERTIES::BPP_EVENT_INIT_SENT)
				m_EventInitSent = _value;
		}
		else if constexpr (std::is_same<PType, long>::value) {
			if (property == PROPERTIES::BPP_FLAGS)
			{
				m_flags = _value;
			}
			else if (property == PROPERTIES::BPP_STATE)
				m_state = _value;

		}
	};

	// Data traffic
	NNBSSThumbnailFrameDataUPTRList m_FrameData;
	NNBSSThumbnailCommandDatas		m_CommandData;
	wxCriticalSection				m_CriticalSection;
private:
	// Get frame data to display bitmap
	void							OnProcessFrameData(wxTimerEvent&);

	void							OnSelect(wxDC* dc);
	void							OnPaint(wxPaintEvent&);
	void							OnFocusSet(wxFocusEvent& evt);
	void							OnFocusKill(wxFocusEvent& evt);
	void							OnAnyKeyRelease(wxKeyEvent& evt);

	SNNBSSMVMData*					p_MVM = nullptr;

	wxBitmap						m_Bitmap;
	wxString						m_PanelName;
	wxString						m_PlaceHolderText;
	wxString						m_RightTopText;

	// default timer interval in ms for processing new camera frame data from MVM threads
	static const long				ms_defaultProcessFrameDataInterval = 30;
	long							m_processFrameDataInterval{ ms_defaultProcessFrameDataInterval };
	wxTimer							m_processFrameDataTimer;
	bool							m_EventInitSent{false};

	long m_state{ STATES::BPS_EMPTY };
	long m_flags{ -1 };

	bool _IsSelected{ false };
};

#endif // #ifndef CAMERAPANEL_H