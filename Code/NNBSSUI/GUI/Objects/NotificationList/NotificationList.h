///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef NOTIFICATIONLIST_H
#define NOTIFICATIONLIST_H

#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>

#include <wx/log.h>
#include <wx/dnd.h>

class CNNBSSNotificationList : public wxListView
{
public:
	CNNBSSNotificationList() : _IsCreated(false)// check and remove if is not needed;
	{};
	~CNNBSSNotificationList()
	{
		if (this)
		{
			this->CallAfter(
				[=]()
				{
					if (this->GetChildren().GetCount() > 0)
						this->DestroyChildren();
					this->Destroy();
				});
		}
	};

	void _Create(wxWindow* parent,
		const wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0L | wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL | wxLC_NO_HEADER | wxNO_BORDER)
	{
		if (_IsCreated)
		{
			if (this)
			{
				if (this->GetChildren().GetCount() > 0)
				{
					this->DestroyChildren();
				}
				this->Destroy();
			}
		}
		if (!_IsCreated)
		{
			Create(parent, this->GetId(), pos, size, style);

			_SetupInterface();

			_IsCreated = true;
		}
	};

	void _RecreateList();
	void _RemoveSelectedFromList();

	void _SelectSingle(long itemIndex);
	int _GetSelectedItemIndex();
	int _GetItemCount();

	void _OnContextMenuPopUp(wxListEvent& event);
	void _OnContextMenuItemClick(wxCommandEvent& event);

	void _SetupInterface();
	void _UpdateInterface();

	void _SetupColours(wxColour backgroundColour, wxColour foregroundColour);

	void _AddHeader();
	void _AddColumns();

public:
	bool _IsCreated;

private:
	int _selectedItem = -1;

private:
	void _OnItemSelect(wxListEvent& event);
	void _OnSizeChange(wxSizeEvent& event);
};

#endif // ! NOTIFICATIONLIST_H