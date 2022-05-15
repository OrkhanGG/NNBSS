///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#ifndef CLASSMANAGER_H
#define CLASSMANAGER_H

// This project contains some of these kinds of header files. I call these manager headers.
// Sector folders that have under the NNBSS syllable such as NNBSSAI, NNBSSRender and etc... contain Management folders.
// Management folders contain this kind of header and they all contain macro-like functions and function-like macros.
// The aim is to create better accessibility among code files of NNBSS.
// I also think this way I do minimize the risk of crashes caused by null pointers.

// Standart C Libs
#include <tuple>
#include <list>
#include <map>
#include <exception>
#include <algorithm>
// wxWidgets
#include <wx/app.h>
// NNBSS UI
#include <NNBSSUI/GlobalDeclarations.h>
#include <NNBSSUI/Background/DynamicVariables/GlobalStructures.h>
#include <NNBSSUI/Bridge/AbstractUIEvents.h>
// Main
#include <NNBSSUI/Background/NNBSSUX/NNBSSUX.h>
#include <NNBSSUI/Background/Management/AttachmentUnitInterface/AUIManager.h>
#include <NNBSSUI/Bridge/Behaviour/LineFlowGlobal.h>
// GUI
	// Objects
#include <NNBSSUI/GUI/Objects/LogIn/LogIn.h>
#include <NNBSSUI/GUI/Objects/TitleBar/TitleBar.h>
#include <NNBSSUI/GUI/Objects/DeviceProperties/DeviceProperties.h>
		// Menu Bar & its pages
#include <NNBSSUI/GUI/Objects/MenuBar/Pages/ViewPage.h>
#include <NNBSSUI/GUI/Objects/MenuBar/Pages/AboutPage.h>
#include <NNBSSUI/GUI/Objects/Notifications/NotificationsManager.h>
#include <NNBSSUI/GUI/Objects/DeviceList/DeviceList.h>
#include <NNBSSUI/GUI/Objects/NotificationList/NotificationList.h>
#include <NNBSSUI/GUI/Objects/Dialogs/AddDeviceDialog/AddDeviceDialog.h>
#include <NNBSSUI/GUI/Objects/Dialogs/AddCameraDialog/AddCameraDialog.h>
#include <NNBSSUI/GUI/Objects/Dialogs/GridViewEditDialog/GridViewEditDialog.h>
		// CameraGrid
#include <NNBSSUI/GUI/Objects/CameraGrid/cameragrid.h>
		// Playback
#include <NNBSSUI/GUI/Objects/Playback/playback.h>
		// Profiles
#include <NNBSSUI/GUI/Objects/Profiles/Profiles.h>
		// DetectionSamplesList
#include <NNBSSUI/GUI/Objects/DetectionSamplesList/DetectionSamplesList.h>
		// Standalone Frames
#include <NNBSSUI/GUI/Objects/StandaloneFrames/StandaloneCameraFrame/StandaloneCameraFrame.h>
		// Bitmap Panel
#include <NNBSSUI/GUI/Objects/BitmapPanel/BitmapPanel.h>
	// Components
#include <NNBSSUI/GUI/Components/ContentPanel/ContentPanel.h>
#include <NNBSSUI/GUI/Components/ControlPanel/ControlPanel.h>
#include <NNBSSUI/GUI/Components/DeviceStatusBar/DeviceStatusBar.h>
// Core
#include <NNBSSUI/Core/Hardware.h>
#include <NNBSSUI/Core/WorkerThread.h>

inline CNNBSSHardware* CNNBSSHardwareAddressHandle()
{
	static CNNBSSHardware* p_CNNBSSHardware = nullptr;

	if (!p_CNNBSSHardware)
	{
		p_CNNBSSHardware = new CNNBSSHardware();
	}

	return p_CNNBSSHardware;
}

inline CNNBSSHardwareUIThread* CNNBSSHardwareUIThreadAddressHandle(long sleepTime = 500L, bool destroy = false)
{
	static CNNBSSHardwareUIThread* p_CNNBSSHardwareUIThread;

	if (destroy)
	{
		if (p_CNNBSSHardwareUIThread)
		{
			p_CNNBSSHardwareUIThread->Delete();
			delete p_CNNBSSHardwareUIThread;
			//p_CNNBSSHardwareUIThread = nullptr;
		}
		return nullptr;
	}

	if (p_CNNBSSHardwareUIThread)
		return p_CNNBSSHardwareUIThread;
	else
		p_CNNBSSHardwareUIThread = new CNNBSSHardwareUIThread();

	return p_CNNBSSHardwareUIThread;
}

inline CNNBSSProfilesInterface* CNNBSSProfilesInterfaceAddressHandle(CNNBSSContentPanel* _CONTENTPANEL_, bool destroy = false)
{
	if (!_CONTENTPANEL_)
		return nullptr;

	auto itr = _CNNBSSProfilesInterfaceList.find(_CONTENTPANEL_);

	if (itr != _CNNBSSProfilesInterfaceList.end())
	{
		if (destroy)
		{
			_CNNBSSProfilesInterfaceList.erase(itr);
			return nullptr;
		}

		// found
		return itr->second;
	}
	else
	{
		if (destroy)
		{
			return nullptr;
		}
		// if not found
		_CNNBSSProfilesInterfaceList[_CONTENTPANEL_] = new CNNBSSProfilesInterface(_CONTENTPANEL_);
		return _CNNBSSProfilesInterfaceList[_CONTENTPANEL_];
	}

	return nullptr;
}

inline CNNBSSPlaybackPanel* CNNBSSPlaybackPanelAddressHandle(CNNBSSContentPanel* _CONTENTPANEL_, bool destroy = false)
{
	if (!_CONTENTPANEL_)
		return nullptr;

	auto itr = _CNNBSSPlaybackPanelList.find(_CONTENTPANEL_);

	if (itr != _CNNBSSPlaybackPanelList.end())
	{
		if (destroy)
		{
			_CNNBSSPlaybackPanelList.erase(itr);
			return nullptr;
		}

		// found
		return itr->second;
	}
	else
	{
		if (destroy)
		{
			return nullptr;
		}
		// if not found
		_CNNBSSPlaybackPanelList[_CONTENTPANEL_] = new CNNBSSPlaybackPanel(_CONTENTPANEL_);
		return _CNNBSSPlaybackPanelList[_CONTENTPANEL_];
	}

	return nullptr;
}

inline CNNBSSDetectionSamplesList* CNNBSSDetectionSamplesListAddressHandle(CNNBSSContentPanel* _CONTENTPANEL_, bool destroy = false)
{
	if (!_CONTENTPANEL_)
		return nullptr;

	auto itr = NNBSSDetectionModel::_CNNBSSDetectionSamplesList.find(_CONTENTPANEL_);

	if (itr != NNBSSDetectionModel::_CNNBSSDetectionSamplesList.end())
	{
		if (destroy)
		{
			NNBSSDetectionModel::_CNNBSSDetectionSamplesList.erase(itr);
			return nullptr;
		}

		// found
		return itr->second;
	}
	else
	{
		if (destroy)
		{
			return nullptr;
		}
		// if not found
		NNBSSDetectionModel::_CNNBSSDetectionSamplesList[_CONTENTPANEL_] = new CNNBSSDetectionSamplesList(_CONTENTPANEL_);
		return NNBSSDetectionModel::_CNNBSSDetectionSamplesList[_CONTENTPANEL_];
	}

	return nullptr;
}

// One of the arguments (except destroy) always must be filled according to needs.
// Parent is given when CameraGrid has to be created and no need for pageID in this case. Do not evaluate parent unless you need to create new instance(commonly created when gridview open command is called by AbstractUIEvents)
// pageID is given when present CameraGrid must be accessed. PageID always should be the value of an existing page that contains CameraGrid
// Destroy may be called right after the page close. Otherwise unexpected results may happen.
inline CNNBSSCameraGrid* CNNBSSCameraGridAddressHandle(CNNBSSContentPanel* _CONTENTPANEL_, bool destroy = false)
{
	if (!_CONTENTPANEL_)
		return nullptr;

	auto itr = CNNBSSCameraGridList.find(_CONTENTPANEL_);

	if (itr != CNNBSSCameraGridList.end())
	{
		if (destroy)
		{
			CNNBSSCameraGridList.erase(itr);
			return nullptr;
		}

		// found
		return itr->second;
	}
	else
	{
		if (destroy)
		{
			return nullptr;
		}
		// not found
		CNNBSSCameraGridList[_CONTENTPANEL_] = new CNNBSSCameraGrid(_CONTENTPANEL_);
		return CNNBSSCameraGridList[_CONTENTPANEL_];
	}

	return nullptr;
}

inline CNNBSSDeviceStatusBar* CNNBSSDeviceStatusBarAddressHandle()
{
	static CNNBSSDeviceStatusBar* p_CNNBSSDeviceStatusBar;

	if (!p_CNNBSSDeviceStatusBar)
	{
		p_CNNBSSDeviceStatusBar = new CNNBSSDeviceStatusBar();
	}

	return p_CNNBSSDeviceStatusBar;
}

inline CNNBSSStatusProgressBar* CNNBSSStatusProgressBarDeviceStatusAddressHandle(wxWindow* parent, wxString barName, bool destroy = false)
{
	static std::map<wxString, CNNBSSStatusProgressBar*> p_CNNBSSStatusProgressBar;

	if (destroy)
	{
		p_CNNBSSStatusProgressBar.erase(p_CNNBSSStatusProgressBar.find(barName));
	}

	// First, try to find the elemnt with given name
	auto itr = p_CNNBSSStatusProgressBar.find(barName);
	if (itr == p_CNNBSSStatusProgressBar.end())
	{
		if (!parent)
		{
			return nullptr;
			// don't use this since we have this in a thread with try/catch
			if (0)
			{
				// Parent is not given and barName does not exist in the list, throw exception
				throw std::runtime_error(std::string("Invalid use of CNNBSSStatusProgressBar. Both parent and barName is invalid. To create, use parent. To find existing, use nullptr parent and existing barName."));
			}
		}
	}
	else
	{
		return itr->second;
	}

	// If not found, check if parent is not nullptr
	if (parent)
	{
		p_CNNBSSStatusProgressBar[barName] = new CNNBSSStatusProgressBar(parent);
		return p_CNNBSSStatusProgressBar[barName];
	}

	return nullptr;
}

inline CNNBSSDevicePropertiesDialog* CNNBSSDevicePropertiesDialogAddressHandle(SNNBSSMVMData* DeviceParams,
	wxWindow* parent = NULL, bool onDestroy = false, wxWindowID id = wxID_ANY,
	const wxString& title = wxEmptyString,
	const wxPoint& pos = wxDefaultPosition,
	const wxSize& size = wxDefaultSize,
	long style = 0L,
	const wxString& name = wxT("DefaultDevicePropertiesDialogName"))
{
	static CNNBSSDevicePropertiesDialog* p_CNNBSSDevicePropertiesDialog;

	if (onDestroy)
	{
		if (p_CNNBSSDevicePropertiesDialog)
			p_CNNBSSDevicePropertiesDialog = nullptr;
		return nullptr;
	}

	if (p_CNNBSSDevicePropertiesDialog)
		return p_CNNBSSDevicePropertiesDialog;
	else
		p_CNNBSSDevicePropertiesDialog = new CNNBSSDevicePropertiesDialog(DeviceParams, parent, id, title, pos, size, style, name);

	return p_CNNBSSDevicePropertiesDialog;

}

inline CNNBSSGridViewEditDialog* CNNBSSGridViewEditDialogAddressHandle(bool onDestroy = false)
{
	static CNNBSSGridViewEditDialog* p_CNNBSSGridViewEditDialog;

	if (onDestroy)
	{
		if (p_CNNBSSGridViewEditDialog)
			p_CNNBSSGridViewEditDialog = nullptr;
		return nullptr;
	}

	if (p_CNNBSSGridViewEditDialog)
		return p_CNNBSSGridViewEditDialog;
	else
		p_CNNBSSGridViewEditDialog = new CNNBSSGridViewEditDialog();

	return p_CNNBSSGridViewEditDialog;

}

inline CNNBSSAddCameraDialog* CNNBSSAddCameraDialogAddressHandle(bool onDestroy = false)
{
	static CNNBSSAddCameraDialog* p_CNNBSSAddCameraDialog;

	if (onDestroy)
	{
		if (p_CNNBSSAddCameraDialog)
			p_CNNBSSAddCameraDialog = nullptr;
		return nullptr;
	}

	if (p_CNNBSSAddCameraDialog)
		return p_CNNBSSAddCameraDialog;
	else
		p_CNNBSSAddCameraDialog = new CNNBSSAddCameraDialog();

	return p_CNNBSSAddCameraDialog;

}

inline CNNBSSAddDeviceDialog* CNNBSSAddDeviceDialogAddressHandle(bool onDestroy = false)
{
	static CNNBSSAddDeviceDialog* p_CNNBSSAddDeviceDialog;

	if (onDestroy)
	{
		if (p_CNNBSSAddDeviceDialog)
			p_CNNBSSAddDeviceDialog = nullptr;
		return nullptr;
	}

	if (p_CNNBSSAddDeviceDialog)
		return p_CNNBSSAddDeviceDialog;
	else
		p_CNNBSSAddDeviceDialog = new CNNBSSAddDeviceDialog();

	return p_CNNBSSAddDeviceDialog;

}

inline CNNBSSNotificationList* CNNBSSNotificationListAddressHandle(CNNBSSContentPanel* _CONTENTPANEL_, bool destroy = false)
{
	if (!_CONTENTPANEL_)
		return nullptr;

	auto itr = NNBSSNotifications::p_CNNBSSNotificationList.find(_CONTENTPANEL_);

	if (itr != NNBSSNotifications::p_CNNBSSNotificationList.end())
	{
		if (destroy)
		{
			NNBSSNotifications::p_CNNBSSNotificationList.erase(itr);
			return nullptr;
		}

		// found
		return itr->second;
	}
	else
	{
		if (destroy)
		{
			return nullptr;
		}
		// not found
		NNBSSNotifications::p_CNNBSSNotificationList[_CONTENTPANEL_] = new CNNBSSNotificationList();
		return NNBSSNotifications::p_CNNBSSNotificationList[_CONTENTPANEL_];
	}

	return nullptr;
}

// This function's purpose is to get the DeviceList UI widget/control according to _CONTENTPANEL_.
// Explanation:
// Since when a new tab is created, it requires a wxWindow*, we do use a special class called CNNBSSContentPanel
// to store some potential pages within. By potential, I mean: Whether you call or not, if a request is sent to
// CNNBSSControlPanel to create a new page/tab, it'll immediately open The Welcome/Services page of CNNBSSContentPanel.
// And user have multiple page options to get into. Since there's no something like "goto previous page", I call it as potential
// (destroy does not exactly deallocate the memory since it'll be done by wxWidgets itself. We just make erase stuff.)
inline CNNBSSDeviceList* CNNBSSDeviceListAddressHandle(CNNBSSContentPanel* _CONTENTPANEL_, bool destroy = false)
{
	if (!_CONTENTPANEL_)
		return nullptr;

	auto itr = p_CNNBSSDeviceList.find(_CONTENTPANEL_);

	if (itr != p_CNNBSSDeviceList.end())
	{
		if (destroy)
		{
			p_CNNBSSDeviceList.erase(itr);
			return nullptr;
		}

		// found
		return itr->second;
	}
	else
	{
		if (destroy)
		{
			return nullptr;
		}
		// not found
		p_CNNBSSDeviceList[_CONTENTPANEL_] = new CNNBSSDeviceList();
		return p_CNNBSSDeviceList[_CONTENTPANEL_];
	}

	return nullptr;
}

inline CNNBSSDeviceListTreeCtrl* CNNBSSDeviceListTreeCtrlAddressHandle()
{
	static CNNBSSDeviceListTreeCtrl* p_CNNBSSDeviceListTreeCtrl = nullptr;

	if (!p_CNNBSSDeviceListTreeCtrl)
		p_CNNBSSDeviceListTreeCtrl = new CNNBSSDeviceListTreeCtrl();

	return p_CNNBSSDeviceListTreeCtrl;
}
// This function as other AddressHandlers is a manager to get or create the related class(CNNBSSContentPanel in this case)
// It iterates through the related list to check whether the _CONTENTPANEL_ is found or not.
// If _CONTENTPANEL_ was not found, it creates and inserts the parent(if the given _CONTENTPANEL_ is not null)
// Otherwise just returns the _CONTENTPANEL_.
// If destroy is called, the given _CONTENTPANEL_ will be erased and nullptr will be returned.
inline CNNBSSContentPanel* CNNBSSContentPanelAddressHandle(CNNBSSContentPanel* _CONTENTPANEL_, bool destroy = false)
{
	if (!_CONTENTPANEL_)
	{
		return nullptr;
	}

	// clear once on start(is not needed actually)
	{
		static bool onceGate = true;
		if (onceGate)
		{
			onceGate = false;
			m_NNBSSContentPanelList.clear();
		}
	}

	int index = 0;
	for (const auto& p : m_NNBSSContentPanelList)
	{
		index++;
		if (_CONTENTPANEL_ == p)
		{
			if (destroy)
			{
				auto itr = std::find(m_NNBSSContentPanelList.begin(),
					m_NNBSSContentPanelList.end(), _CONTENTPANEL_);
				if (itr != m_NNBSSContentPanelList.end())
				{
					// if found
					m_NNBSSContentPanelList.erase(itr);
				}
				return nullptr;
			}
			else
			{
				return p;
			}
		}
	}

	// if not found
	m_NNBSSContentPanelList.emplace_back(_CONTENTPANEL_);

	return m_NNBSSContentPanelList[m_NNBSSContentPanelList.size() - 1];
}

inline CNNBSSControlPanel* CNNBSSControlPanelAddressHandle(bool destroy = false)
{
	static CNNBSSControlPanel* p_CNNBSSControlPanel;

	if (destroy)
	{
		if (p_CNNBSSControlPanel)
			p_CNNBSSControlPanel->~CNNBSSControlPanel();
		return nullptr;
	}

	if (p_CNNBSSControlPanel)
		return p_CNNBSSControlPanel;
	else
		p_CNNBSSControlPanel = new CNNBSSControlPanel();

	return p_CNNBSSControlPanel;
}

inline CNNBSSAuiManager* CNNBSSAuiManagerAddressHandle()
{
	static CNNBSSAuiManager* p_CNNBSSAuiManager;

	if (p_CNNBSSAuiManager)
	{
		return p_CNNBSSAuiManager;
	}
	else
	{
		p_CNNBSSAuiManager = new CNNBSSAuiManager();
	}

	return p_CNNBSSAuiManager;
}

inline CNNBSSLineFlow* CNNBSSLineFlowAddressHandle(bool destroy = false)
{
	static CNNBSSLineFlow* p_CNNBSSLineFlow;

	if (!p_CNNBSSLineFlow)
	{
		p_CNNBSSLineFlow = new CNNBSSLineFlow;
	}

	if (destroy)
	{
		if (p_CNNBSSLineFlow)
		{
			delete p_CNNBSSLineFlow;
			return nullptr;
		}
		else
		{
			// nullptr can't be destroyed/deleted!
			NNBSSErrorShow("System Error: nullptr can't be destroyed! -CNNBSSLineFlow-", 100);
			return nullptr;
		}
	}

	return p_CNNBSSLineFlow;
}

inline CNNBSS_SC_TitleBar* CNNBSS_SC_TitleBarAddressHandle(bool destroy = false)
{
	static CNNBSS_SC_TitleBar* p_CNNBSS_SC_TitleBar;

	if (!p_CNNBSS_SC_TitleBar)
	{
		p_CNNBSS_SC_TitleBar = new CNNBSS_SC_TitleBar;
	}

	if (destroy)
	{
		if (p_CNNBSS_SC_TitleBar)
		{
			if (p_CNNBSS_SC_TitleBar->GetPanel()->GetChildren().GetCount() > 0)
				p_CNNBSS_SC_TitleBar->GetPanel()->DestroyChildren();
			p_CNNBSS_SC_TitleBar->GetPanel()->Destroy();
			return nullptr;
		}
		else
		{
			// nullptr can't be destroyed/deleted!
			NNBSSErrorShow("System Error: nullptr can't be destroyed! -CNNBSS_SC_TitleBar-", 100);
			return nullptr;
		}
	}

	return p_CNNBSS_SC_TitleBar;
}

inline CNNBSSMouseEvents* CNNBSSMouseEventsAddressHandle(bool destroy = false)
{
	static CNNBSSMouseEvents* p_CNNBSSMouseEvents;

	if (!p_CNNBSSMouseEvents)
	{
		p_CNNBSSMouseEvents = new CNNBSSMouseEvents;
	}

	if (destroy)
	{
		if (p_CNNBSSMouseEvents)
		{
			delete p_CNNBSSMouseEvents;
			return nullptr;
		}
		else
		{
			// nullptr can't be destroyed/deleted!
			NNBSSErrorShow("System Error: nullptr can't be destroyed! -CNNBSSMouseEvents-", 100);
			return nullptr;
		}
	}

	return p_CNNBSSMouseEvents;
}

inline CNNBSSLogIn* CNNBSSLogInAddressHandle(bool destroy = false)
{
	static CNNBSSLogIn* p_CNNBSSLogIn;

	if (!p_CNNBSSLogIn)
	{
		p_CNNBSSLogIn = new CNNBSSLogIn;
	}

	if (destroy)
	{
		if (p_CNNBSSLogIn)
		{
			if (p_CNNBSSLogIn->GetChildren().GetCount() > 0)
				p_CNNBSSLogIn->DestroyChildren();
			p_CNNBSSLogIn->Destroy();
			return nullptr;
		}
		else
		{
			// nullptr can't be destroyed/deleted!
			NNBSSErrorShow("System Error: nullptr can't be destroyed! -CNNBSSLogIn-", 100);
			return nullptr;
		}
	}

	return p_CNNBSSLogIn;
}

inline CNNBSSUX* CNNBSSUXAddressHandle(bool destroy = false)
{
	static CNNBSSUX* p_CNNBSSUX;

	if (!p_CNNBSSUX)
	{
		p_CNNBSSUX = new CNNBSSUX;
	}

	if (destroy)
	{
		if (p_CNNBSSUX)
		{
			if (p_CNNBSSUX->GetChildren().GetCount() > 0)
				p_CNNBSSUX->DestroyChildren();
			p_CNNBSSUX->Destroy();

			return nullptr;
		}
		else
		{
			// nullptr can't be destroyed/deleted!
			NNBSSErrorShow("System Error: nullptr can't be destroyed! -CNNBSSUX-", 100);
			return nullptr;
		}
	}

	return p_CNNBSSUX;
}


inline CNNBSSUX* GetActiveUXFrame()
{
	return CNNBSSUXAddressHandle();
}

namespace DeviceList
{
	std::string CNNBSSSearchText(const long id, bool set, std::string text = std::string());
	void CNNBSSDeviceListSearch(wxCommandEvent& evt);

	int GetIDFromType(wxString type);
	wxString GetTypeFromID(int id);
	void CNNBSSDeviceListAddDevice(SNNBSSMVMData* params);
	void CNNBSSDeviceListRemoveSelected();
	void CNNBSSDeviceListRemoveCache(int index);
	void CNNBSSDeviceListConnectionHandle(SNNBSSMVMData* params, const wxString& connectionStatus);
	void CNNBSSDeviceListUpdate();
}

namespace NNBSSNotifications
{
	void AppendNotification(NNBSSNotifications::SNNBSSNotificationAttributes params);
}

// Pages
inline CNNBSSViewPage* CNNBSSViewPageAddressHandle(bool destroy = false)
{
	static CNNBSSViewPage* p_CNNBSSViewPage = nullptr;

	if (destroy)
	{
		if (p_CNNBSSViewPage)
			p_CNNBSSViewPage = nullptr;
		return nullptr;
	}

	if (!p_CNNBSSViewPage)
		p_CNNBSSViewPage = new CNNBSSViewPage();

	return p_CNNBSSViewPage;
}

inline CNNBSSAboutPage* CNNBSSAboutPageAddressHandle(bool destroy = false)
{
	static CNNBSSAboutPage* p_CNNBSSAboutPage = nullptr;

	if (destroy)
	{
		if (p_CNNBSSAboutPage)
			delete p_CNNBSSAboutPage;
		return nullptr;
	}

	if (!p_CNNBSSAboutPage)
		p_CNNBSSAboutPage = new CNNBSSAboutPage();

	return p_CNNBSSAboutPage;
}

//--------------------------------------------------------------------------------------

#endif // ! CLASSMANAGER_H