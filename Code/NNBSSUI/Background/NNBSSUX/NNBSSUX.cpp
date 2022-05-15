///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "NNBSSUX.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

void CNNBSSUX::_Create(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
{
	Create(parent, id, title, pos, size, style);

	// Title Bar
	CNNBSS_SC_TitleBarAddressHandle()->_Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxNO_BORDER);

	p_ManagedWindow = new wxPanel(this, wxID_ANY);
	
	wxSizer* p_BaseSizer = new wxBoxSizer(wxVERTICAL);
	p_BaseSizer->Add(CNNBSS_SC_TitleBarAddressHandle(),wxSizerFlags().Expand());
	p_BaseSizer->AddSpacer(FromDIP(2));
	p_BaseSizer->Add(GetUXManagedWindow(), wxSizerFlags().Proportion(1).Expand());
	SetSizer(p_BaseSizer);
	SetBackgroundColour(RGB_COLOURDEF_PLACEHOLDER);
	//--------------------------------------------------------------------------------------------

	// Bind
	Bind(EVT_DEVICELIST_ON_ADD_REQUEST, &CNNBSSUX::OnDeviceListEvent, this);
	Bind(EVT_DEVICELIST_ON_ADD, &CNNBSSUX::OnDeviceListEvent, this);
	Bind(EVT_DEVICELIST_ON_REMOVE_REQUEST, &CNNBSSUX::OnDeviceListEvent, this);
	Bind(EVT_DEVICELIST_ON_REMOVE, &CNNBSSUX::OnDeviceListEvent, this);

	// MVM Events
		// Device
	Bind(EVT_MVM_DEVICE_CONNECT_ATTEMPT, &CNNBSSUX::OnMVMEvent, this);
	Bind(EVT_MVM_DEVICE_CONNECT_FAIL, &CNNBSSUX::OnMVMEvent, this);
	Bind(EVT_MVM_DEVICE_CONNECTED, &CNNBSSUX::OnMVMEvent, this);
	Bind(EVT_MVM_DEVICE_HIGH_CONNECTION, &CNNBSSUX::OnMVMEvent, this);
	Bind(EVT_MVM_DEVICE_LOW_CONNECTION, &CNNBSSUX::OnMVMEvent, this);
	Bind(EVT_MVM_DEVICE_DISCONNECTED, &CNNBSSUX::OnMVMEvent, this);
	// Thumbnail
	Bind(EVT_MVM_CAPTURE_STARTED, &CNNBSSUX::OnMVMEvent, this);
	Bind(EVT_MVM_COMMAND_RESULT, &CNNBSSUX::OnMVMEvent, this);
	Bind(EVT_MVM_ERROR_OPEN, &CNNBSSUX::OnMVMEvent, this);
	Bind(EVT_MVM_ERROR_EMPTY, &CNNBSSUX::OnMVMEvent, this);
	Bind(EVT_MVM_ERROR_EXCEPTION, &CNNBSSUX::OnMVMEvent, this);
	// Others
	Bind(wxEVT_CLOSE_WINDOW, &CNNBSSUX::OnFrameClose, this);
}

void CNNBSSUX::OnDeviceListEvent(CNNBSSEventsDeviceList& evt)
{
	wxEventType et = evt.GetEventType();
	if (et == EVT_DEVICELIST_ON_ADD_REQUEST)
	{
		DeviceList::CNNBSSDeviceListAddDevice(evt.GetMVMParams());
	}
	else if (et == EVT_DEVICELIST_ON_ADD)
	{
		// nothing for now
	}
	else if (et == EVT_DEVICELIST_ON_REMOVE_REQUEST)
	{
		DeviceList::CNNBSSDeviceListRemoveSelected();
	}
	else if (et == EVT_DEVICELIST_ON_REMOVE)
	{
		// Send events to respective places
		for (const auto& p : m_NNBSSContentPanelList)
		{
			// For Grid View
			if (p->GetCurrentContent() == NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW)
			{
				CNNBSSCameraGridAddressHandle(p)->OnDeviceListEvent(evt);
			}
		}

		// Destroy that element when we're done
		auto itr = std::find(SCNNBSSMVMParametersList.begin(), SCNNBSSMVMParametersList.end(), evt.GetMVMParams());
		auto _itr = (*itr);
		_itr->m_MVMThread->Delete();
		delete _itr->m_MVMThread;
		delete _itr;
		SCNNBSSMVMParametersList.erase(itr);
		// Update all device lists that are open
		DeviceList::CNNBSSDeviceListUpdate();
	}
	evt.Skip();
}

void CNNBSSUX::OnMVMEvent(MovingVisualMediaEvent& evt)
{
	wxEventType et = evt.GetEventType();
	if (et == EVT_MVM_DEVICE_CONNECT_ATTEMPT)
	{
		NNBSSInfoShow("Attempting to add device named '%s'", 100, evt.GetMVMParams()->m_MVMName.ToStdString().c_str());
	}
	else if (et == EVT_MVM_DEVICE_CONNECT_FAIL)
	{
		DeviceList::CNNBSSDeviceListConnectionHandle(evt.GetMVMParams(), NNBSS_DEVICE_CONNECTION_STATUS_STRING_DISCONNECTED);

		NNBSSInfoShow("Connection to device named '%s' failed", 100, evt.GetMVMParams()->m_MVMName.ToStdString().c_str());
	}
	else if (et == EVT_MVM_DEVICE_CONNECTED)
	{
		DeviceList::CNNBSSDeviceListConnectionHandle(evt.GetMVMParams(), NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE);

		NNBSSInfoShow("Connected to device named '%s'", 100, evt.GetMVMParams()->m_MVMName.ToStdString().c_str());
	}
	else if (et == EVT_MVM_DEVICE_HIGH_CONNECTION)
	{
		DeviceList::CNNBSSDeviceListConnectionHandle(evt.GetMVMParams(), NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE);
	}
	else if (et == EVT_MVM_DEVICE_LOW_CONNECTION)
	{
		DeviceList::CNNBSSDeviceListConnectionHandle(evt.GetMVMParams(), NNBSS_DEVICE_CONNECTION_STATUS_STRING_LOW_CONNECTION);
	}
	else if (et == EVT_MVM_DEVICE_DISCONNECTED)
	{
		DeviceList::CNNBSSDeviceListConnectionHandle(evt.GetMVMParams(), NNBSS_DEVICE_CONNECTION_STATUS_STRING_DISCONNECTED);

		NNBSSErrorShow("Disconnected from device named '%s'", 100, evt.GetMVMParams()->m_MVMName.ToStdString().c_str());
	}
	//-
	else if (et == EVT_MVM_CAPTURE_STARTED)
	{
	}
	else if (et == EVT_MVM_COMMAND_RESULT)
	{
	}
	else if (et == EVT_MVM_ERROR_OPEN)
	{
	}
	else if (et == EVT_MVM_ERROR_EMPTY)
	{
	}
	else if (et == EVT_MVM_ERROR_EXCEPTION)
	{
	}
	evt.Skip();
}

void CNNBSSUX::OnFrameClose(wxCloseEvent& evt)
{
	const int selection = NNBSSMessageBoxSimple(_("Warning"), _("Are you sure you want to close NNBSS?"), wxYES_NO, this);
	if (selection != wxID_OK)
	{
		if (evt.CanVeto())
			evt.Veto();
		return;
	}

	ExitAll();

	evt.Skip();
}

void CNNBSSUX::ExitAll()
{
	// Delete all pages
	CNNBSSControlPanelAddressHandle()->DeleteAllPages();

	// NNBSS Hardware
	CNNBSSHardwareAddressHandle()->~CNNBSSHardware();// This class actually will stop a thread that interact with UI directly.
}

CNNBSSUX::~CNNBSSUX()
{

}

WXLRESULT CNNBSSUX::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
	/* When we have a custom titlebar in the window, we don't need the non-client area of a normal window
	  * to be painted. In order to achieve this, we handle the "WM_NCCALCSIZE" which is responsible for the
	  * size of non-client area of a window and set the return value to 0. Also we have to tell the
	  * application to not paint this area on activate and deactivation events so we also handle
	  * "WM_NCACTIVATE" message. */
	switch (nMsg)
	{
	case WM_NCACTIVATE:
	{
		/* Returning 0 from this message disable the window from receiving activate events which is not
		desirable. However When a visual style is not active (?) for this window, "lParam" is a handle to an
		optional update region for the nonclient area of the window. If this parameter is set to -1,
		DefWindowProc does not repaint the nonclient area to reflect the state change. */
		lParam = -1;
		break;
	}
	/* To remove the standard window frame, you must handle the WM_NCCALCSIZE message, specifically when
	its wParam value is TRUE and the return value is 0 */
	case WM_NCCALCSIZE:
		if (wParam)
		{
			/* Detect whether window is maximized or not. We don't need to change the resize border when win is
			*  maximized because all resize borders are gone automatically */
			HWND hWnd = (HWND)this->GetHandle();
			WINDOWPLACEMENT wPos;
			// GetWindowPlacement fail if this member is not set correctly.
			wPos.length = sizeof(wPos);
			GetWindowPlacement(hWnd, &wPos);
			if (wPos.showCmd != SW_SHOWMAXIMIZED)
			{
				RECT borderThickness;
				SetRectEmpty(&borderThickness);
				AdjustWindowRectEx(&borderThickness,
					GetWindowLongPtr(hWnd, GWL_STYLE) & ~WS_CAPTION, FALSE, NULL);
				borderThickness.left *= -1;
				borderThickness.top *= -1;
				NCCALCSIZE_PARAMS* sz = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				// Add 1 pixel to the top border to make the window resizable from the top border
				sz->rgrc[0].top += 1;
				sz->rgrc[0].left += borderThickness.left;
				sz->rgrc[0].right -= borderThickness.right;
				sz->rgrc[0].bottom -= borderThickness.bottom;
				return 0;
			}
		}
		break;
	}

	return wxFrame::MSWWindowProc(nMsg, wParam, lParam);
}