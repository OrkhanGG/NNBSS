///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#include "NotificationList.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

// ID defintions
	// ContextMenu(Popup)

void CNNBSSNotificationList::_RecreateList()
{
	this->DeleteAllItems();

	// to speed up inserting we hide the control temporarily
	this->Hide();

	int RowNumber = 0;

	for (const auto& p : NNBSSNotifications::SNNBSSNotificationAttributesList)
	{
		// container
		wxString buffer;

		// Row number
		RowNumber++;
		buffer.Printf("%i", RowNumber);

		// Set tmp as index and insert item
		long tmp = InsertItem(RowNumber, buffer, -1);
		SetItemData(tmp, RowNumber);

		// Device Name
		buffer.Printf("%s", p.DetectionName);
		SetItem(tmp, 1, buffer);

		// Address
		buffer.Printf("%s", p.Message);
		SetItem(tmp, 2, buffer);

		// Device Type
		buffer.Printf("%s", p.DateAndTime);
		SetItem(tmp, 3, buffer);

		// Status
		buffer.Printf("%s", p.AdditionalNotes);
		SetItem(tmp, 4, buffer);
	}

	// Select last one
	_SelectSingle(this->GetItemCount() - 1 < 0 ? 0 : this->GetItemCount() - 1);

	// Add header whether we have element(s) or not.
	_AddHeader();

	this->Show();
}

void CNNBSSNotificationList::_RemoveSelectedFromList()
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

	if (_GetSelectedItemIndex() >= 0)
	{
		int indexToRemove = (_GetSelectedItemIndex()) < 0 ? 0 : (_GetSelectedItemIndex());
		DeviceList::CNNBSSDeviceListRemoveCache(indexToRemove);

		_RecreateList();
	}
}

void CNNBSSNotificationList::_SelectSingle(long itemIndex)
{
	if (this->GetItemCount() > 0)
	{
		this->SetFocus();
		this->Select(itemIndex);
		_selectedItem = itemIndex;
	}
}

int CNNBSSNotificationList::_GetSelectedItemIndex()
{
	return _selectedItem;
}

int CNNBSSNotificationList::_GetItemCount()
{
	return this->GetItemCount();
}

void CNNBSSNotificationList::_OnContextMenuPopUp(wxListEvent& event)
{
	if (event.GetIndex() <= 0)
	{
		event.Skip();
		return;
	}
	wxMenu DeviceContextMenu;

	DeviceContextMenu.Append(wxID_ANY, _("Remove"));
	DeviceContextMenu.Append(wxID_ANY, _("Details"));
	
	DeviceContextMenu.Bind(wxEVT_COMMAND_MENU_SELECTED, &CNNBSSNotificationList::_OnContextMenuItemClick, this);
	PopupMenu(&DeviceContextMenu);

	event.Skip();
}

void CNNBSSNotificationList::_OnContextMenuItemClick(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	default:
		break;
	}

	event.Skip();
}

void CNNBSSNotificationList::_SetupInterface()
{
	// freeze this so we may commit all changes accurately after thaw.
	this->Freeze();

	// Add columns
	_AddColumns();

	// This will add the items on start(if we have) and the header.
	_UpdateInterface();

	// Accepted as default theme
	_SetupColours(RGB_COLOURDEF_TRIPLE20, RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);

	// After freeze, we need to thaw.
	this->Thaw();

	// Setup event(s)
	this->Bind(wxEVT_LIST_ITEM_SELECTED, &CNNBSSNotificationList::_OnItemSelect, this);
	this->Bind(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, &CNNBSSNotificationList::_OnContextMenuPopUp, this);
	this->Bind(wxEVT_SIZE, &CNNBSSNotificationList::_OnSizeChange, this);
}

void CNNBSSNotificationList::_UpdateInterface()
{
	_RecreateList();
}

void CNNBSSNotificationList::_SetupColours(wxColour backgroundColour, wxColour foregroundColour)
{
	this->SetBackgroundColour(backgroundColour);
	this->SetForegroundColour(foregroundColour);
}

void CNNBSSNotificationList::_AddHeader()
{
	int itemIndex = 0;
	this->InsertItem(itemIndex, _("Header"),-1);

	// use this function to add text
	// long SetItem(long index, int col, const wxString& label, int imageId = -1)

	this->SetItem(itemIndex, 0, _("Row Number"),-1);
	this->SetItem(itemIndex, 1, _("Detection Name"),-1);
	this->SetItem(itemIndex, 2, _("Message"),-1);
	this->SetItem(itemIndex, 3, _("Date and Time"),-1);
	this->SetItem(itemIndex, 4, _("Additional Notes"),-1);


}

void CNNBSSNotificationList::_AddColumns()
{
	this->InsertColumn(0, _("Row Number"), wxLIST_FORMAT_CENTER);
	this->InsertColumn(1, _("Device Name"), wxLIST_FORMAT_CENTER);
	this->InsertColumn(2, _("Address/Port"), wxLIST_FORMAT_CENTER);
	this->InsertColumn(3, _("Device Type"), wxLIST_FORMAT_CENTER);
	this->InsertColumn(4, _("Status"), wxLIST_FORMAT_CENTER);
}

void CNNBSSNotificationList::_OnItemSelect(wxListEvent& event)
{
	if (event.GetIndex() == 0)
	{
		this->Select(0, false);
		event.Veto();
	}

	_selectedItem = event.GetIndex() - 1;// because of header

	event.Skip();
}

void CNNBSSNotificationList::_OnSizeChange(wxSizeEvent& event)
{
	if (this == nullptr)
		return;

	this->Freeze();

	int firstColumnWidth = 150;
	int ExceptFirstColumn = this->GetColumnCount() - 1;
	int betterFit = 0;//to prevent it from overflowing.

	int FitSize =
		((this->GetSize().x - firstColumnWidth) / ExceptFirstColumn) - betterFit;

	this->SetColumnWidth(0, firstColumnWidth);
	for (int c = 1; c < this->GetColumnCount(); c++) {
		this->SetColumnWidth(c, FitSize);
	}

	this->Thaw();
}