#ifndef NNBSSARTPROVIDER_H
#define NNBSSARTPROVIDER_H

// wxWidgets
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/string.h>

// Loam Logo(s)
#define NNBSSART_LOAM_LOGO_512P wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons", "LOAM_LOGO.png")

// NNBSS LOGO 
#define NNBSSART_LOGO_SMALL_PNG wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons", "NNBSS_LOGO_SMALL.png")
#define NNBSSART_LOGO_MEDIUM_PNG wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons", "NNBSS_LOGO_MEDIUM.png")
#define NNBSSART_LOGO_LARGE_PNG wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons", "NNBSS_LOGO_LARGE.png")

// Splash Screen
#define NNBSSART_SPLASHSCREEN wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Engine", "SplashScreen.png")

// Global
#define NNBSSART_GLOBAL_EXPORT_16x16p_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Global", "export_16x16p_idle.png")
#define NNBSSART_GLOBAL_EXPORT_16x16p_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Global", "export_16x16p_hover.png")
#define NNBSSART_GLOBAL_IMPORT_16x16p_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Global", "import_16x16p_idle.png")
#define NNBSSART_GLOBAL_IMPORT_16x16p_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Global", "import_16x16p_hover.png")
#define NNBSSART_GLOBAL_RESET_16x16p_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Global", "reset_16x16p_idle.png")
#define NNBSSART_GLOBAL_RESET_16x16p_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Global", "reset_16x16p_hover.png")
#define NNBSSART_GLOBAL_X_16x16p_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Global", "x_16x16p_idle.png")
#define NNBSSART_GLOBAL_X_16x16p_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Global", "x_16x16p_hover.png")
#define NNBSSART_GLOBAL_ADD_64x64p_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Global", "add_64x64p_idle.png")
#define NNBSSART_GLOBAL_ADD_64x64p_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Global", "add_64x64p_hover.png")

// Login
#define NNBSSART_WARNING_SMALL_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/LogIn", "warning_idle.png")

// Title bar
#define NNBSSART_HEADER_LOGO_PNG wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons", "NNBSS_LOGO.png")
#define NNBSSART_HEADER_X_BUTTON wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Engine/Symbols", "close.png")
#define NNBSSART_HEADER_RESTORE_BUTTON wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Engine/Symbols", "restore.png")
#define NNBSSART_HEADER_MINIMIZE_BUTTON wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Engine/Symbols", "minimize.png")

// Menu bar
#define NNBSSART_MENUBAR_SERVICES_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/MenuBar", "services_button_idle.png")
#define NNBSSART_MENUBAR_SERVICES_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/MenuBar", "services_button_hover.png")
#define NNBSSART_MENUBAR_OPTIONS_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/MenuBar", "settings_button_idle.png")
#define NNBSSART_MENUBAR_OPTIONS_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/MenuBar", "settings_button_hover.png")
#define NNBSSART_MENUBAR_LAYOUT_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/MenuBar", "layout_button_idle.png")
#define NNBSSART_MENUBAR_LAYOUT_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/MenuBar", "layout_button_hover.png")
#define NNBSSART_MENUBAR_ABOUT_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/MenuBar", "about_button_idle.png")
#define NNBSSART_MENUBAR_ABOUT_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/MenuBar", "about_button_hover.png")

// Services
#define NNBSSART_SERVICES_DEVICE_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "device_idle.png")
#define NNBSSART_SERVICES_DEVICE_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "device_hover.png")
#define NNBSSART_SERVICES_GRIDVIEW_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "grid_view_idle.png")
#define NNBSSART_SERVICES_GRIDVIEW_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "grid_view_hover.png")
#define NNBSSART_SERVICES_NOTIFICATIONS_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "notifications_idle.png")
#define NNBSSART_SERVICES_NOTIFICATIONS_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "notifications_hover.png")
#define NNBSSART_SERVICES_PLAYBACK_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "playback_idle.png")
#define NNBSSART_SERVICES_PLAYBACK_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "playback_hover.png")
#define NNBSSART_SERVICES_PROFILES_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "profiles_idle.png")
#define NNBSSART_SERVICES_PROFILES_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "profiles_hover.png")
#define NNBSSART_SERVICES_DETECTION_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "detection_idle.png")
#define NNBSSART_SERVICES_DETECTION_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Services", "detection_hover.png")

// Devices List
#define NNBSSART_DEVICESLIST_ADDDEVICE_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList", "add_device_idle.png")
#define NNBSSART_DEVICESLIST_ADDDEVICE_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList", "add_device_hover.png")
#define NNBSSART_DEVICESLIST_REMOVEDEVICE_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList", "remove_device_idle.png")
#define NNBSSART_DEVICESLIST_REMOVEDEVICE_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList", "remove_device_hover.png")
#define NNBSSART_DEVICESLIST_DIALOGADD_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList", "dialog_add_device_idle.png")
#define NNBSSART_DEVICESLIST_DIALOGADD_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList", "dialog_add_device_hover.png")
#define NNBSSART_DEVICESLIST_DIALOGADD_DISABLED wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList", "dialog_add_device_disabled.png")
#define NNBSSART_DEVICESLIST_DIALOGCANCEL_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList", "dialog_cancel_device_idle.png")
#define NNBSSART_DEVICESLIST_DIALOGCANCEL_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList", "dialog_cancel_device_hover.png")

#define NNBSSART_DEVICELIST_TREECTRL_ROOT_DEVICE_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList/DeviceListTree", "_root_devices_idle.png")
#define NNBSSART_DEVICELIST_TREECTRL_USB_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList/DeviceListTree", "usb_idle.png")
#define NNBSSART_DEVICELIST_TREECTRL_IPCAM_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList/DeviceListTree", "ip_cam_idle.png")
#define NNBSSART_DEVICELIST_TREECTRL_DVR_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList/DeviceListTree", "dvr_idle.png")
#define NNBSSART_DEVICELIST_TREECTRL_NVR_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList/DeviceListTree", "nvr_idle.png")
#define NNBSSART_DEVICELIST_TREECTRL_MICROPHONE_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/DevicesList/DeviceListTree", "microphone.png")

// Grid View
#define NNBSSART_GRIDVIEW_2x2_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/GridView", "gridview_2x2_idle.png")
#define NNBSSART_GRIDVIEW_4x4_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/GridView", "gridview_4x4_idle.png")
#define NNBSSART_GRIDVIEW_SETTINGS_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/GridView", "gridview_settings_idle.png")
#define NNBSSART_GRIDVIEW_EXPAND_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/GridView", "gridview_expand_idle.png")
#define NNBSSART_GRIDVIEW_CAMERA_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/GridView", "gridview_camera_idle.png")
#define NNBSSART_GRIDVIEW_CAMERA_SIGNAL_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/GridView", "gridview_camera_signal_idle.png")

// Playback
#define NNBSSART_PLAYBACK_FILMROLL_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "filmroll_32px32p_idle.png")
#define NNBSSART_PLAYBACK_BACKWARD_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "backward_32px32p_idle.png")
#define NNBSSART_PLAYBACK_BACKWARD_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "backward_32px32p_hover.png")
#define NNBSSART_PLAYBACK_PREVIOUS_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "previous_32px32p_idle.png")
#define NNBSSART_PLAYBACK_PREVIOUS_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "previous_32px32p_hover.png")
#define NNBSSART_PLAYBACK_PLAY_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "play_32px32p_idle.png")
#define NNBSSART_PLAYBACK_PLAY_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "play_32px32p_hover.png")
#define NNBSSART_PLAYBACK_PAUSE_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "pause_32px32p_idle.png")
#define NNBSSART_PLAYBACK_PAUSE_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "pause_32px32p_hover.png")
#define NNBSSART_PLAYBACK_STOP_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "stop_32px32p_idle.png")
#define NNBSSART_PLAYBACK_STOP_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "stop_32px32p_hover.png")
#define NNBSSART_PLAYBACK_NEXT_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "next_32px32p_idle.png")
#define NNBSSART_PLAYBACK_NEXT_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "next_32px32p_hover.png")
#define NNBSSART_PLAYBACK_FORWARD_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "forward_32px32p_idle.png")
#define NNBSSART_PLAYBACK_FORWARD_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Playback", "forward_32px32p_hover.png")

// Profiles
#define NNBSSART_PROFILES_FACEID_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Profiles", "face_scan_idle.png")
#define NNBSSART_PROFILES_FACEID_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Profiles", "face_scan_hover.png")
#define NNBSSART_PROFILES_MANAGEROLES_IDLE wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Profiles", "manage_roles_idle.png")
#define NNBSSART_PROFILES_MANAGEROLES_HOVER wxString::Format("%s/%s/%s", wxGetCwd(), "Assets/Textures/Icons/Profiles", "manage_roles_hover.png")


// Converts png to bitmap. It's enough to write path of the png file.
inline wxBitmap NNBSS_PNG_TO_BITMAP(const wxString& loadFile)
{
	wxBitmap bmp;
	bmp.LoadFile(loadFile, wxBITMAP_TYPE_PNG);

	return bmp;
}
// A shortcut to convert png to bitmap. It's enough to write path of the png file(wxString).
#define NNBSS_PNG2BMP(loadFile) NNBSS_PNG_TO_BITMAP(loadFile)

#endif // ! NNBSSARTPROVIDER_H