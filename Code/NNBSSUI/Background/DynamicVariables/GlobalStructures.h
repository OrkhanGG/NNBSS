#ifndef GLOBALSTRUCTURES_H
#define GLOBALSTRUCTURES_H

// Standard C Lib
#include <memory>
// OpenCV
#include <opencv2/imgproc.hpp>

class CNNBSSDeviceList;
class CNNBSSNotificationList;
class CNNBSSCameraGrid;
class CNNBSSDetectionSamplesList;
class CNNBSSPlaybackPanel;
class CNNBSSProfilesInterface;
class CNNBSSContentPanel;
class CNNBSSAIObjectDetection;
class CNNBSSMovingVisualMediaThread;

#define NNBSS_DEVICE_CONNECTION_STATUS_STRING_OFFLINE wxString(_("Offline"))
#define NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE wxString(_("Online"))
#define NNBSS_DEVICE_CONNECTION_STATUS_STRING_DISCONNECTED wxString(_("Disconnected"))
#define NNBSS_DEVICE_CONNECTION_STATUS_STRING_LOW_CONNECTION wxString(_("Low Connection"))

// This vector helps us to display Contents in Notebook tabs as a page that displays various contents as a service for user.
inline std::vector<CNNBSSContentPanel*> m_NNBSSContentPanelList;

// UI widget class list; CNNBSSDeviceList.
// This array just contains DeviceList Panel which's the class named CNNBSSDeviceList and it's a wxList based class.
// wxWindow* key of this map will contain the parent address(parent is CNNBSSContentPanel),
// so we may know that to which panel this device list belongs.
inline std::map<wxWindow*, CNNBSSDeviceList*> p_CNNBSSDeviceList;

// As the name of this struct says, it simply contains Moving Visual Media parameters.
// Moving Visual Media simply means video; any video file, IP camera stream, the frames retrieved from USB camera and etc...
// This struct may contain any MVM related data. Such as the name given by user or id and so on.
// Most importantly, as some variables/datas must be easily accessible by other threads (Mostly by MVM threads)
// the datas/variables that this struct contains will make easier to access by any thread.
struct SNNBSSMVMData
{
	// Device main props
	wxString    m_MVMName;
	wxString    m_MVMSourceAddress;	// Connection Address. Currently, can't be changed run-time. Mostly used for regular OpenCV connections but, consider NNBSS connections.
	int         m_MVMType{ -1 };// Device Type: USB, IP, VIDEO_FILE and so on.(Add some details)
	wxString    m_MVMConnectionStatus{ NNBSS_DEVICE_CONNECTION_STATUS_STRING_OFFLINE };// Current connection status. Some key-features may indirectly depend on this.

	// Device Special(Especially for USB Cameras)
	std::string m_DeviceNetName;
	std::string m_DevicePath;
	//-------------------------------------------

	// Secondary thread to manage all MVM related stuff.
	CNNBSSMovingVisualMediaThread*	m_MVMThread;
	// Communication object. I choose UX Frame to create communication among the existing UI objects.
	// Since the threads other than main may send events, some UI objects may need to recieve accurate and exact events.
	// Global Event Sink object will help us to create that communcation. Especially with CNNBSSMovingVisualMediaThread.
	wxEvtHandler*					p_GlobalEventSink = nullptr;

	// MVM Input Params -----------------------
	bool m_MVMUseDefault{false};
	int  m_MVMWidth{ 0 }, 
		 m_MVMHeight{ 0 }, 
		 m_MVMFPS{ 0 }; // set 0 to set default
	//-----------------------------------------

	// MVM Record Params ----------------------
	bool m_Record{false};
	int  m_RecordResolutionWidth{ 640 }, 
		 m_RecordResolutionHeight{ 480 }, 
		 m_RecordFPS{ 25 };
	std::string m_RecordEncoderName{};
	//-----------------------------------------

	// MVM Streaming Params -------------------
	bool m_Streaming{false};
	std::string m_StreamAddress, m_StreamAddressPort;
	int m_StreamResolutionWidth{ 640 }, m_StreamResolutionHeight{ 480 }, m_StreamFPS{ 25 }, m_StreamCompressPercentage{50}/*compression between 0-100, lower better performance, lack of quality*/;
	//--------------------------------------------

	// MVM Visual Params ----------------------
	bool   m_ImageProcessingShowHistogram{ false };
	bool   m_ImageProcessingActivate{ false };
	static constexpr int m_ImageProcessingBrightnessDefault = 0, m_ImageProcessingContrastDefault = 1;
	int    m_ImageProcessingBrightness{ m_ImageProcessingBrightnessDefault };
	double m_ImageProcessingContrast{ static_cast<double>(m_ImageProcessingContrastDefault) };

	bool m_ImageProcessingFlipHorizontal{ false }, m_ImageProcessingFlipVertical{ false };
	//--------------------------------------------

	// MVM Struct Operator(s)
	auto operator==(const SNNBSSMVMData& otherData) const
	{
		// Just these will be enough for validation
		if (m_MVMName != otherData.m_MVMName || m_MVMSourceAddress != otherData.m_MVMSourceAddress || m_DeviceNetName != otherData.m_DeviceNetName
			|| m_DevicePath != otherData.m_DevicePath)
			return false;
		else
			return true;
	}
};
// Contains the data that belongs to devices.
// Iteration may be done by the help of any members of SNNBSSMVMData struct.
// Just compare with DeviceList array's members to get accurate results.
inline std::vector<SNNBSSMVMData*> SCNNBSSMVMParametersList;

// By the help of CNNBSSContentPanel, we find CNNBSSCameraGrid.
// As it is the child of CNNBSSContentPanel
inline std::map<CNNBSSContentPanel*, CNNBSSCameraGrid*> CNNBSSCameraGridList;

namespace NNBSSNotifications
{

	// Notification Attrs
	struct SNNBSSNotificationAttributes
	{
		wxString DetectionName;
		wxString DateAndTime;
		wxString Message;
		wxString AdditionalNotes;

		cv::Mat ROI;// Region of interest will be saved as thumbnail to be displayed whenever user desire

		// wxBitmap or wxImage to display notification moment
	};

	inline std::vector<SNNBSSNotificationAttributes> SNNBSSNotificationAttributesList;
	inline std::map<wxWindow*, CNNBSSNotificationList*> p_CNNBSSNotificationList;
}

namespace NNBSSDetectionModel
{
	// To be used in array which will be checked by threads so they may perform detection for their device
	struct SCNNBSSDeviceAssignedDetectionModels
	{
		// Each assigned classifier will be iterated
		// according to Device Params.
		// So, (in our case) mentioning them here is necessarry.

		wxString DeviceName{ wxString() };
		wxString Address{ wxString() };

		// We don't need these, do we?
		//int DeviceType{ -1 };
		//wxString DeviceConnectionStatus{ NNBSS_DEVICE_CONNECTION_STATUS_STRING_OFFLINE };

		// USB Camera Special
		std::string DeviceNetName{ std::string() };
		std::string DevicePath{ std::string() };

		// Detection Model
		wxString _Label;
		std::string _AssignedModelPath;
		std::string _NotificationMessage;
		std::string _AdditionalUserNotes;
		double m_arrayScaleFactor = 1.0;
		double m_scaleFactor = 1.1;
		int m_minNeighbors = 3;
		//int m_flags = 0;
		wxSize m_minSize = wxSize();
		wxSize m_maxSize = wxSize();

		bool Works{ false };

		auto operator==(const SCNNBSSDeviceAssignedDetectionModels& otherData) const
		{
			if (DeviceName != otherData.DeviceName || Address != otherData.Address || DeviceNetName != otherData.DeviceNetName
				|| DevicePath != otherData.DevicePath || _AssignedModelPath != otherData._AssignedModelPath
				|| m_arrayScaleFactor != otherData.m_arrayScaleFactor || m_scaleFactor != otherData.m_scaleFactor
				|| m_minNeighbors != otherData.m_minNeighbors || m_minSize != otherData.m_minSize || m_maxSize != otherData.m_maxSize
				|| Works != otherData.Works)
				return false;
			else
				return true;
		}
	};

	// Stores device addresses with detection model information
	// We check this to know that which detection models are assigned to device once we need
	inline std::vector<SCNNBSSDeviceAssignedDetectionModels> SCNNBSSAssignedDetectionModelsList;
	// Base class for object detection
	// It's a bind from NNBSSAI to NNBSSUI
	inline std::map<wxString, CNNBSSAIObjectDetection*> p_CNNBSSAIObjectDetectionBase;
	// Data-only struct. To be used in an array.
	struct SNNBSSCascadeClassifierCacheData
	{
		wxString FileName;
		wxString Label;
		// Note: Path contains file name
		wxString Path;
		bool Works{ false };

		// UI Related
		wxString DisplayName;
		int Position;// Position on grid
	};

	// Contains all added cascade classifiers
	inline std::vector<SNNBSSCascadeClassifierCacheData> CNNBSSDetectionModelDataList;

	// By the help of CNNBSSContentPanel, we find CNNBSSDetectionSamplesList.
	// As it is the child of CNNBSSContentPanel
	inline std::map<CNNBSSContentPanel*, CNNBSSDetectionSamplesList*> _CNNBSSDetectionSamplesList;
}

// By the help of CNNBSSContentPanel, we find CNNBSSPlaybackPanel.
// As it is the child of CNNBSSContentPanel
inline std::map<CNNBSSContentPanel*, CNNBSSPlaybackPanel*> _CNNBSSPlaybackPanelList;

// Profiles Interface
inline std::map<CNNBSSContentPanel*, CNNBSSProfilesInterface*> _CNNBSSProfilesInterfaceList;

#endif // ! GLOBALSTRUCTURES_H