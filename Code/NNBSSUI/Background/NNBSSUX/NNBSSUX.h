///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef NNBSSUX_H
#define NNBSSUX_H

#include <wx/frame.h>

// Forward declaration
class MovingVisualMediaEvent;
class CNNBSSEventsDeviceList;

class CNNBSSUX : public wxFrame
{
public:
    CNNBSSUX() {};
    ~CNNBSSUX();

    void _Create(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0L);

    WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam) wxOVERRIDE;

    wxPanel* GetUXManagedWindow()const { return p_ManagedWindow; };
private:
    void OnDeviceListEvent(CNNBSSEventsDeviceList& evt);
    void OnMVMEvent(MovingVisualMediaEvent& evt);

    void OnFrameClose(wxCloseEvent& evt);

    void ExitAll();

    //--------------------------------
    wxPanel* p_ManagedWindow = nullptr;
};

#endif // ! NNBSSUX_H