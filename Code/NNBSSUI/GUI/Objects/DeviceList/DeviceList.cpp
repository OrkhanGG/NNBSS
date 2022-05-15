///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "DeviceList.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/control.h"
#include "wx/settings.h"
#include "wx/dc.h"
#include <wx/dcbuffer.h>
#include "wx/dcclient.h"
#endif // WX_PRECOMP
#include <wx/richmsgdlg.h>

#include "DeviceList.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// Boost
#include <boost/algorithm/string.hpp>

// ID defintions
	// ContextMenu(Popup)
#define OBJECT_DETECTION_PROPERTIES 25000
#define FACE_RECOGNITION_PROPERTIES 25001
#define MOTION_DETECTION_PROPERTIES 25002
#define NOTIFICATION_PROPERTIES 25003
#define _REMOVE_DEVICE_ 25004
#define _DEVICE_PROPERTIES_ 25005

//-------------------------------------------------------------------------------------
// CNNBSSDeviceList

CNNBSSDeviceList::~CNNBSSDeviceList()
{
}

void CNNBSSDeviceList::_Create(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
	if (_IsCreated)
	{
		if (this)
		{
			if (GetChildren().GetCount() > 0)
			{
				DestroyChildren();
			}
			Destroy();
		}
	}
	if (!_IsCreated)
	{
		Create(parent, GetId(), pos, size, style);

		_SetupInterface();

		_IsCreated = true;
	}
}

void CNNBSSDeviceList::_RecreateList()
{
	// to speed up inserting we hide the control temporarily
	DeleteAllItems();

	int RowNumber = 0;

	for (const auto& p : SCNNBSSMVMParametersList)
	{
		if (p->m_MVMType == NNBSS_MVM_TYPE_VIDEO)
			continue;

		// container
		wxString buffer;

		// Row number
		RowNumber++;
		buffer.Printf("%i", RowNumber);

		int image = -1;
		switch (p->m_MVMType)
		{
		case NNBSS_MVM_TYPE_USBCAM:
			image = 0;
			break;
		case NNBSS_MVM_TYPE_IPCAM:
			image = 1;
			break;
		case NNBSS_MVM_TYPE_DVR:
			image = 2;
			break;
		case NNBSS_MVM_TYPE_NVR:
			image = 3;
			break;
		case NNBSS_MVM_TYPE_MICROPHONE:
			image = 4;
			break;
		default:
			image = -1;
			break;
		}

		// Set tmp as index and insert item
		long tmp = InsertItem(RowNumber, buffer, image);
		SetItemData(tmp, RowNumber);

		// Device Name
		buffer.Printf("%s", p->m_MVMName);
		SetItem(tmp, 1, buffer);

		// Product Name
		if (p->m_DeviceNetName.empty())
			buffer.Printf("N/A");
		else
			buffer.Printf("%s", p->m_DeviceNetName);
		SetItem(tmp, 2, buffer);

		// Product path
		if (p->m_DevicePath.empty())
			buffer.Printf("N/A");
		else
			buffer.Printf("%s", p->m_DevicePath);
		SetItem(tmp, 3, buffer);

		// Address
		buffer.Printf("%s", p->m_MVMSourceAddress);
		SetItem(tmp, 4, buffer);

		// Device Type
		buffer.Printf("%s", DeviceList::GetTypeFromID(p->m_MVMType));
		SetItem(tmp, 5, buffer);

		// Status
		buffer.Printf("%s", p->m_MVMConnectionStatus);
		SetItem(tmp, 6, buffer);

		// Change item colour depending on device status
		if (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_OFFLINE)
		{
			SetItemTextColour(tmp, RGB_COLOURDEF_DAVYSGRAY);
		}
		else if (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE)
		{
			SetItemTextColour(tmp, RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
		}
		else if (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_DISCONNECTED)
		{
			SetItemTextColour(tmp, RGB_COLOURDEF_RED);
		}
		else if (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_LOW_CONNECTION)
		{
			SetItemTextColour(tmp, RGB_COLOURDEF_YELLOWNCS);
		}
		else
		{
			SetItemTextColour(tmp, RGB_COLOURDEF_RED);
		}

		// Consider search bar
		std::string searchFilter = DeviceList::CNNBSSSearchText(NNBSS_CONTENTPAGE_CURRENTCONTENT_DEVICES, false/*false means get text*/);
		if (!searchFilter.empty())
		{
			// create a temp buffer for all strings that search operation will be done on
			bool contains = boost::algorithm::contains(p->m_MVMName + p->m_MVMSourceAddress, searchFilter);
			if (contains)
				SetItemBackgroundColour(tmp, RGB_COLOURDEF_PLACEHOLDER);
		}
	}

	// Add header whether we have element(s) or not.
	_AddHeader();


	// Update Device Count label on parent panel
	for (const auto& p : m_NNBSSContentPanelList)
	{
		// Update all contents that have Device List active
		// By checking existing of DeviceList, we understand that, that content have DeviceList and label to be updated
		if (CNNBSSDeviceListAddressHandle(p)->_IsCreated)
		{
			p->DevicePanelUpdateDeviceCount();
		}
	}

	// Update List Tree
	CNNBSSDeviceListTreeCtrlAddressHandle()->_UpdateList();
}

void CNNBSSDeviceList::_RemoveSelectedFromList()
{
	if (_GetItemCount() <= 1)
	{
		NNBSSInfoShow("There's no device to remove", 100);
		return;
	}

	if (_GetSelectedItemIndex() == -1) {
		NNBSSInfoShow("Select a device to remove.", 100);
		return;
	}

	// Show dialog before removing
	static bool _ShowRemoveDeviceWarning = true;
	if (_ShowRemoveDeviceWarning)
	{
		// Todo: move to related places/class maybe? :)
		wxRichMessageDialog dlg(GetActiveUXFrame(), _("Are you sure to remove this device?"), _("Warning"), wxYES_NO);
		dlg.ShowCheckBox(_("Don't show this dialog again"));
		if (dlg.ShowModal() == wxID_YES)
		{
			if (dlg.IsCheckBoxChecked())
				_ShowRemoveDeviceWarning = false;
		}
		else
		{
			return;
		}
	}

	if (_GetSelectedItemIndex() >= 0)
	{
		int indexToRemove = (_GetSelectedItemIndex()) < 0 ? 0 : (_GetSelectedItemIndex());
		DeviceList::CNNBSSDeviceListRemoveCache(indexToRemove);

		_RecreateList();

		// Select last one
		_SelectSingle(std::clamp(GetItemCount() - 1,0,NNBSSMAXELEMENT));
	}
}

void CNNBSSDeviceList::_SelectSingle(const long& itemIndex)
{
	if (GetItemCount() > 0)
	{
		SetFocus();
		Select(itemIndex);
		_selectedItem = itemIndex;
	}
}

int CNNBSSDeviceList::_GetSelectedItemIndex()
{
	return _selectedItem;
}

int CNNBSSDeviceList::_GetItemCount()
{
	return GetItemCount();
}

void CNNBSSDeviceList::_OnContextMenuPopUp(wxListEvent& event)
{
	{
		// sanity check
		if (_GetItemCount() <= 1)
		{
			return;
		}

		if (_GetSelectedItemIndex() == -1) {
			return;
		}
	}

	bool _ShowForValid = true;

	int selectedIndex = (_GetSelectedItemIndex()) < 0 ? 0 : (_GetSelectedItemIndex());
	SNNBSSMVMData* _CurrentDeviceParameter = SCNNBSSMVMParametersList[selectedIndex];

	if (_CurrentDeviceParameter->m_MVMConnectionStatus != NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE
		&& _CurrentDeviceParameter->m_MVMConnectionStatus != NNBSS_DEVICE_CONNECTION_STATUS_STRING_LOW_CONNECTION)
	{
		_ShowForValid = false;
	}

	wxMenu* DeviceContextMenu = new wxMenu();
	if (_ShowForValid) {
		wxMenuItem* ObjectDetectionMenuItem = new wxMenuItem(DeviceContextMenu, OBJECT_DETECTION_PROPERTIES, _("Object Detection"));
		wxMenuItem* FaceRecognitionMenuItem = new wxMenuItem(DeviceContextMenu, FACE_RECOGNITION_PROPERTIES, _("Face Recognition"));
		wxMenuItem* MotionDetectionMenuItem = new wxMenuItem(DeviceContextMenu, MOTION_DETECTION_PROPERTIES, _("Motion Detection"));
		wxMenuItem* NotificationsMenuItem = new wxMenuItem(DeviceContextMenu, NOTIFICATION_PROPERTIES, _("Notifications"));
		wxMenuItem* PropertiesMenuItem = new wxMenuItem(DeviceContextMenu, _DEVICE_PROPERTIES_, _("Properties"));
		DeviceContextMenu->Append(ObjectDetectionMenuItem);
		DeviceContextMenu->Append(FaceRecognitionMenuItem);
		DeviceContextMenu->Append(MotionDetectionMenuItem);
		DeviceContextMenu->Append(NotificationsMenuItem);
		DeviceContextMenu->Append(PropertiesMenuItem);
	}

	wxMenuItem* RemoveMenuItem = new wxMenuItem(DeviceContextMenu, _REMOVE_DEVICE_, _("Remove"));
	DeviceContextMenu->Append(RemoveMenuItem);

	DeviceContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &CNNBSSDeviceList::_OnContextMenuItemClick, this);
	PopupMenu(DeviceContextMenu);

	event.Skip();
}

void CNNBSSDeviceList::_OnContextMenuItemClick(wxCommandEvent& event)
{
	{
		// sanity check
		if (_GetItemCount() <= 1)
		{
			NNBSSInfoShow("There's no device to select", 100);
			event.Skip();
			return;
		}

		if (_GetSelectedItemIndex() == -1) {
			NNBSSInfoShow("Select a valid device.", 100);
			event.Skip();
			return;
		}
	}

	// ----------------------------------------------------------------------------------
	// Get MVMParameter by selected item's Device Name
	// Be careful! Changing anything related to this snippet may cause bugs that you may not percieve/find easily.
	int selectedIndex = (_GetSelectedItemIndex()) < 0 ? 0 : (_GetSelectedItemIndex());
	wxString selectedItemName = GetItemText(selectedIndex+1,1);// 1 is the second column and stands for the DeviceName name
	wxLogMessage(selectedItemName);
	SNNBSSMVMData* _CurrentDeviceParameter = nullptr;
	for (const auto& p : SCNNBSSMVMParametersList)
		if (selectedItemName == p->m_MVMName)
			_CurrentDeviceParameter = p;
	if (!_CurrentDeviceParameter)
	{
		event.Skip();
		return;
	}
	//-----------------------------------------------------------------------------------

	switch (event.GetId())
	{
	case OBJECT_DETECTION_PROPERTIES:
		CNNBSSDevicePropertiesDialogAddressHandle(_CurrentDeviceParameter, GetActiveUXFrame());
		CNNBSSDevicePropertiesDialogAddressHandle(_CurrentDeviceParameter, GetActiveUXFrame())->ShowModal();
		break;
	case FACE_RECOGNITION_PROPERTIES:
		break;
	case MOTION_DETECTION_PROPERTIES:
		break;
	case NOTIFICATION_PROPERTIES:
		break;
	case _REMOVE_DEVICE_:
		_RemoveSelectedFromList();
		break;
	case _DEVICE_PROPERTIES_:
		break;
	default:
		break;
	}

	event.Skip();
}

void CNNBSSDeviceList::_SetupInterface()
{
	Hide();
	// freeze this so we may commit all changes accurately after thaw.
	Freeze();

	// Register icons
	p_ElementIcons = new wxImageList(32, 32, false, 1);
	p_ElementIcons->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_USB_IDLE));
	p_ElementIcons->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_IPCAM_IDLE));
	p_ElementIcons->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_DVR_IDLE));
	p_ElementIcons->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_NVR_IDLE));
	p_ElementIcons->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_MICROPHONE_IDLE));
	SetImageList(p_ElementIcons, wxIMAGE_LIST_SMALL);

	// Add columns
	_AddColumns();

	// This will add the items on start(if we have) and the header.
	_UpdateInterface();

	// Accepted as default theme
	_SetupColours(RGB_COLOURDEF_TRIPLE20, RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);

	// After freeze, we need to thaw.
	Thaw();

	// Setup event(s)
	Bind(wxEVT_LIST_ITEM_SELECTED, &CNNBSSDeviceList::_OnItemSelect, this);
	Bind(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, &CNNBSSDeviceList::_OnContextMenuPopUp, this);
	Bind(wxEVT_SIZE, &CNNBSSDeviceList::_OnSizeChange, this);
	Bind(wxEVT_ERASE_BACKGROUND, &CNNBSSDeviceList::OnErase, this);
	Bind(wxEVT_KEY_UP, &CNNBSSDeviceList::OnKeyRelease, this);

	SetDoubleBuffered(true);

	Show();
}

void CNNBSSDeviceList::_UpdateInterface()
{
	_RecreateList();
}

void CNNBSSDeviceList::_SetupColours(wxColour backgroundColour, wxColour foregroundColour)
{
	SetBackgroundColour(backgroundColour);
	SetForegroundColour(foregroundColour);// Note, Foreground colour also changes the rulers' colour.
												// So, to have different item colour, you've to change it manually.
}

void CNNBSSDeviceList::_AddHeader()
{
	int itemIndex = 0;
	InsertItem(itemIndex, _("Header"), -1);

	// use this function to add text
	// long SetItem(long index, int col, const wxString& label, int imageId = -1)

	SetItem(itemIndex, 0, _("Row Number"), -1);
	SetItem(itemIndex, 1, _("Device Name"));
	SetItem(itemIndex, 2, _("Product Name"));
	SetItem(itemIndex, 3, _("Path"));
	SetItem(itemIndex, 4, _("Address/Port"));
	SetItem(itemIndex, 5, _("Device Type"));
	SetItem(itemIndex, 6, _("Status"));


}

void CNNBSSDeviceList::_AddColumns()
{
	InsertColumn(0, _("Row Number"), wxLIST_FORMAT_CENTER);
	InsertColumn(1, _("Device Name"), wxLIST_FORMAT_CENTER);
	InsertColumn(2, _("Product Name"), wxLIST_FORMAT_CENTER);
	InsertColumn(3, _("Path"), wxLIST_FORMAT_CENTER);
	InsertColumn(4, _("Address/Port"), wxLIST_FORMAT_CENTER);
	InsertColumn(5, _("Device Type"), wxLIST_FORMAT_CENTER);
	InsertColumn(6, _("Status"), wxLIST_FORMAT_CENTER);
}

void CNNBSSDeviceList::OnKeyRelease(wxKeyEvent& event)
{
	if (!HasFocus() || !IsEnabled() || !IsShown() || !IsShownOnScreen())
	{
		event.Skip();
		return;
	}

	if (event.GetKeyCode() == 395)
	{
		// simulate right click
		wxCommandEvent simulateClick(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, m_windowId);
		simulateClick.SetEventObject(this);

		GetEventHandler()->ProcessEvent(simulateClick);
	}

	event.Skip();
}

void CNNBSSDeviceList::_OnItemSelect(wxListEvent& event)
{
	if (event.GetIndex() == 0)
	{
		Select(0, false);
		event.Veto();
	}

	_selectedItem = event.GetIndex() - 1;// because of header

	event.Skip();
}

void CNNBSSDeviceList::_OnSizeChange(wxSizeEvent& event)
{
	if (this == nullptr)
		return;

	Freeze();

	int firstColumnWidth = 150;
	int ExceptFirstColumn = GetColumnCount() - 1;
	int betterFit = 0;//to prevent it from overflowing.

	int FitSize =
		((GetSize().x - firstColumnWidth) / ExceptFirstColumn) - betterFit;

	SetColumnWidth(0, firstColumnWidth);
	for (int c = 1; c < GetColumnCount(); c++) {
		SetColumnWidth(c, FitSize);
	}

	Thaw();
}

//-------------------------------------------------------------------------------------
// CNNBSSDeviceListTreeCtrl

CNNBSSDeviceListTreeCtrl::CNNBSSDeviceListTreeCtrl()
{
}

CNNBSSDeviceListTreeCtrl::~CNNBSSDeviceListTreeCtrl()
{
}

void CNNBSSDeviceListTreeCtrl::_Create(wxWindow* parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
{
	Create(parent, winid, pos, size, style, validator, name);

	_SetupInterface();

	_BindEvents(true);
}

void CNNBSSDeviceListTreeCtrl::_SetupInterface()
{
	wxImageList* p_Images = new wxImageList(32, 32, false, 1);
	p_Images->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_USB_IDLE));
	p_Images->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_IPCAM_IDLE));
	p_Images->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_DVR_IDLE));
	p_Images->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_NVR_IDLE));
	p_Images->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_MICROPHONE_IDLE));
	p_Images->Add(NNBSS_PNG2BMP(NNBSSART_DEVICELIST_TREECTRL_ROOT_DEVICE_IDLE));
	SetImageList(p_Images);

	SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	SetForegroundColour(RGB_COLOURDEF_WHITE);

	SetMinSize(GetBestSize() + wxSize(20, 0));
}

void CNNBSSDeviceListTreeCtrl::_BindEvents(bool bind)
{
	if (bind)
	{
		Bind(wxEVT_TREE_BEGIN_DRAG, &CNNBSSDeviceListTreeCtrl::OnBeginDrag, this);
		Bind(wxEVT_TREE_END_DRAG, &CNNBSSDeviceListTreeCtrl::OnEndDrag, this);

		Bind(wxEVT_TREE_ITEM_MENU, &CNNBSSDeviceListTreeCtrl::OnContextMenu, this);
	}
	else
	{
		Unbind(wxEVT_TREE_BEGIN_DRAG, &CNNBSSDeviceListTreeCtrl::OnBeginDrag, this);
		Unbind(wxEVT_TREE_END_DRAG, &CNNBSSDeviceListTreeCtrl::OnEndDrag, this);
	}
}
void CNNBSSDeviceListTreeCtrl::OnContextMenu(wxTreeEvent& evt)
{
	{
		// sanity check
		if (GetCount() <= 1)
		{
			return;
		}
	}

	wxMenu* DeviceContextMenu = new wxMenu(wxNO_BORDER | wxBORDER_NONE);

	wxMenuItem* ObjectDetectionMenuItem = new wxMenuItem(DeviceContextMenu, OBJECT_DETECTION_PROPERTIES, _("Object Detection"));
	ObjectDetectionMenuItem->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	ObjectDetectionMenuItem->SetTextColour(RGB_COLOURDEF_WHITE);
	wxMenuItem* FaceRecognitionMenuItem = new wxMenuItem(DeviceContextMenu, FACE_RECOGNITION_PROPERTIES, _("Face Recognition"));
	FaceRecognitionMenuItem->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	FaceRecognitionMenuItem->SetTextColour(RGB_COLOURDEF_WHITE);
	wxMenuItem* MotionDetectionMenuItem = new wxMenuItem(DeviceContextMenu, MOTION_DETECTION_PROPERTIES, _("Motion Detection"));
	MotionDetectionMenuItem->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	MotionDetectionMenuItem->SetTextColour(RGB_COLOURDEF_WHITE);
	wxMenuItem* NotificationsMenuItem = new wxMenuItem(DeviceContextMenu, NOTIFICATION_PROPERTIES, _("Notifications"));
	NotificationsMenuItem->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	NotificationsMenuItem->SetTextColour(RGB_COLOURDEF_WHITE);
	wxMenuItem* PropertiesMenuItem = new wxMenuItem(DeviceContextMenu, _DEVICE_PROPERTIES_, _("Properties"));
	PropertiesMenuItem->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	PropertiesMenuItem->SetTextColour(RGB_COLOURDEF_WHITE);

	DeviceContextMenu->Append(ObjectDetectionMenuItem);
	DeviceContextMenu->Append(FaceRecognitionMenuItem);
	DeviceContextMenu->Append(MotionDetectionMenuItem);
	DeviceContextMenu->Append(NotificationsMenuItem);
	DeviceContextMenu->Append(PropertiesMenuItem);

	for (int c = 0; c < (int)SCNNBSSMVMParametersList.size(); c++)
	{
		if (GetItemText(GetFocusedItem()) == SCNNBSSMVMParametersList[c]->m_MVMName)
		{
			SNNBSSMVMData* _CurrentDeviceParameter = SCNNBSSMVMParametersList[c];

			const int id = GetPopupMenuSelectionFromUser(*DeviceContextMenu);
			switch (id)
			{
			case OBJECT_DETECTION_PROPERTIES:
				CNNBSSDevicePropertiesDialogAddressHandle(_CurrentDeviceParameter, GetActiveUXFrame());
				CNNBSSDevicePropertiesDialogAddressHandle(_CurrentDeviceParameter, GetActiveUXFrame())->ShowModal();
				break;
			case FACE_RECOGNITION_PROPERTIES:
				break;
			case MOTION_DETECTION_PROPERTIES:
				break;
			case NOTIFICATION_PROPERTIES:
				break;
			case _DEVICE_PROPERTIES_:
				break;
			default:
				break;
			}

			break;
		}
	}

	evt.Skip();
}
void CNNBSSDeviceListTreeCtrl::_UpdateList()
{
	DeleteAllItems();

	wxTreeItemId m_root = AddRoot(_("Devices"), 5, 5);

	for (const auto& p : SCNNBSSMVMParametersList)
	{
		if (p->m_MVMType == NNBSS_MVM_TYPE_VIDEO)
			continue;

		int image = -1;
		switch (p->m_MVMType)
		{
		case NNBSS_MVM_TYPE_USBCAM:
			image = 0;
			break;
		case NNBSS_MVM_TYPE_IPCAM:
			image = 1;
			break;
		case NNBSS_MVM_TYPE_DVR:
			image = 2;
			break;
		case NNBSS_MVM_TYPE_NVR:
			image = 3;
			break;
		case NNBSS_MVM_TYPE_MICROPHONE:
			image = 4;
			break;
		default:
			image = 5;
			break;
		}

		wxTreeItemId newItem = AppendItem(m_root, wxString::Format("%s", p->m_MVMName), image, image);

		// Change item colour depending on device status
		if (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_OFFLINE)
		{
			SetItemTextColour(newItem, RGB_COLOURDEF_DAVYSGRAY);
		}
		else if (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE)
		{
			SetItemTextColour(newItem, RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
		}
		else if (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_DISCONNECTED)
		{
			SetItemTextColour(newItem, RGB_COLOURDEF_RED);
		}
		else if (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_LOW_CONNECTION)
		{
			SetItemTextColour(newItem, RGB_COLOURDEF_YELLOWNCS);
		}
		else
		{
			SetItemTextColour(newItem, RGB_COLOURDEF_RED);
		}
	}
	ExpandAll();

	Refresh();
}

void CNNBSSDeviceListTreeCtrl::OnBeginDrag(wxTreeEvent& event)
{

	// need to explicitly allow drag
	if (event.GetItem() != GetRootItem())
	{

		wxTreeItemId m_draggedItem = event.GetItem();

		wxFileDataObject data;
		data.AddFile(wxString::Format("%s", GetItemText(m_draggedItem)));

		wxDropSource dragSource(this);
		dragSource.SetData(data);

		m_DragResult = dragSource.DoDragDrop(wxDrag_DefaultMove);

		switch (m_DragResult)
		{
		case wxDragError:
			break;
		case wxDragNone:
			break;
		case wxDragCopy:
			break;
		case wxDragMove:
			break;
		case wxDragLink:
			break;
		case wxDragCancel:
			break;
		default:
			break;
		}
	}

	event.Skip();
}

void CNNBSSDeviceListTreeCtrl::OnEndDrag(wxTreeEvent& event)
{
	event.Skip();
};