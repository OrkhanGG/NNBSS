///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef DEVICELIST_H
#define DEVICELIST_H

// wxWidgets
#include <wx/listctrl.h>
#include <wx/dnd.h>

class CNNBSSDeviceList : public wxListView
{
public:
	CNNBSSDeviceList() : _IsCreated(false)// check and remove if is not needed;
	{};
	~CNNBSSDeviceList();

	void _Create(wxWindow* parent,
		const wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxLC_REPORT | wxLC_HRULES | wxLC_SINGLE_SEL | wxLC_NO_HEADER | wxNO_BORDER);

	void _RecreateList();
	void _RemoveSelectedFromList();

	void _AddHeader();
	void _AddColumns();

	void _SelectSingle(const long& itemIndex);
	int _GetSelectedItemIndex();
	int _GetItemCount();

	void _SetupColours(wxColour backgroundColour, wxColour foregroundColour);

	bool IsCreated() const { return _IsCreated; };
private:
	void _SetupInterface();
	void _UpdateInterface();

	void OnKeyRelease(wxKeyEvent& event);
	void _OnContextMenuPopUp(wxListEvent& event);
	void _OnContextMenuItemClick(wxCommandEvent& event);
	void _OnItemSelect(wxListEvent& event);
	void _OnSizeChange(wxSizeEvent& event);
	void OnErase(wxEraseEvent& event) {};

	wxImageList* p_ElementIcons = nullptr;

	int _selectedItem = -1;
	bool _IsCreated;
};

//

#include <wx/treectrl.h>

class CNNBSSDeviceListTreeCtrl : public wxTreeCtrl
{
public:
	CNNBSSDeviceListTreeCtrl();
	~CNNBSSDeviceListTreeCtrl();

	void _Create(wxWindow* parent, wxWindowID winid = -1,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = 9L, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxT("DefaultDLTCName"));
public:
	void _UpdateList();

	wxTreeCtrl* GetPanel()
	{
		return this;
	}
private:
	void _SetupInterface();

	void _BindEvents(bool bind = true);

	void OnContextMenu(wxTreeEvent& evt);

	void OnBeginDrag(wxTreeEvent& event);
	void OnEndDrag(wxTreeEvent& event);

	wxDragResult m_DragResult;

};

#endif // ! DEVICELIST_H