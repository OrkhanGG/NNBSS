///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef GLOBALDECLARATIONS_H
#define GLOBALDECLARATIONS_H

// NNBSS UI
#include <NNBSSUI/GUI/ArtProvider/NNBSSArtProvider.h>

inline const auto CORP_NAME = std::string("LOAM");
inline const auto ENGINE_NAME = std::string("Neural Network Based Security Systems");
inline const auto ENGINE_VERSION = std::string("0.1 Alpha");

#define NNBSS_VERSION_MAJOR 0
#define NNBSS_VERSION_MINOR 1
#define NNBSS_VERSION_REVISION 0

// NNBSS Version Check
#define CHECK_NNBSS_VERSION(major,minor,revision) \
    (NNBSS_VERSION_MAJOR >  (major) || \
    (NNBSS_VERSION_MAJOR == (major) && NNBSS_VERSION_MINOR >  (minor)) || \
    (NNBSS_VERSION_MAJOR == (major) && NNBSS_VERSION_MINOR == (minor) && NNBSS_VERSION_REVISION >= (revision)))


// OpenCV Version Check
#define CHECK_OPENCV_VERSION(major,minor,revision) \
    (CV_VERSION_MAJOR >  (major) || \
    (CV_VERSION_MAJOR == (major) && CV_VERSION_MINOR >  (minor)) || \
    (CV_VERSION_MAJOR == (major) && CV_VERSION_MINOR == (minor) && CV_VERSION_REVISION >= (revision)))

#define DEBUG_TRACE_HELPER "TRACE_HELPER"

inline const int NNBSSMAXELEMENT = 10000;

// GridView related
#define NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER 8
#define NNBSS_GRIDVIEW_MAX_CAMERA_COUNT (NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER*NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER)

// DetectionSamplesList related
#define NNBSS_DETECTIONSAMPLES_MAX_COUNT 64

// Paths/directories
	// Const vars
inline const std::string NNBSS_PROJECT_DIRECTORY = wxGetCwd().mb_str();
inline const std::string NNBSS_DETECTIONMODELS_DIRECTORY = "\\Assets\\Scripts\\AI\\DetectionModels";
inline const std::string NNBSS_SEPARATOR = "\\";
// Macros
inline const auto NNBSS_DIR__FACIALDATABASE_DIR = wxString::Format(wxGetCwd() + "\\Data\\Records\\FacialDatabase\\");
inline const auto NNBSS_DIR__RECORDS_DIR = wxString::Format(wxGetCwd() + "\\Data\\Records\\");
inline const auto NNBSS_DIR__PLAYBACK_DIR = wxString::Format(wxGetCwd() + "\\Data\\Playback\\");
inline const auto NNBSS_DIR__FACIALDATABASE_XML_PATH = wxString::Format(wxGetCwd() + "\\Data\\Records\\FacialDatabase\\FDatasetConfig.xml");
inline const auto NNBSS_DIR__PROFILES_XML_PATH = wxString::Format(wxGetCwd() + "\\Data\\Records\\Profiles\\Profiles.xml");

// DeviceStatusBar special
inline const auto NNBSS_STATUSBAR_CPU_BARNAME = wxT("BARNAME_CPU_GENERAL");
inline const auto NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME = wxT("BARNAME_VIRTUALMEMORY_GENERAL");
inline const auto NNBSS_STATUSBAR_RAM_BARNAME = wxT("BARNAME_RAM_GENERAL");
inline const auto NNBSS_STATUSBAR_MEMORY_BARNAME = wxT("BARNAME_MEMORY_GENERAL");
inline const auto NNBSS_STATUSBAR_DISKSPACE_BARNAME = wxT("BARNAME_DISKSPACE_GENERAL");

typedef enum
{
	NNBSS_CONTENTPAGE_CURRENTCONTENT_SERVICES = 6100,
	NNBSS_CONTENTPAGE_CURRENTCONTENT_DEVICES,
	NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW,
	NNBSS_CONTENTPAGE_CURRENTCONTENT_NOTIFICATIONS,
	NNBSS_CONTENTPAGE_CURRENTCONTENT_PLAYBACK,
	NNBSS_CONTENTPAGE_CURRENTCONTENT_PROFILES,
	NNBSS_CONTENTPAGE_CURRENTCONTENT_DETECTION
}CONTENTPAGE_CURRENTCONTENT;

// Events
typedef enum
{
	NNBSS_DESTROY_ALL = 4000,

	NNBSS_EVT_OK = 4500,
	NNBSS_EVT_CANCEL,
	NNBSS_EVT_RESET,

	NNBSS_EVT_ADD_SERVICES_PAGE,
	NNBSS_EVT_SHOW_OPTIONS,
	NNBSS_EVT_SHOW_LAYOUT_OPTIONS,
	NNBSS_EVT_SHOW_ABOUT_US,

	NNBSS_EVT_SHOW_DEVICES_PANEL,
	NNBSS_EVT_SHOW_GRIDVIEW_PANEL,
	NNBSS_EVT_SHOW_NOTIFICATIONS_PANEL,
	NNBSS_EVT_SHOW_PLAYBACK_PANEL,
	NNBSS_EVT_SHOW_PROFILES_PANEL,
	NNBSS_EVT_SHOW_DETECTIONCONTENT_PANEL,

	NNBSS_EVT_ADD_DEVICE_DIALOG,
	NNBSS_EVT_REMOVE_DEVICE_DIALOG,

	NNBSS_EVT_GRIDVIEW_2x2,
	NNBSS_EVT_GRIDVIEW_FULLSCREEN,
	NNBSS_EVT_GRIDVIEW_4x4,
	NNBSS_EVT_GRIDVIEW_CUSTOMIZE,

	// CNNBSSHardware specials
	NNBSS_PORT_EVT_ON_STANCE_CHANGE,
	NNBSS_PORT_EVT_ON_MOUNT_USB_CAMERA,		 
	NNBSS_PORT_EVT_ON_UNMOUNT_USB_CAMERA,	 
	NNBSS_PORT_EVT_ON_MOUNT_MOUSE,			 
	NNBSS_PORT_EVT_ON_MOUNT_KEYBOARD,		 
	NNBSS_PORT_EVT_ON_MOUNT_MIC,			 
	NNBSS_PORT_EVT_ON_MOUNT_UNKNOWN,         
}NNBSS_EVENT_ID;


/* Moving Visual Media types. Once any moving visual media is added to SNNBSSMVMParametersList,
  one of the followings must be assigned to its member, MVMType.
  Device type is the keypoint of MovingVisualMedia. Most of MVM related code will be executed depending on that.*/
typedef enum
{
	NNBSS_MVM_TYPE_START = 6000,// Neither modify nor delete!

	NNBSS_MVM_TYPE_USBCAM,
	NNBSS_MVM_TYPE_IPCAM,
	NNBSS_MVM_TYPE_DVR,
	NNBSS_MVM_TYPE_NVR,
	NNBSS_MVM_TYPE_MICROPHONE,
	NNBSS_MVM_TYPE_VIDEO,

	NNBSS_MVM_TYPE_END,// Neither modify nor delete!
}NNBSS_MVM_TYPE;
/* DEVICE TYPE COUNT IS THE ROW COUNT OF DEVICE TYPES OF THE ADD DEVICES DIALOG.
  IN OTHER WORDS, IT IS THE COUNT OF HOW MANY DEVICE TYPES WILL BE DISPLAYED FOR USER.
  SO, DECREASING/INCREASING THIS DOES NOT CAUSE ANYTHING ELSE BUT VISUAL CHANGE.*/
inline const int NNBSS_EVT_DEVICETYPE_COUNT = 2;


// Colours
class wxColour;
// Black
inline const wxColour RGB_COLOURDEF_BLACK = wxColour(28, 36, 44);
	// Some Unsorted Triples
inline const wxColour RGB_COLOURDEF_TRIPLE10 = wxColour(10, 10, 10);
inline const wxColour RGB_COLOURDEF_TRIPLE20 = wxColour(23,29,34);
inline const wxColour RGB_COLOURDEF_TRIPLE25 = wxColour(25, 25, 25);
inline const wxColour RGB_COLOURDEF_TRIPLE30 = wxColour(30, 30, 30);
inline const wxColour RGB_COLOURDEF_TRIPLE35 = wxColour(35, 35, 35);
inline const wxColour RGB_COLOURDEF_TRIPLE40 = wxColour(40, 40, 40);
inline const wxColour RGB_COLOURDEF_TRIPLE45 = wxColour(45, 45, 45);
inline const wxColour RGB_COLOURDEF_TRIPLE50 = wxColour(50, 50, 50);
inline const wxColour RGB_COLOURDEF_TRIPLE100 = wxColour(100, 100, 100);
inline const wxColour RGB_COLOURDEF_TRIPLE140 = wxColour(140, 140, 140);
inline const wxColour RGB_COLOURDEF_TRIPLE240 = wxColour(240, 240, 240);
inline const wxColour RGB_COLOURDEF_WHITE = wxColour(10, 165, 154);
// Mixed
inline const wxColour RGB_COLOURDEF_PLACEHOLDER = wxColour(28,36,44);
inline const wxColour RGB_COLOURDEF_REDDENSE = wxColour(80, 30, 30);
inline const wxColour RGB_COLOURDEF_RED = wxColour(255, 0, 0);
inline const wxColour  RGB_COLOURDEF_CORNDENSE = wxColour(80, 60, 30);
inline const wxColour  RGB_COLOURDEF_LIKEORANGE = wxColour(255, 102, 0);
inline const wxColour RGB_COLOURDEF_NNBSSBLUE = wxColour(RGB_COLOURDEF_WHITE);
inline const wxColour RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT = wxColour(RGB_COLOURDEF_WHITE);
inline const wxColour  RGB_COLOURDEF_NNBSSRED = wxColour(201, 15, 2);
inline const wxColour RGB_COLOURDEF_NNBSSPURPLE = wxColour(135, 0, 128);
// Grays
inline const wxColour RGB_COLOURDEF_JET = wxColour(52, 52, 52);
inline const wxColour RGB_COLOURDEF_DAVYSGRAY = wxColour(85, 85, 85);
inline const wxColour RGB_COLOURDEF_GRAY = wxColour(128, 128, 128);
inline const wxColour RGB_COLOURDEF_GAINSBORO = wxColour(220, 220, 220);
inline const wxColour RGB_COLOURDEF_GAINSBOROLIGHT = wxColour(247, 247, 247);
// Yellows
inline const wxColour RGB_COLOURDEF_YELLOWNCS = wxColour(255, 211, 0);
inline const wxColour RGB_COLOURDEF_YELLOW = wxColour(255, 255, 0);


#endif // ! GLOBALDECLARATIONS_H