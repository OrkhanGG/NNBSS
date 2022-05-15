#include "ClassManager.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// Standard C Lib
#include <ctime>
#include <chrono>

#define NNBSS_GET_UTC_TIME_MS_ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


namespace DeviceList
{
	// Set/Get Search texts according to ID. ID will define which search text value You want to set/get.
	// Usually, NNBSS uses IDs that defines pages. That's because at any time that search action is executed 
	// we should update all pages. e.g.: Device List has a const ID and once search action occurs,
	// all opened Device List pages will get that text and apply that search filter
	std::string CNNBSSSearchText(const long id, bool set, std::string text)
	{
		// Search texts contains search texts : key = id/page that searchbar belong to, value = search text
		static std::map<int, std::string> s_SearchTexts;
		//get
		if (!set)
		{
			auto itr = s_SearchTexts.find(id);
			if (itr != s_SearchTexts.end())
			{
				return itr->second;
			}
		}
		// set
		else
		{
			s_SearchTexts[id] = text;
		}

		return std::string();
	}
	// Gets an event from a GUI class (i.e.:CNNBSSContentPanel) and caches global search text data,
	// applies that filter to all regarded pages and update all of those pages.
	void CNNBSSDeviceListSearch(wxCommandEvent& evt)
	{
		/* Set search text value for specific page category so any page that is open or created recently will get that value.
		* CNNBSSSearchText will store all wxSearchCtrl texts of all pages(use IDs for pages) and you can set/get their value.
		* evt.GetInt() may be == NNBSS_CONTENTPAGE_CURRENTCONTENT_DEVICES, NNBSS_CONTENTPAGE_CURRENTCONTENT_NOTIFICATIONS
		 or any other currentcontent page type*/
		CNNBSSSearchText(evt.GetInt(), true, evt.GetString().ToStdString());

		// Update all pages that have Search Bar// Actually, we might make a global update function for pages...
		for (const auto& p : m_NNBSSContentPanelList)
		{
			// This method will update searchbars of its content pages and won't do anything if has no searchbar
			p->UpdateSearchBar();

			if (p->GetCurrentContent() == NNBSS_CONTENTPAGE_CURRENTCONTENT_DEVICES)
			{
				if (CNNBSSDeviceListAddressHandle(p)->IsCreated())
				{
					CNNBSSDeviceListAddressHandle(p)->_RecreateList();
				}
			}
			else if (p->GetCurrentContent() == NNBSS_CONTENTPAGE_CURRENTCONTENT_NOTIFICATIONS)
			{
				if (CNNBSSNotificationListAddressHandle(p)->_IsCreated)
				{
					// TODO: Update notifications list!
					//CNNBSSDeviceListAddressHandle(m_NNBSSContentPanelList[c])->_RecreateList();
				}
			}

		}
	}

	int GetIDFromType(wxString type)
	{
		int m_id;

		if (type == _("USB Camera"))
		{
			m_id = NNBSS_MVM_TYPE_USBCAM;
		}
		else if (type == _("IP Camera"))
		{
			m_id = NNBSS_MVM_TYPE_IPCAM;
		}
		else if (type == _("Microphone"))
		{
			m_id = NNBSS_MVM_TYPE_MICROPHONE;
		}
		else if (type == _("DVR"))
		{
			m_id = NNBSS_MVM_TYPE_DVR;
		}
		else if (type == _("NVR"))
		{
			m_id = NNBSS_MVM_TYPE_NVR;
		}
		else if (type == _("Video"))
		{
			m_id = NNBSS_MVM_TYPE_VIDEO;
		}
		else
		{
			m_id = -100;
			NNBSSErrorShow("Given DeviceType was wrong while getting int from wxString!", 100);
		}

		return m_id;
	}

	wxString GetTypeFromID(int id)
	{
		// Has to be given by using GetIDFromEnum

		wxString m_type;

		switch (id)
		{
		case NNBSS_MVM_TYPE_USBCAM:
			m_type = _("USB Camera");
			break;
		case NNBSS_MVM_TYPE_IPCAM:
			m_type = _("IP Camera");
			break;
		case NNBSS_MVM_TYPE_MICROPHONE:
			m_type = _("Microphone");
			break;
		case NNBSS_MVM_TYPE_DVR:
			m_type = _("DVR");
			break;
		case NNBSS_MVM_TYPE_NVR:
			m_type = _("NVR");
			break;
		case NNBSS_MVM_TYPE_VIDEO:
			m_type = _("Video");
		default:
			NNBSSErrorShow("Given DeviceType was wrong while getting wxString from int!", 100);
			break;
		}

		return m_type;
	}


	// This function may commonly be called from AddDeviceDialog. 
	// Arguments will be filled by the information of the mentioned dialog.
	// I accept that declaration of this function's being here is not the best way, but it does the stuff for now.
	void CNNBSSDeviceListAddDevice(SNNBSSMVMData* params)
	{
		// If device or address already exist, we won't let it to come this far.
		// Check add device dialog.

		params->p_GlobalEventSink = CNNBSSUXAddressHandle();
		params->m_MVMThread = new CNNBSSMovingVisualMediaThread(params);
		if (params->m_MVMThread->Run() != wxTHREAD_NO_ERROR)
		{
			NNBSSMessageBoxSimple(_("Error"), _("Could not start MVM Thread!"), wxICON_ERROR);
			return;
		}
		SCNNBSSMVMParametersList.emplace_back(params);

		// Update all pages that have Device List active
		for (const auto&  p : m_NNBSSContentPanelList)
		{
			if (CNNBSSDeviceListAddressHandle(p)->IsCreated())
			{
				CNNBSSDeviceListAddressHandle(p)->_RecreateList();

				// Select last one
				CNNBSSDeviceListAddressHandle(p)->		
					_SelectSingle(std::clamp(CNNBSSDeviceListAddressHandle(p)->GetItemCount() - 1,0,NNBSSMAXELEMENT));
			}
		}

		wxQueueEvent(CNNBSSUXAddressHandle(), new CNNBSSEventsDeviceList(EVT_DEVICELIST_ON_ADD, params));
	}

	// Once, user requests will be recieved by system by this function.
	// It should also be mentioned that This function does not removes anything in fact.
	// It just sends request to UI(in this case DeviceList) to remove the selected Device according to its index
	// If UI approves the offer, it'll call CNNBSSDeviceListRemoveCache function within
	void CNNBSSDeviceListRemoveSelected()
	{
		// Removing only from THE CURRENT PAGE should work. Because we'll update it afterwards
		if (CNNBSSDeviceListAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel())->IsCreated())
		{
			CNNBSSDeviceListAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel())
				->_RemoveSelectedFromList();
		}

		// Update all pages that have Device List active
		for (const auto & p : m_NNBSSContentPanelList) {
			if (CNNBSSDeviceListAddressHandle(p)->IsCreated())
			{
				CNNBSSDeviceListAddressHandle(p)->_RecreateList();
			}
		}
	}

	// If User requested to remove an element and DeviceList approved that it may be a go,
	// Proceed the execution. int index argument is given by DeviceList(which's based on the selected index in the list)
	void CNNBSSDeviceListRemoveCache(int index)
	{
		if (index < 0)
		{
			index = 0;
		}

		if (SCNNBSSMVMParametersList.size() < index)
		{
			index = (int)SCNNBSSMVMParametersList.size();
		}

		for (int c = 0; c < (int)SCNNBSSMVMParametersList.size(); c++)
		{
			if (c != index)
				continue;

			if (SCNNBSSMVMParametersList[c] == nullptr) { NNBSSMessageBoxSimple(_("Error"), _("Device could not be removed! Error Code: DEVLIST:NULLPTR!")); return; }

			wxQueueEvent(CNNBSSUXAddressHandle(), new CNNBSSEventsDeviceList(EVT_DEVICELIST_ON_REMOVE, SCNNBSSMVMParametersList[c]));
			
			break;
		}
	}

	void CNNBSSDeviceListConnectionHandle(SNNBSSMVMData* params, const wxString& connectionStatus)
	{
		wxString statusMessage;

		if (connectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE)
		{
			for (auto& _params : SCNNBSSMVMParametersList)
			{
				if (_params == params)
				{
					_params->m_MVMConnectionStatus = NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE;
					statusMessage = wxString::Format("%s %s", params->m_MVMName, _("device is now online!"));
					break;
				}
			}
		}
		else if (connectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_LOW_CONNECTION)
		{
			for (auto& _params : SCNNBSSMVMParametersList)
			{
				if (_params == params)
				{
					// Not decided yet
					_params->m_MVMConnectionStatus = NNBSS_DEVICE_CONNECTION_STATUS_STRING_LOW_CONNECTION;

					statusMessage = wxString::Format("%s %s", params->m_MVMName, _("device has slow connection!"));

					break;
				}
			}
		}
		else if (connectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_OFFLINE)
		{
			for (auto& _params : SCNNBSSMVMParametersList)
			{
				if (_params == params)
				{
					// Not decided yet
					_params->m_MVMConnectionStatus = NNBSS_DEVICE_CONNECTION_STATUS_STRING_OFFLINE;

					statusMessage = wxString::Format("%s %s", params->m_MVMName, _("device is now offline!"));

					break;
				}
			}
		}
		else if (connectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_DISCONNECTED)
		{
			for (auto& _params : SCNNBSSMVMParametersList)
			{
				if (_params == params)
				{
					_params->m_MVMSourceAddress = wxT("-1");
					_params->m_MVMConnectionStatus = NNBSS_DEVICE_CONNECTION_STATUS_STRING_DISCONNECTED;

					statusMessage = wxString::Format("%s %s", params->m_MVMName, _("device is disconnected!"));

					break;
				}
			}
		}
		// Update all pages that have Device List active
		for (const auto& p : m_NNBSSContentPanelList)
		{
			if (CNNBSSDeviceListAddressHandle(p)->IsCreated())
			{
				CNNBSSDeviceListAddressHandle(p)->_RecreateList();
			}
		}
	}


	void CNNBSSDeviceListUpdate()
	{
		// Update all pages that have Device List active
		for (const auto& p : m_NNBSSContentPanelList)
		{
			if (CNNBSSDeviceListAddressHandle(p)->IsCreated())
			{
				CNNBSSDeviceListAddressHandle(p)->_RecreateList();
			}
		}
	}
}

namespace NNBSSNotifications
{
	void AppendNotification(NNBSSNotifications::SNNBSSNotificationAttributes params)
	{
		// The idea is, to gather all notifications and then once the gate is open, display those queueing notifications

		static std::vector<NNBSSNotifications::SNNBSSNotificationAttributes> queuingNotifications;
		{
			std::time_t t = std::time(0);   // get time now
			std::tm* now = std::localtime(&t);
			params.DateAndTime =
				std::to_string(now->tm_mday) + "/" + std::to_string(now->tm_mon) + "/" + std::to_string(now->tm_year + 1900) +
				" " + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec);
		}
		queuingNotifications.emplace_back(params);

		static auto m_LastPassMoment = NNBSS_GET_UTC_TIME_MS_;
		const int m_OpenGatePeriod = 10;// With seconds
		if (NNBSS_GET_UTC_TIME_MS_ - m_LastPassMoment < m_OpenGatePeriod * 1000)
		{
			return;
		}
		else
		{
			for (const auto& p : queuingNotifications)
			{
				NNBSSNotifications::SNNBSSNotificationAttributesList.emplace_back(p);
			}

			queuingNotifications.clear();

			m_LastPassMoment = NNBSS_GET_UTC_TIME_MS_;
		}

		// Update all pages that have NotificationList active
		for (const auto& p : m_NNBSSContentPanelList)
		{
			if (p->GetCurrentContent() == NNBSS_CONTENTPAGE_CURRENTCONTENT_DEVICES)
				if (CNNBSSNotificationListAddressHandle(p)->_IsCreated)
				{
					CNNBSSNotificationListAddressHandle(p)->_RecreateList();
				}
		}
	}
}