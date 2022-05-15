///////////////////////////////////////////////////////////////////////////////
// Name:        MovingVisualMediaThread.h
// Purpose:     Thread and event for retrieving images from a camera with cv::VideoCapture
// Author:      Orkhan Aliyev
// Created:     2022-01-01
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

// Forward declaration
class CNNBSSBitmapPanel;
class CNNBSSAIObjectDetection;
class CNNBSSAIFaceRecognition;
struct SNNBSSMovingVisualMediaThreadInitList;
namespace cv { class Mat; class VideoCapture; }// To avoid including OpenCV header

// Standard C Lib
#include <memory>
#include <map>
#include <vector>

/***********************************************************************************************

   NNBSSBitmapPanelPool:
   The pointer NNBSSBitmapPanelPool contains considered to be used only for pointers to the CNNBSSBitmapPanel.
   Any data modification should be done under CrticialSection(wxCriticalSection) protection.
   Currently, Critical Section object (used to avoid data race) is the member of CNNBSSBitmapPanel(see CNNBSSBitmapPanel members).
   TODO: Move a better place.

***********************************************************************************************/
typedef std::vector<CNNBSSBitmapPanel*>  NNBSSBitmapPanelPool;
inline NNBSSBitmapPanelPool& GetBitmapPanelPool()
{
    static NNBSSBitmapPanelPool s_NNBSSBitmapPanelPool;
    return s_NNBSSBitmapPanelPool;
};

/***********************************************************************************************

    MovingVisualMediaEvent: an event sent from the worker thread to the GUI thread.
                 When type is EVT_MVM_ERROR_EXCEPTION, the exception information
                 is available via its inherited GetString() method.

***********************************************************************************************/

class MovingVisualMediaEvent : public wxThreadEvent
{
public:
    // Event type is one of EVT_CAMERA_xxx types declared below
    MovingVisualMediaEvent(wxEventType eventType, SNNBSSMVMData* MVM = (SNNBSSMVMData*)0)
        : wxThreadEvent(eventType), p_MVM(MVM)
    {}

    SNNBSSMVMData*               GetMVMParams() const { return p_MVM; }

    // Only for EVT_MVM_COMMAND_RESULT
    // TODO:[Implemention is required for Commands]
    SNNBSSThumbnailCommandData          GetCommandResult() const { return GetPayload<SNNBSSThumbnailCommandData>(); }

    wxEvent*                            Clone() const override { return new MovingVisualMediaEvent(*this); }
private:
    SNNBSSMVMData*               p_MVM;
};

//-----------------------------------------------------------------------------------------------------------------------
/***********************************************************************************************

    CNNBSSMovingVisualMediaThread Events

***********************************************************************************************/
// Device Events
// EVT_MVM_DEVICE_CONNECT_ATTEMPT- Attempt to add a new MVM device, request to allocate a new thread
// For all events that their name contain 'MVM_DEVICE' word will have only device information, not thumbnail data.
// EVT_MVM_DEVICE_CONNECT_FAIL - Could not be connected to MVM device
// EVT_MVM_DEVICE_CONNECTED - Successfully connected to MVM device
// EVT_MVM_DEVICE_HIGH_CONNECTION - The device has an average or higher connection.
// EVT_MVM_DEVICE_LOW_CONNECTION - Low/bad MVM device connection detected
// EVT_MVM_DEVICE_DISCONNECTED - Disconnected from device
//-----------------------------------------------------------------------------------------------------------------------
// Thumbnail Events
// EVT_MVM_CAPTURE_STARTED - VideoCapture's backend can be retrieved via event's GetString(),
// camera fps can be retrieved via event's GetInt(), if it returns non-zero
// EVT_MVM_COMMAND_RESULT - Result of the SNNBSSThumbnailCommandData's command sent to camera, use GetCommandResult()
// EVT_MVM_ERROR_OPEN - Could not open OpenCV camera capture
// EVT_MVM_ERROR_EMPTY - Could not retrieve a frame, consider connection to the camera lost.
// EVT_MVM_ERROR_EXCEPTION - An exception was thrown in the camera thread,
// see the event's GetString() for the exception information.
wxDECLARE_EVENT(EVT_MVM_DEVICE_CONNECT_ATTEMPT, MovingVisualMediaEvent);
wxDECLARE_EVENT(EVT_MVM_DEVICE_CONNECT_FAIL, MovingVisualMediaEvent);
wxDECLARE_EVENT(EVT_MVM_DEVICE_CONNECTED, MovingVisualMediaEvent);
wxDECLARE_EVENT(EVT_MVM_DEVICE_HIGH_CONNECTION, MovingVisualMediaEvent);
wxDECLARE_EVENT(EVT_MVM_DEVICE_LOW_CONNECTION, MovingVisualMediaEvent);
wxDECLARE_EVENT(EVT_MVM_DEVICE_DISCONNECTED, MovingVisualMediaEvent);
wxDECLARE_EVENT(EVT_MVM_CAPTURE_STARTED, MovingVisualMediaEvent);
wxDECLARE_EVENT(EVT_MVM_COMMAND_RESULT, MovingVisualMediaEvent);
wxDECLARE_EVENT(EVT_MVM_ERROR_OPEN, MovingVisualMediaEvent);
wxDECLARE_EVENT(EVT_MVM_ERROR_EMPTY, MovingVisualMediaEvent);
wxDECLARE_EVENT(EVT_MVM_ERROR_EXCEPTION, MovingVisualMediaEvent);
//-----------------------------------------------------------------------------------------------------------------------

/***********************************************************************************************

    CNNBSSMovingVisualMediaThread: retrieving images from a camera device with OpenCV

***********************************************************************************************/

class CNNBSSMovingVisualMediaThread : public wxThread
{
public:
    CNNBSSMovingVisualMediaThread(SNNBSSMVMData* params);
    ~CNNBSSMovingVisualMediaThread();

    bool     IsCapturing() const      { return m_isCapturing; }

    enum E_DATA_DECODER_TYPE
    {
        CT_INVALID = 0,
        CT_OPENCV_USB,
        CT_OPENCV_IP,
        CT_OPENCV_FILE,
        CT_NNBSS_IP
    };
    int m_DATA_DECODER_TYPE = CT_INVALID;
    int InitCapture();
   
private:
    ExitCode    Entry() override;

    void        ResizeKeepAspectRatio(const cv::Mat& src, cv::Mat& dst, const cv::Size& dstSize, const cv::Scalar& backgroundColor);
    int         PerformDetection(cv::Mat& output);// Returns rectangle count

    SNNBSSMovingVisualMediaThreadInitList*  m_Init = nullptr;
    SNNBSSMVMData*                          m_Parameters;
    std::shared_ptr<cv::VideoCapture>       p_VideoCapture;
    std::atomic_bool                        m_isCapturing{ false };

    enum DEVICE_CONNECTION_LEVEL
    {
        DCL_INIT = 0,
        DCL_LOW,
        DCL_HIGH
    };
    int m_LastDCL{ DCL_INIT };

    // Detection specials
    CNNBSSAIFaceRecognition* p_MainFaceRecognition = nullptr;
    std::vector<CNNBSSAIObjectDetection*> p_MainDetectorBase;
public:
    // Underscore(_) prefix is used only video files
    class _VideoFrameManager
    {
    public:
        _VideoFrameManager(){};
        ~_VideoFrameManager() {};

        const unsigned long long GetMin() const { return _min; };
        const unsigned long long GetMax() const { return _max; };
        const unsigned long long GetCurrent() const { return _current; };
        const bool GetPaused() const { return _Paused; };
        const bool GetLoop() const { return _Loop; };

        const void SetMin(const unsigned long long min) { _min = min; };
        const void SetMax(const unsigned long long max) { _max = max; };
        const void SetCurrent(const unsigned long long current) { _current = current; };
        const void SetPaused(bool paused) { _Paused = paused; };
        const void SetLoop(bool loop) { _Loop = loop; };
    private:
        unsigned long long _min = 0;
        unsigned long long _max = 0;
        unsigned long long _current = 0;
        bool _Paused = false;
        bool _Loop = false;
    }p_VideoManager;

    const unsigned long long _GetFrameCount() const;
    const unsigned long long _GetCurrentFrame() const;
    bool _PauseAt(const unsigned long long pauseAt);
    bool _IsPaused()const;
    bool _IsLooped()const;

    void _SetFrameNumber(const unsigned long long frameNumber);
    void _SetFrameNumberAndPause(const unsigned long long frameNumber);
    void _Play();
    void _Stop();
    void _NextFrame();
    void _PreviousFrame();
    void _Loop(bool loop);
private:
    void ProcessCameraCommand(const SNNBSSThumbnailCommandData& commandData);
};

#endif // #ifndef CAMERATHREAD_H