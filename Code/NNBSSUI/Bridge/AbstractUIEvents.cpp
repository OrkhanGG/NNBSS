///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#include "AbstractUIEvents.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

wxDEFINE_EVENT(EVT_DEVICELIST_ON_ADD_REQUEST, CNNBSSEventsDeviceList);
wxDEFINE_EVENT(EVT_DEVICELIST_ON_ADD, CNNBSSEventsDeviceList);
wxDEFINE_EVENT(EVT_DEVICELIST_ON_REMOVE_REQUEST, CNNBSSEventsDeviceList);
wxDEFINE_EVENT(EVT_DEVICELIST_ON_REMOVE, CNNBSSEventsDeviceList);

wxDEFINE_EVENT(EVT_CONTROLSLIDER_ON_CHANGE, CNNBSSEventsControlSlider);
//----------------------------------------------------------------

CNNBSSMouseEvents::~CNNBSSMouseEvents()
{

}

void CNNBSSMouseEvents::OnAnyButtonClick(wxMouseEvent& event)
{

	switch (event.GetId())
	{
		// Menu Bar
	case NNBSS_EVT_ADD_SERVICES_PAGE:
		CNNBSSControlPanelAddressHandle()->OnAddPage(wxCommandEvent());
		break;
	case NNBSS_EVT_SHOW_OPTIONS:
		break;
	case NNBSS_EVT_SHOW_LAYOUT_OPTIONS:
		CNNBSSViewPageAddressHandle()->_Create();
		CNNBSSViewPageAddressHandle()->ShowModal();
		break;
	case NNBSS_EVT_SHOW_ABOUT_US:
		CNNBSSAboutPageAddressHandle()->_RequestCreate();
		break;
		// Services
	case NNBSS_EVT_SHOW_DEVICES_PANEL:
		CNNBSSControlPanelAddressHandle()->SetPageText(CNNBSSControlPanelAddressHandle()->_GetCurrentPageIndex(),
			_("Devices"));
		CNNBSSContentPanelAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel(), false)
			->_ShowDevicesContent();
		break;
	case NNBSS_EVT_SHOW_GRIDVIEW_PANEL:
		CNNBSSControlPanelAddressHandle()->SetPageText(CNNBSSControlPanelAddressHandle()->_GetCurrentPageIndex(),
			_("GridView"));
		CNNBSSContentPanelAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel(), false)
			->_ShowGridViewContent();
		break;
	case NNBSS_EVT_SHOW_NOTIFICATIONS_PANEL:
		CNNBSSControlPanelAddressHandle()->SetPageText(CNNBSSControlPanelAddressHandle()->_GetCurrentPageIndex(),
			_("Notifications"));
		CNNBSSContentPanelAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel(), false)
			->_ShowNotificationsContent();
		break;
	case NNBSS_EVT_SHOW_PLAYBACK_PANEL:
		CNNBSSControlPanelAddressHandle()->SetPageText(CNNBSSControlPanelAddressHandle()->_GetCurrentPageIndex(),
			_("Playback"));
		CNNBSSContentPanelAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel(), false)
			->_ShowPlaybackContent();
		break;
	case NNBSS_EVT_SHOW_PROFILES_PANEL:
		CNNBSSControlPanelAddressHandle()->SetPageText(CNNBSSControlPanelAddressHandle()->_GetCurrentPageIndex(),
			_("Profiles"));
		CNNBSSContentPanelAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel(), false)
			->_ShowProfilesContent();
		break;
	case NNBSS_EVT_SHOW_DETECTIONCONTENT_PANEL:
		CNNBSSControlPanelAddressHandle()->SetPageText(CNNBSSControlPanelAddressHandle()->_GetCurrentPageIndex(),
			_("Detection"));
		CNNBSSContentPanelAddressHandle(CNNBSSControlPanelAddressHandle()->_GetCurrentContentPanel(), false)
			->_ShowDetectionContent();
		break;
		// Devices Panel
	case NNBSS_EVT_ADD_DEVICE_DIALOG:
		CNNBSSAddDeviceDialogAddressHandle()->_Create(GetActiveUXFrame());
		CNNBSSAddDeviceDialogAddressHandle()->ShowModal();
		break;
	case NNBSS_EVT_REMOVE_DEVICE_DIALOG:
		wxQueueEvent(CNNBSSUXAddressHandle(), new CNNBSSEventsDeviceList(EVT_DEVICELIST_ON_REMOVE_REQUEST, nullptr));
		break;
		// GridView
	case NNBSS_EVT_GRIDVIEW_2x2:
	{
		switch (((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->GetCurrentContent())
		{
		case NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW:
			CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->ChangeGridSize(wxSize(2, 2));
			break;
		case NNBSS_CONTENTPAGE_CURRENTCONTENT_PLAYBACK:
			CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->ChangeGridSize(wxSize(2, 2));
			break;
		default:
			break;
		}
	}
	break;
	case NNBSS_EVT_GRIDVIEW_4x4:
	{
		switch (((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->GetCurrentContent())
		{
		case NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW:
			CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->ChangeGridSize(wxSize(4, 4));
			break;
		case NNBSS_CONTENTPAGE_CURRENTCONTENT_PLAYBACK:
			CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->ChangeGridSize(wxSize(4, 4));
			break;
		default:
			break;
		}
	}
	break;

	case NNBSS_EVT_GRIDVIEW_CUSTOMIZE:
		CNNBSSGridViewEditDialogAddressHandle()->_Create(GetActiveUXFrame());
		CNNBSSGridViewEditDialogAddressHandle()->ShowModal();
		break;

	case NNBSS_EVT_GRIDVIEW_FULLSCREEN:
		switch (((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->GetCurrentContent())
		{
		case NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW:
		{
			CNNBSSBitmapPanel* _selectedPanelGV =
				CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())
				->GetSelectedThumbnail();

			if (!_selectedPanelGV)
			{
				NNBSSInfoShow("There's no active camera panel to fullscreen!", 100);
				return;
			}
			bool _fullscreenGV = CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())
				->_IsFullScreen();
			CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())
				->FullscreenThumbnail(_selectedPanelGV, !_fullscreenGV);
		}
			break;
		case NNBSS_CONTENTPAGE_CURRENTCONTENT_PLAYBACK:
		{
			CNNBSSBitmapPanel* _selectedPanelPB =
				CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())
				->GetSelectedThumbnail();

			if (!_selectedPanelPB)
			{
				NNBSSInfoShow("There's no active camera panel to fullscreen!", 100);
				return;
			}
			bool _fullscreenPB = CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())
				->_IsFullScreen();
			CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())
				->FullscreenThumbnail(_selectedPanelPB, !_fullscreenPB);
		}
			break;
		default:
			break;
		}
		break;
	}
	event.Skip();
}