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
#include <wx/colourdata.h>
#include <wx/colordlg.h>

#include "ControlPanel.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

#define MENU_EDIT_DELETE_ALL				10000
#define MENU_EDIT_ADD_PAGE					10001
#define MENU_EDIT_DELETE_PAGE				10002
#define MENU_EDIT_SET_SELECTION				10003
#define MENU_EDIT_ADVANCE_SELECTION_FWD		10004
#define MENU_EDIT_ADVANCE_SELECTION_BACK	10005
#define MENU_SET_ALL_TABS_SHAPE_ANGLE		10006
#define MENU_SHOW_IMAGES					10007
#define MENU_USE_VC71_STYLE					10008
#define MENU_USE_DEFULT_STYLE				10009
#define MENU_USE_FANCY_STYLE				10010
#define MENU_SELECT_GRADIENT_COLOR_FROM		10011
#define MENU_SELECT_GRADIENT_COLOR_TO		10012
#define MENU_SELECT_GRADIENT_COLOR_BORDER	10013
#define MENU_SET_PAGE_IMAGE_INDEX			10014
#define MENU_HIDE_X							10015
#define MENU_HIDE_NAV_BUTTONS				10016
#define MENU_USE_MOUSE_MIDDLE_BTN			10017
#define MENU_DRAW_BORDER					10018
#define MENU_USE_BOTTOM_TABS				10019
#define MENU_ENABLE_TAB						10020
#define MENU_DISABLE_TAB					10021
#define MENU_ENABLE_DRAG_N_DROP				10022
#define MENU_DCLICK_CLOSES_TAB				10023
#define MENU_USE_VC8_STYLE					10024
#define MENU_USE_FF2_STYLE					10025

#define MENU_SET_ACTIVE_TEXT_COLOR			10027
#define MENU_DRAW_TAB_X						10028
#define MENU_SET_ACTIVE_TAB_COLOR			10029
#define MENU_SET_TAB_AREA_COLOR				10030
#define MENU_SELECT_NONACTIVE_TEXT_COLOR	10031
#define MENU_GRADIENT_BACKGROUND			10032
#define MENU_COLORFULL_TABS					10033
#define MENU_SMART_TABS						10034
#define MENU_USE_DROP_ARROW_BUTTON			10035
#define MENU_ALLOW_FOREIGN_DND				10036
#define MENU_EDIT_INSERT_PAGE				10037
#define MENU_EDIT_INSERT_BEFORE_PAGE		10038
#define MENU_EDIT_SELECT_PAGE_TO_DELETE		10039

BEGIN_EVENT_TABLE(CNNBSSControlPanel, wxFlatNotebook)
EVT_MENU_RANGE(MENU_HIDE_X, MENU_USE_BOTTOM_TABS, CNNBSSControlPanel::OnStyle)
EVT_MENU(wxID_EXIT, CNNBSSControlPanel::OnQuit)
EVT_MENU(MENU_EDIT_DELETE_ALL, CNNBSSControlPanel::OnDeleteAll)
EVT_MENU(MENU_EDIT_ADD_PAGE, CNNBSSControlPanel::OnAddPage)
EVT_MENU(MENU_EDIT_DELETE_PAGE, CNNBSSControlPanel::OnDeletePage)
EVT_MENU(MENU_EDIT_SET_SELECTION, CNNBSSControlPanel::OnSetSelection)
EVT_MENU(MENU_EDIT_ADVANCE_SELECTION_FWD, CNNBSSControlPanel::OnAdvanceSelectionFwd)
EVT_MENU(MENU_EDIT_ADVANCE_SELECTION_BACK, CNNBSSControlPanel::OnAdvanceSelectionBack)
EVT_MENU(MENU_SET_ALL_TABS_SHAPE_ANGLE, CNNBSSControlPanel::OnSetAllPagesShapeAngle)
EVT_MENU(MENU_SHOW_IMAGES, CNNBSSControlPanel::OnShowImages)
EVT_MENU_RANGE(MENU_SELECT_GRADIENT_COLOR_FROM, MENU_SELECT_GRADIENT_COLOR_BORDER, CNNBSSControlPanel::OnSelectColor)
EVT_MENU(MENU_USE_VC71_STYLE, CNNBSSControlPanel::OnVC71Style)
EVT_MENU(MENU_USE_VC8_STYLE, CNNBSSControlPanel::OnVC8Style)
EVT_MENU(MENU_USE_DEFULT_STYLE, CNNBSSControlPanel::OnDefaultStyle)
EVT_MENU(MENU_USE_FANCY_STYLE, CNNBSSControlPanel::OnFancyStyle)
EVT_MENU(MENU_USE_FF2_STYLE, CNNBSSControlPanel::OnFF2Style)
EVT_MENU(MENU_SET_PAGE_IMAGE_INDEX, CNNBSSControlPanel::OnSetPageImageIndex)
EVT_MENU(MENU_ENABLE_TAB, CNNBSSControlPanel::OnEnableTab)
EVT_MENU(MENU_DISABLE_TAB, CNNBSSControlPanel::OnDisableTab)
EVT_MENU(MENU_ENABLE_DRAG_N_DROP, CNNBSSControlPanel::OnEnableDrag)
EVT_MENU(MENU_SELECT_NONACTIVE_TEXT_COLOR, CNNBSSControlPanel::OnSelectColor)
EVT_MENU(MENU_SET_ACTIVE_TAB_COLOR, CNNBSSControlPanel::OnSelectColor)
EVT_MENU(MENU_SET_TAB_AREA_COLOR, CNNBSSControlPanel::OnSelectColor)
EVT_MENU(MENU_SET_ACTIVE_TEXT_COLOR, CNNBSSControlPanel::OnSelectColor)
EVT_MENU(MENU_DRAW_TAB_X, CNNBSSControlPanel::OnDrawTabX)
EVT_MENU(MENU_DCLICK_CLOSES_TAB, CNNBSSControlPanel::OnDClickCloseTab)
EVT_MENU(MENU_GRADIENT_BACKGROUND, CNNBSSControlPanel::OnGradientBack)
EVT_MENU(MENU_COLORFULL_TABS, CNNBSSControlPanel::OnColorfullTabs)
EVT_MENU(MENU_SMART_TABS, CNNBSSControlPanel::OnSmartTabs)
EVT_MENU(MENU_USE_DROP_ARROW_BUTTON, CNNBSSControlPanel::OnDropDownArrow)
EVT_MENU(MENU_ALLOW_FOREIGN_DND, CNNBSSControlPanel::OnAllowForeignDnd)

EVT_UPDATE_UI(MENU_USE_DROP_ARROW_BUTTON, CNNBSSControlPanel::OnDropDownArrowUI)
EVT_UPDATE_UI(MENU_HIDE_NAV_BUTTONS, CNNBSSControlPanel::OnHideNavigationButtonsUI)
EVT_UPDATE_UI(MENU_ALLOW_FOREIGN_DND, CNNBSSControlPanel::OnAllowForeignDndUI)

EVT_FLATNOTEBOOK_PAGE_CHANGING(-1, CNNBSSControlPanel::OnPageChanging)
EVT_FLATNOTEBOOK_PAGE_CHANGED(-1, CNNBSSControlPanel::OnPageChanged)
EVT_FLATNOTEBOOK_PAGE_CLOSING(-1, CNNBSSControlPanel::OnPageClosing)

EVT_MENU(MENU_EDIT_INSERT_PAGE, CNNBSSControlPanel::OnInsertPage)
EVT_MENU(MENU_EDIT_INSERT_BEFORE_PAGE, CNNBSSControlPanel::OnInsertBeforeGivenPage)
EVT_MENU(MENU_EDIT_SELECT_PAGE_TO_DELETE, CNNBSSControlPanel::OnDeleteGivenPage)
END_EVENT_TABLE()


CNNBSSControlPanel::~CNNBSSControlPanel()
{
}

void CNNBSSControlPanel::_Create(wxWindow* parent, wxWindowID winid,
	const wxPoint& pos, const wxSize& size, long style, const wxString& name)
{
	Create(parent, winid, pos, size, style);

	_SetupInterface();
}

CNNBSSControlPanel* CNNBSSControlPanel::GetPanel()
{
	return this;
}

void CNNBSSControlPanel::_SetupInterface()
{
	_DefaultTheme();

	// Create a right click menu
	wxMenu* RightClickMenu = new wxMenu();
	wxMenuItem* item = new wxMenuItem(RightClickMenu, wxID_EXIT, wxEmptyString, wxEmptyString, wxITEM_NORMAL);
	item = new wxMenuItem(RightClickMenu, MENU_EDIT_DELETE_PAGE, wxT("Close Tab"), wxT("Close Tab"), wxITEM_NORMAL);
	item->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	item->SetTextColour(RGB_COLOURDEF_WHITE);
	RightClickMenu->Append(item);

	SetRightClickMenu(RightClickMenu);
}

void CNNBSSControlPanel::_DefaultTheme()
{
	Freeze();

	SetThemeEnabled(true);
	SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	SetTabAreaColour(RGB_COLOURDEF_TRIPLE20);
	SetActiveTabColour(RGB_COLOURDEF_TRIPLE20);
	SetActiveTabTextColour(RGB_COLOURDEF_WHITE);
	SetNonActiveTabTextColour(RGB_COLOURDEF_TRIPLE100);
	SetSingleLineBorderColor(RGB_COLOURDEF_PLACEHOLDER);
	SetWindowStyle(wxFNB_NO_X_BUTTON | wxFNB_X_ON_TAB | wxFNB_MOUSE_MIDDLE_CLOSES_TABS);//wxFNB_NODRAG may be necessarry, not tested yet
	//this->SetWindowStyle(wxFNB_BOTTOM); // DRAW TABS BOTTOM

	Thaw();
	//-----------------------------------------------------------------

	CNNBSSAuiManagerAddressHandle()->Update();
}

// ---------------------------------------------------------------------------------------


void CNNBSSControlPanel::OnStyle(wxCommandEvent& event)
{
	long style = this->GetWindowStyleFlag();
	switch (event.GetId())
	{
	case MENU_HIDE_NAV_BUTTONS:
		if (event.IsChecked())
		{
			// Hide the navigation buttons
			style |= wxFNB_NO_NAV_BUTTONS;
		}
		else
		{
			style &= ~(wxFNB_NO_NAV_BUTTONS);
			style &= ~(wxFNB_DROPDOWN_TABS_LIST);
		}
		this->SetWindowStyleFlag(style);
		break;
	case MENU_HIDE_X:
		if (event.IsChecked())
		{
			// Hide the X button
			style |= wxFNB_NO_X_BUTTON;
		}
		else
		{
			if (style & wxFNB_NO_X_BUTTON)
				style ^= wxFNB_NO_X_BUTTON;
		}
		this->SetWindowStyleFlag(style);
		break;
	case MENU_DRAW_BORDER:
		if (event.IsChecked())
		{
			style |= wxFNB_TABS_BORDER_SIMPLE;
		}
		else
		{
			if (style & wxFNB_TABS_BORDER_SIMPLE)
				style ^= wxFNB_TABS_BORDER_SIMPLE;
		}
		this->SetWindowStyleFlag(style);
		break;
	case MENU_USE_MOUSE_MIDDLE_BTN:
		if (event.IsChecked())
		{
			style |= wxFNB_MOUSE_MIDDLE_CLOSES_TABS;
		}
		else
		{
			if (style & wxFNB_MOUSE_MIDDLE_CLOSES_TABS)
				style ^= wxFNB_MOUSE_MIDDLE_CLOSES_TABS;
		}
		this->SetWindowStyleFlag(style);
		break;
	case MENU_USE_BOTTOM_TABS:
		if (event.IsChecked())
		{
			style |= wxFNB_BOTTOM;
		}
		else
		{
			if (style & wxFNB_BOTTOM)
				style ^= wxFNB_BOTTOM;
		}
		this->SetWindowStyleFlag(style);
		this->Refresh();
		break;
	}
}

void CNNBSSControlPanel::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void CNNBSSControlPanel::OnDeleteAll(wxCommandEvent& WXUNUSED(event))
{
	this->DeleteAllPages();
}

void CNNBSSControlPanel::OnShowImages(wxCommandEvent& event)
{
	m_bShowImages = event.IsChecked();
}

void CNNBSSControlPanel::OnFF2Style(wxCommandEvent& WXUNUSED(event))
{
	long style = this->GetWindowStyleFlag();

	// remove old tabs style
	long mirror = ~(wxFNB_VC71 | wxFNB_VC8 | wxFNB_FANCY_TABS);
	style &= mirror;

	style |= wxFNB_FF2;

	this->SetWindowStyleFlag(style);
}

void CNNBSSControlPanel::OnVC71Style(wxCommandEvent& WXUNUSED(event))
{
	long style = this->GetWindowStyleFlag();

	// remove old tabs style
	long mirror = ~(wxFNB_VC71 | wxFNB_VC8 | wxFNB_FANCY_TABS | wxFNB_FF2);
	style &= mirror;

	style |= wxFNB_VC71;

	this->SetWindowStyleFlag(style);
}

void CNNBSSControlPanel::OnVC8Style(wxCommandEvent& WXUNUSED(event))
{
	long style = this->GetWindowStyleFlag();

	// remove old tabs style
	long mirror = ~(wxFNB_VC71 | wxFNB_VC8 | wxFNB_FANCY_TABS | wxFNB_FF2);
	style &= mirror;

	// set new style
	style |= wxFNB_VC8;

	this->SetWindowStyleFlag(style);
}

void CNNBSSControlPanel::OnDefaultStyle(wxCommandEvent& WXUNUSED(event))
{
	long style = this->GetWindowStyleFlag();

	// remove old tabs style
	long mirror = ~(wxFNB_VC71 | wxFNB_VC8 | wxFNB_FANCY_TABS | wxFNB_FF2);
	style &= mirror;

	this->SetWindowStyleFlag(style);
}

void CNNBSSControlPanel::OnFancyStyle(wxCommandEvent& WXUNUSED(event))
{
	long style = this->GetWindowStyleFlag();

	// remove old tabs style
	long mirror = ~(wxFNB_VC71 | wxFNB_VC8 | wxFNB_FANCY_TABS | wxFNB_FF2);
	style &= mirror;

	style |= wxFNB_FANCY_TABS;
	this->SetWindowStyleFlag(style);
}

void CNNBSSControlPanel::OnSelectColor(wxCommandEvent& event)
{
	// Open a color dialog
	wxColourData data;
	wxColourDialog dlg(this, &data);
	if (dlg.ShowModal() == wxID_OK)
	{
		switch (event.GetId())
		{
		case MENU_SELECT_GRADIENT_COLOR_BORDER:
			this->SetGradientColorBorder(dlg.GetColourData().GetColour());
			break;
		case MENU_SELECT_GRADIENT_COLOR_FROM:
			this->SetGradientColorFrom(dlg.GetColourData().GetColour());
			break;
		case MENU_SELECT_GRADIENT_COLOR_TO:
			this->SetGradientColorTo(dlg.GetColourData().GetColour());
			break;
		case MENU_SET_ACTIVE_TEXT_COLOR:
			this->SetActiveTabTextColour(dlg.GetColourData().GetColour());
			break;
		case MENU_SELECT_NONACTIVE_TEXT_COLOR:
			this->SetNonActiveTabTextColour(dlg.GetColourData().GetColour());
			break;
		case MENU_SET_ACTIVE_TAB_COLOR:
			this->SetActiveTabColour(dlg.GetColourData().GetColour());
			break;
		case MENU_SET_TAB_AREA_COLOR:
			this->SetTabAreaColour(dlg.GetColourData().GetColour());
			break;
		}
		this->Refresh();
	}
}

void CNNBSSControlPanel::OnAddPage(wxCommandEvent& WXUNUSED(event))
{
	CreatePage(NNBSS_EVT_ADD_SERVICES_PAGE);// defaultValue
}

wxWindow* CNNBSSControlPanel::CreatePage(long showPage)
{

	Freeze();

	wxString caption;
	caption.Printf(wxT("Services"));

#if 0
	static int servicesPageID = -1;

	servicesPageID++;
	// find better solution
	//_GetCurrentPageIndex() < 0 ? servicesPageID = 0 : servicesPageID = _GetCurrentPageIndex() + 1;
#endif
	CNNBSSContentPanel* p_NewPage = new CNNBSSContentPanel();

	CNNBSSContentPanelAddressHandle(p_NewPage)->_Create(this, wxID_ANY);

	this->AddPage(CNNBSSContentPanelAddressHandle(p_NewPage)->GetPanel(), caption, true);

	// When you create page, those below will be called according argument
	switch (showPage)
	{
	case NNBSS_EVT_ADD_SERVICES_PAGE:
		CNNBSSContentPanelAddressHandle(p_NewPage)->_ShowServicesContent();// same with default
		break;
	case NNBSS_EVT_SHOW_DEVICES_PANEL:
		CNNBSSContentPanelAddressHandle(p_NewPage)->_ShowDevicesContent();
		break;
	case NNBSS_EVT_SHOW_GRIDVIEW_PANEL:
		CNNBSSContentPanelAddressHandle(p_NewPage)->_ShowGridViewContent();
		break;
	case NNBSS_EVT_SHOW_NOTIFICATIONS_PANEL:
		CNNBSSContentPanelAddressHandle(p_NewPage)->_ShowNotificationsContent();
		break;
	default:
		CNNBSSContentPanelAddressHandle(p_NewPage)->_ShowServicesContent();
		break;
	}

	Thaw();

	CNNBSSAuiManagerAddressHandle()->Update();

	return CNNBSSContentPanelAddressHandle(p_NewPage);
}

long CNNBSSControlPanel::_GetCurrentPageIndex()
{
	return this->GetPageIndex(this->GetCurrentPage());
}

CNNBSSContentPanel* CNNBSSControlPanel::_GetCurrentContentPanel()
{
	wxWindow* _cPage = this->GetCurrentPage();
#if 0
	wxLog::AddTraceMask(DEBUG_TRACE_HELPER);
	if (_cPage != NULL)
	{
		wxString className = _cPage->GetClassInfo()->GetClassName(); // fetch the class info
		wxLogTrace(DEBUG_TRACE_HELPER, "Gotten ClassName:%s", className);

		if (className.Cmp(wxT("CNNBSSContentPanel")) == 0) {
			CNNBSSContentPanel* p_CNNBSSContentPanel = (CNNBSSContentPanel*)_cPage;

			return p_CNNBSSContentPanel;
		}
		else
		{
			// didn't match anything
			wxLogTrace(DEBUG_TRACE_HELPER, "_GetCurrentContentPanel returned anything other than CNNBSSContentPanel class. RETURNING NULLPTR");
		}
	}
#endif

	return (CNNBSSContentPanel*)_cPage;
}

void CNNBSSControlPanel::OnInsertPage(wxCommandEvent& WXUNUSED(event))
{
	size_t index = this->GetSelection();
	Freeze();
	bool ret = InsertNotebookPage((long)index);
	wxUnusedVar(ret);
	Thaw();
}

void CNNBSSControlPanel::OnInsertBeforeGivenPage(wxCommandEvent& WXUNUSED(event))
{
	long tabIdx = GetTabIndexFromUser(wxT("Insert Page"), wxT("Enter Tab Number to insert the page before:"));
	if (tabIdx != -1)
	{
		Freeze();
		bool ret = InsertNotebookPage(tabIdx);
		wxUnusedVar(ret);
		Thaw();
	}
}

void CNNBSSControlPanel::OnDeletePage(wxCommandEvent& WXUNUSED(event))
{
	DeleteNotebookPage(this->GetSelection());
}

void CNNBSSControlPanel::OnDeleteGivenPage(wxCommandEvent& WXUNUSED(event))
{
	long tabIdx = GetTabIndexFromUser(wxT("Delete Page"), wxT("Enter Tab Number to delete:"));
	if (tabIdx != -1)
		DeleteNotebookPage(tabIdx);
}

void CNNBSSControlPanel::OnSetSelection(wxCommandEvent& WXUNUSED(event))
{
	long tabIdx = GetTabIndexFromUser(wxT("Enable Tab"), wxT("Enter Tab Number to enable:"));
	if (tabIdx != -1)
		this->SetSelection((size_t)tabIdx);
}

void CNNBSSControlPanel::OnEnableTab(wxCommandEvent& WXUNUSED(event))
{
	long tabIdx = GetTabIndexFromUser(wxT("Set Selection"), wxT("Enter Tab Number to select:"));
	if (tabIdx != -1)
		this->Enable((size_t)tabIdx, true);
}

void CNNBSSControlPanel::OnDisableTab(wxCommandEvent& WXUNUSED(event))
{
	long tabIdx = GetTabIndexFromUser(wxT("Disable Tab"), wxT("Enter Tab Number to disable:"));
	if (tabIdx != -1)
		this->Enable((size_t)tabIdx, false);
}

void CNNBSSControlPanel::OnEnableDrag(wxCommandEvent& event)
{
	long style = this->GetWindowStyleFlag();
	if (event.IsChecked())
	{
		if (style & wxFNB_NODRAG)
			style ^= wxFNB_NODRAG;
	}
	else
	{
		style |= wxFNB_NODRAG;
	}

	this->SetWindowStyleFlag(style);
}
long CNNBSSControlPanel::GetTabIndexFromUser(const wxString& title, const wxString& prompt) const
{
	wxTextEntryDialog dlg((wxWindow*)this, prompt, title);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString val = dlg.GetValue();
		long tabIdx;
		val.ToLong(&tabIdx);
		return tabIdx;
	}
	return -1;
}

void CNNBSSControlPanel::DeleteNotebookPage(long tabIdx)
{
	this->DeletePage(tabIdx);
}

bool CNNBSSControlPanel::InsertNotebookPage(long tabIdx)
{
	wxString caption = wxT("Services");
	return this->InsertPage(tabIdx, CreatePage(), caption, true, m_bShowImages ? rand() % (int)this->GetImageList()->size() : -1);
}

void CNNBSSControlPanel::OnSetAllPagesShapeAngle(wxCommandEvent& WXUNUSED(event))
{

	wxTextEntryDialog dlg(this, wxT("Enter an inclination of header borders (0-15):"), wxT("Set Angle"));
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString val = dlg.GetValue();
		unsigned int angle = wxAtoi(val);
		this->SetAllPagesShapeAngle(angle);
	}
}

void CNNBSSControlPanel::OnSetPageImageIndex(wxCommandEvent& WXUNUSED(event))
{

	wxTextEntryDialog dlg(this,
		wxString::Format(wxT("Enter an image index (0-%i):"), this->GetImageList()->size() - 1),
		wxT("Set Angle"));
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString val = dlg.GetValue();
		unsigned int imgindex = wxAtoi(val);
		this->SetPageImageIndex(this->GetSelection(), imgindex);
	}
}

void CNNBSSControlPanel::OnAdvanceSelectionFwd(wxCommandEvent& WXUNUSED(event))
{
	this->AdvanceSelection(true);
}

void CNNBSSControlPanel::OnAdvanceSelectionBack(wxCommandEvent& WXUNUSED(event))
{
	this->AdvanceSelection(false);
}

void CNNBSSControlPanel::OnPageChanging(wxFlatNotebookEvent& WXUNUSED(event))
{
}

void CNNBSSControlPanel::OnPageChanged(wxFlatNotebookEvent& event)
{
#ifdef DEVELOPMENT
	wxString msg;
	msg.Printf(wxT("Page has changed, new selection is now=%ld"), event.GetSelection());
	wxLogMessage(msg);
#else
	wxUnusedVar(event);
#endif

	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSControlPanel::OnPageClosing(wxFlatNotebookEvent& event)
{
	CNNBSSContentPanelAddressHandle(_GetCurrentContentPanel(), true);

	// Destroy current panel. Calling each GUI classes' AddressHandle function is not the best way I can think of though.
	// Unfortunately, due to dead-line, I'll keep them this way for a while.
	CNNBSSDeviceListAddressHandle(_GetCurrentContentPanel(), true);
	CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage(), true);
	CNNBSSNotificationListAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage(), true);
	CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage(), true);
	CNNBSSProfilesInterfaceAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage(), true);
	CNNBSSDetectionSamplesListAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage(), true);
	
	event.Skip();

	wxUnusedVar(event);

	if (this->GetPageCount() <= 1)
	{
		CNNBSSControlPanelAddressHandle()->OnAddPage(wxCommandEvent());
	}
}

void CNNBSSControlPanel::OnDrawTabX(wxCommandEvent& event)
{
	long style = this->GetWindowStyleFlag();
	if (event.IsChecked())
	{
		style |= wxFNB_X_ON_TAB;
	}
	else
	{
		if (style & wxFNB_X_ON_TAB)
			style ^= wxFNB_X_ON_TAB;
	}

	this->SetWindowStyleFlag(style);
}

void CNNBSSControlPanel::OnDClickCloseTab(wxCommandEvent& event)
{
	long style = this->GetWindowStyleFlag();
	if (event.IsChecked())
	{
		style |= wxFNB_DCLICK_CLOSES_TABS;
	}
	else
	{
		style &= ~(wxFNB_DCLICK_CLOSES_TABS);
	}

	this->SetWindowStyleFlag(style);
}


void CNNBSSControlPanel::OnGradientBack(wxCommandEvent& event)
{
	long style = this->GetWindowStyleFlag();
	if (event.IsChecked())
		style |= wxFNB_BACKGROUND_GRADIENT;
	else
		style &= ~(wxFNB_BACKGROUND_GRADIENT);
	this->SetWindowStyleFlag(style);
	this->Refresh();
}

void CNNBSSControlPanel::OnColorfullTabs(wxCommandEvent& event)
{
	long style = this->GetWindowStyleFlag();
	if (event.IsChecked())
		style |= wxFNB_COLORFUL_TABS;
	else
		style &= ~(wxFNB_COLORFUL_TABS);
	this->SetWindowStyleFlag(style);
	this->Refresh();
}

void CNNBSSControlPanel::OnSmartTabs(wxCommandEvent& event)
{
	long style = this->GetWindowStyleFlag();
	if (event.IsChecked())
		style |= wxFNB_SMART_TABS;
	else
		style &= ~(wxFNB_SMART_TABS);
	this->SetWindowStyleFlag(style);
	this->Refresh();
}

void CNNBSSControlPanel::OnDropDownArrow(wxCommandEvent& event)
{
	long style = this->GetWindowStyleFlag();
	if (event.IsChecked())
	{
		style |= wxFNB_DROPDOWN_TABS_LIST;
		style |= wxFNB_NO_NAV_BUTTONS;
	}
	else
	{
		style &= ~(wxFNB_DROPDOWN_TABS_LIST);
		style &= ~(wxFNB_NO_NAV_BUTTONS);
	}

	this->SetWindowStyleFlag(style);
	this->Refresh();
}

void CNNBSSControlPanel::OnHideNavigationButtonsUI(wxUpdateUIEvent& event)
{
	long style = this->GetWindowStyleFlag();
	event.Check(style & wxFNB_NO_NAV_BUTTONS ? true : false);
}

void CNNBSSControlPanel::OnDropDownArrowUI(wxUpdateUIEvent& event)
{
	long style = this->GetWindowStyleFlag();
	event.Check(style & wxFNB_DROPDOWN_TABS_LIST ? true : false);
}

void CNNBSSControlPanel::OnAllowForeignDnd(wxCommandEvent& event)
{
	long style = this->GetWindowStyleFlag();
	if (event.IsChecked())
		style |= wxFNB_ALLOW_FOREIGN_DND;
	else
		style &= ~(wxFNB_ALLOW_FOREIGN_DND);
	this->SetWindowStyleFlag(style);
	this->Refresh();
}

void CNNBSSControlPanel::OnAllowForeignDndUI(wxUpdateUIEvent& event)
{
	long style = this->GetWindowStyleFlag();
	event.Enable(style & wxFNB_NODRAG ? false : true);
}

