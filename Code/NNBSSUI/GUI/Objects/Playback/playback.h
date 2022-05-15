///////////////////////////////////////////////////////////////////////////////
// Name:        Playback.h
// Purpose:     Helps user to play recorded videos with handy functionalities
// Author:      Orkhan Aliyev
// Created:     2022-02-13
// Copyright:   (c) 2022 Orkhan Aliyev
//////////////////////////////////////////////////////////////////////////////

#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <NNBSSUI/GUI/Objects/BaseClasses/GridViewPanel/GridViewPanel.h>

class CNNBSSControlSlider;
class CNNBSSControlButton;
class CNNBSSEventsControlSlider;

class wxBitmapButton;
class wxCommandEvent;
class wxMouseEvent;

class CNNBSSPlaybackPanel : public CNNBSSGridViewPanelBase
{
public:
                                                                    CNNBSSPlaybackPanel(wxWindow* parent = nullptr);
    virtual                                                         ~CNNBSSPlaybackPanel();

    void                                                            _BindEvents(bool bind = true)wxOVERRIDE;

    void                                                            AddVideo(SNNBSSMVMData* params, const int& rowNumber);
    void                                                            RemoveVideo(SNNBSSMVMData* params);

    void															OnDeviceListEvent(CNNBSSEventsDeviceList& evt) wxOVERRIDE {};
private:


    void															OnDeviceAddAttempt(MovingVisualMediaEvent& evt) wxOVERRIDE {};
    void															OnDeviceConnectionFail(MovingVisualMediaEvent& evt) wxOVERRIDE {};
    void															OnDeviceConnectionLevelChange(MovingVisualMediaEvent& evt) wxOVERRIDE {};
    void															OnDeviceConnected(MovingVisualMediaEvent& evt) wxOVERRIDE {};
    void															OnDeviceDisconnected(MovingVisualMediaEvent& evt) wxOVERRIDE {};

    void															OnCameraCaptureStarted(MovingVisualMediaEvent& evt) wxOVERRIDE {};
    void															OnCameraCommandResult(MovingVisualMediaEvent& evt) wxOVERRIDE {};
    void															OnCameraErrorException(MovingVisualMediaEvent& evt) wxOVERRIDE {};
    void															OnCameraErrorOpen(MovingVisualMediaEvent& evt) wxOVERRIDE {};
    void															OnCameraErrorEmpty(MovingVisualMediaEvent& evt) wxOVERRIDE {};

    void															OnContextMenu(wxContextMenuEvent& evt)wxOVERRIDE;
    void															OnDropCameraOnThumbnail(wxDropFilesEvent& event) wxOVERRIDE;
    void															OnAnyThumbnailClick(wxMouseEvent& evt) wxOVERRIDE;
    void															OnAnyThumbnailDoubleClick(wxMouseEvent& evt) wxOVERRIDE;

    // Was not overriden and declared as different function
    // But as the name suggests, it'sa button click event
    // The reason of instancing this(I mean, it also exist in baseclass with different name)
    // Is because of the additional custom widgets/controls that baseclass does not have
    void                                                            OnAnyButtonLeftUPSpecial(wxMouseEvent& evt);

    void                                                            OnTimelineUpdate(wxTimerEvent&);
    void                                                            OnTimelineChange(CNNBSSEventsControlSlider& evt);


    enum ContextMenuEventID
    {
        CMEID_NONE = 304,

        CMEID_FULLSCREEN,
        CMEID_HORIZONTAL_FLIP,
        CMEID_VERTICAL_FLIP,
        CMEID_REMOVE,
        CMEID_PROPERTIES
    };

    struct ContextMenu
    {
        wxMenu* p_ContextMenu = nullptr;
        wxMenu* p_DisplaySubMenu = nullptr;

        wxMenuItem* FullscreenMenuItem = nullptr;
        wxMenuItem* HorizontalFlipMenuItem = nullptr;
        wxMenuItem* VerticalFlipMenuItem = nullptr;
        wxMenuItem* RemoveMenuItem = nullptr;
        wxMenuItem* PropertiesMenuItem = nullptr;
    }m_ContextMenu;

    CNNBSSControlButton*                                            m_VideoPlayButton = nullptr;
    CNNBSSControlButton*                                            m_VideoPauseButton = nullptr;
    CNNBSSControlButton*                                            m_VideoStopButton = nullptr;
    CNNBSSControlButton*                                            m_VideoPreviousFrameButton = nullptr;
    CNNBSSControlButton*                                            m_VideoNextFrameButton = nullptr;
    CNNBSSControlSlider*                                            p_VideoSlider = nullptr;

    wxTimer                                                         m_SliderUpdateTimer;
};

#endif // ! PLAYBACK_H