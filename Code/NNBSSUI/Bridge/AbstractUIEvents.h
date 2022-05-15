///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef ABSTRACTUIEVENTS_H
#define ABSTRACTUIEVENTS_H

// Standard C Lib
#include <list>
// wxWidgets
#include <wx/event.h>
#include <wx/eventfilter.h>
#include <wx/cmdproc.h>// Command Processor

class CNNBSSMouseEvents
{
    // This is not the best way of doing this, I'd recommend to use GlobalEventHandler(wxWidgets' events for this one)
public:
    CNNBSSMouseEvents() {};
	~CNNBSSMouseEvents();

    void OnAnyButtonClick(wxMouseEvent& event);
};


//------------------------------------------------------------
// CommandProcessor
// Operations that are registered by this class may be done and undone.(Undo/Redo)
// Base class for all operations on DrawingDocument
class NNBSSCommandsBase : public wxCommand
{
public:
    NNBSSCommandsBase(NNBSSCommandsBase* doc,
        const wxString& name)
        : wxCommand(true, name)
    {
    }

protected:
    bool DoAdd() { }
    bool DoRemove() {}
};

/*
  /$$$$$$                        /$$                                   /$$$$$$$$                              /$$             
 /$$__  $$                      | $$                                  | $$_____/                             | $$             
| $$  \__/ /$$   /$$  /$$$$$$$ /$$$$$$    /$$$$$$  /$$$$$$/$$$$       | $$    /$$    /$$ /$$$$$$  /$$$$$$$  /$$$$$$   /$$$$$$$
| $$      | $$  | $$ /$$_____/|_  $$_/   /$$__  $$| $$_  $$_  $$      | $$$$$|  $$  /$$//$$__  $$| $$__  $$|_  $$_/  /$$_____/
| $$      | $$  | $$|  $$$$$$   | $$    | $$  \ $$| $$ \ $$ \ $$      | $$__/ \  $$/$$/| $$$$$$$$| $$  \ $$  | $$   |  $$$$$$ 
| $$    $$| $$  | $$ \____  $$  | $$ /$$| $$  | $$| $$ | $$ | $$      | $$     \  $$$/ | $$_____/| $$  | $$  | $$ /$$\____  $$
|  $$$$$$/|  $$$$$$/ /$$$$$$$/  |  $$$$/|  $$$$$$/| $$ | $$ | $$      | $$$$$$$$\  $/  |  $$$$$$$| $$  | $$  |  $$$$//$$$$$$$/
 \______/  \______/ |_______/    \___/   \______/ |__/ |__/ |__/      |________/ \_/    \_______/|__/  |__/   \___/ |_______/ 
*/                                                                                                                            
//-----------------------------------------------------------
class SNNBSSMVMData;/* Forward declaration*/
class CNNBSSEventsDeviceList : public wxCommandEvent
{
public:
    CNNBSSEventsDeviceList(wxEventType eventType,
        SNNBSSMVMData* mvmParams = (SNNBSSMVMData*)0)
        : wxCommandEvent(eventType), _mvm_params(mvmParams)
    {}

    SNNBSSMVMData* GetMVMParams() const { return _mvm_params; }

    wxEvent* Clone() const override { return new CNNBSSEventsDeviceList(*this); }
protected:
    SNNBSSMVMData* _mvm_params;
};

// Declare custom events
wxDECLARE_EVENT(EVT_DEVICELIST_ON_ADD_REQUEST, CNNBSSEventsDeviceList);
wxDECLARE_EVENT(EVT_DEVICELIST_ON_ADD, CNNBSSEventsDeviceList);
wxDECLARE_EVENT(EVT_DEVICELIST_ON_REMOVE_REQUEST, CNNBSSEventsDeviceList);
wxDECLARE_EVENT(EVT_DEVICELIST_ON_REMOVE, CNNBSSEventsDeviceList);

//-----------------------------------------------------------
class CNNBSSEventsControlSlider : public wxCommandEvent
{
public:
    CNNBSSEventsControlSlider(wxEventType eventType)
        : wxCommandEvent(eventType)
    {}

    long GetSliderType()const { return m_SliderType; }
    long GetThumbType()const { return m_ThumbType; }
    long GetSliderValue()const { return m_SliderValue; }

    void SetSliderType(const long& sliderType) { m_SliderType = sliderType; };
    void SetThumbType(const long& thumbType) { m_ThumbType = thumbType; };
    void SetSliderValue(const long& sliderValue) { m_SliderValue = sliderValue; };

    wxEvent* Clone() const override { return new CNNBSSEventsControlSlider(*this); }
protected:
    long m_SliderType{-1};
    long m_ThumbType{ -1 };

    long m_SliderValue{0};
};

wxDECLARE_EVENT(EVT_CONTROLSLIDER_ON_CHANGE, CNNBSSEventsControlSlider);

#endif // ! ABSTRACTUIEVENTS_H