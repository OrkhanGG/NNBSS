// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "ViewPage.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

#define NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_HOVERCOLOUR wxString(_("P_HoverColour")) 
#define NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_ICONCOLOUR_0 wxString(_("P_HeaderIcons"))
#define NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_ICONCOLOUR_1 wxString(_("P_OtherIcons"))
#define NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_DISABLEDTEXTCOLOUR wxString(_("P_DisabledTextColour"))
#define NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_CAMERAPANELSELECTCOLOUR _("P_SelectedCameraPanelUnderline")
#define NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_BACKGROUNDCOLOUR _("P_BackgroundColour")

void CNNBSSViewPage::_SetupInterface()
{
	_SetupDesign();

	wxInitAllImageHandlers();

	// Create
	p_MainSizer = new wxBoxSizer(wxVERTICAL);

	p_NotebookSizer = new wxBoxSizer(wxVERTICAL);

	p_SubButtonAreaSizer = new wxBoxSizer(wxHORIZONTAL);
	p_SubButtonAreaVerticalSizer = new wxBoxSizer(wxVERTICAL);
	p_SubButtonArea = new wxPanel(this, wxID_ANY);

	p_AcceptButton = new wxButton(p_SubButtonArea, NNBSS_EVT_OK, _("Save"), wxDefaultPosition, wxDefaultSize,
		wxNO_BORDER);
	p_DeclineButton = new wxButton(p_SubButtonArea, NNBSS_EVT_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize,
		wxNO_BORDER);
	p_ResetButton = new wxButton(p_SubButtonArea, NNBSS_EVT_RESET, _("Reset"), wxDefaultPosition, wxDefaultSize,
		wxNO_BORDER);

	p_ViewOptions = new wxFlatNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	p_ViewOptionsImageList = new wxFlatNotebookImageList();

	// Add pages with icons
	p_ViewOptionsImageList->Add(NNBSS_PNG2BMP(NNBSSART_GRIDVIEW_2x2_IDLE));
	p_ViewOptions->SetImageList(p_ViewOptionsImageList);

	p_AppearancePanel = new CNNBSSAppearancePanel(p_ViewOptions, wxID_ANY);

	p_ViewOptions->AddPage(p_AppearancePanel, _("Appearance"), true, 0);
	p_ViewOptions->AddPage(_LayoutPanel(), _("Layout"), true, 0);

	// SubButtonAreaSizers handling
	p_SubButtonAreaSizer->Add(p_AcceptButton, wxSizerFlags().Proportion(0));
	p_SubButtonAreaSizer->AddSpacer(FromDIP(10));
	p_SubButtonAreaSizer->Add(p_DeclineButton, wxSizerFlags().Proportion(0));
	p_SubButtonAreaSizer->AddSpacer(FromDIP(10));
	p_SubButtonAreaSizer->Add(p_ResetButton, wxSizerFlags().Proportion(0));
	p_SubButtonAreaVerticalSizer->Add(p_SubButtonAreaSizer, wxSizerFlags().Proportion(0).Align(wxALIGN_CENTER_HORIZONTAL));
	p_SubButtonArea->SetSizer(p_SubButtonAreaVerticalSizer);

	p_NotebookSizer->Add(p_ViewOptions, wxSizerFlags().Proportion(1).Expand());
	p_NotebookSizer->Add(p_SubButtonArea, wxSizerFlags().Proportion(0).Expand());

	p_MainSizer->Add(p_NotebookSizer, wxSizerFlags().Proportion(1).Expand());
	p_MainSizer->FitInside(this);
	SetSizer(p_MainSizer);

	_SetupDesign();

	_BindEvents(true);

}

void CNNBSSViewPage::_SetupDesign()
{
	if (!p_ViewOptions)
		return;

	p_ViewOptions->SetThemeEnabled(true);
	p_ViewOptions->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_ViewOptions->SetTabAreaColour(RGB_COLOURDEF_TRIPLE20);
	p_ViewOptions->SetActiveTabColour(RGB_COLOURDEF_TRIPLE20);
	p_ViewOptions->SetActiveTabTextColour(RGB_COLOURDEF_WHITE);
	p_ViewOptions->SetNonActiveTabTextColour(RGB_COLOURDEF_TRIPLE100);
	p_ViewOptions->SetSingleLineBorderColor(RGB_COLOURDEF_NNBSSBLUE);
	p_ViewOptions->SetWindowStyle(wxFNB_NO_X_BUTTON | wxFNB_NO_NAV_BUTTONS | wxFNB_NODRAG);


	// SubButtons
	p_SubButtonArea->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	p_AcceptButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_AcceptButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_AcceptButton->SetBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_IDLE));
	p_AcceptButton->SetBitmapDisabled(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_DISABLED));

	p_DeclineButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_DeclineButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_DeclineButton->SetBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_IDLE));

	p_ResetButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_ResetButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_ResetButton->SetBitmap(NNBSS_PNG2BMP(NNBSSART_GLOBAL_RESET_16x16p_IDLE));
	/// End subbuttons
}

void CNNBSSViewPage::_BindEvents(bool bind)
{
	// Binding
	if (bind)
	{
		p_AcceptButton->Bind(wxEVT_LEFT_UP, &CNNBSSViewPage::OnAnyButtonClick, this);
		p_DeclineButton->Bind(wxEVT_LEFT_UP, &CNNBSSViewPage::OnAnyButtonClick, this);
		p_ResetButton->Bind(wxEVT_LEFT_UP, &CNNBSSViewPage::OnAnyButtonClick, this);

	}
	else
	{
		p_AcceptButton->Unbind(wxEVT_LEFT_UP, &CNNBSSViewPage::OnAnyButtonClick, this);
		p_DeclineButton->Unbind(wxEVT_LEFT_UP, &CNNBSSViewPage::OnAnyButtonClick, this);
		p_ResetButton->Unbind(wxEVT_LEFT_UP, &CNNBSSViewPage::OnAnyButtonClick, this);
	}
}

bool CNNBSSViewPage::CheckDialogDemandMeet()
{
	return true;
}

void CNNBSSViewPage::OnAnyButtonClick(wxMouseEvent& event)
{
	CNNBSSMouseEventsAddressHandle()->OnAnyButtonClick(event);

	bool destroy = false;

	// Local process
	switch (event.GetId())
	{
	case NNBSS_EVT_OK:
		if (CheckDialogDemandMeet())
		{
			EndModal(NNBSS_EVT_OK);

			destroy = true;
		}
		break;
	case NNBSS_EVT_CANCEL:
		EndModal(NNBSS_EVT_CANCEL);
		destroy = true;
		break;
	default:
		break;
	}

	event.Skip();

	if (destroy)
	{
		if (this->GetChildren().GetCount() > 0)
		{
			for (wxWindowList::iterator it = this->GetChildren().begin();
				it != this->GetChildren().end(); it++) {

				wxWindow* Child = dynamic_cast<wxWindow*>(*it);

				if (Child) {
					Child->Hide();
				}

			}
		}
		// Then Destroy dialog
		this->Destroy();
		// To use this dialog again(Otherwise you'll get crash):
		CNNBSSViewPageAddressHandle(true);
	}
}


wxScrolledWindow* CNNBSSViewPage::_LayoutPanel()
{
	p_LP = new wxScrolledWindow(p_ViewOptions, wxID_ANY);

	p_LP->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	wxBoxSizer* p_ButtonsAreaSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* p_ButtonsAreaSizerVerticalSizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* p_ButtonsArea = new wxPanel(p_LP, wxID_ANY);

	wxButton* p_SaveLayoutButton = new wxButton(p_ButtonsArea, NNBSS_EVT_OK, _("Save Layout"), wxDefaultPosition, wxDefaultSize,
		wxNO_BORDER);
	wxButton* p_RemoveLayotButton = new wxButton(p_ButtonsArea, NNBSS_EVT_CANCEL, _("Remove Layout"), wxDefaultPosition, wxDefaultSize,
		wxNO_BORDER);
	wxButton* p_ResetLayotButton = new wxButton(p_ButtonsArea, NNBSS_EVT_RESET, _("Reset Layout"), wxDefaultPosition, wxDefaultSize,
		wxNO_BORDER);

	// SubButtonAreaSizers handling
	p_ButtonsAreaSizer->Add(p_SaveLayoutButton, wxSizerFlags().Proportion(0));
	p_ButtonsAreaSizer->AddSpacer(FromDIP(10));
	p_ButtonsAreaSizer->Add(p_RemoveLayotButton, wxSizerFlags().Proportion(0));
	p_ButtonsAreaSizer->AddSpacer(FromDIP(10));
	p_ButtonsAreaSizer->Add(p_ResetLayotButton, wxSizerFlags().Proportion(0));
	p_ButtonsAreaSizerVerticalSizer->Add(p_ButtonsAreaSizer, wxSizerFlags().Proportion(0).Align(wxALIGN_CENTER_HORIZONTAL));
	p_ButtonsArea->SetSizer(p_ButtonsAreaSizerVerticalSizer);

	wxBoxSizer* p_LayoutPanelMainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* p_LayoutListSizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString _test;
	for(int c= 0; c< 9; c++)
		_test.Add(wxString::Format("Layout %i",c));

#if 0
	wxButton** p_TestButton = new wxButton*[20];

	int c = 0;
	for(const auto& p : _test)
	{ 
		c++;
		p_TestButton[c] = new wxButton(p_LP, wxID_ANY, p, wxDefaultPosition,
			wxDefaultSize, wxNO_BORDER);

		p_TestButton[c]->SetBackgroundColour(RGB_COLOURDEF_TRIPLE25);
		p_TestButton[c]->SetForegroundColour(RGB_COLOURDEF_WHITE);

		p_LayoutListSizer->Add(p_TestButton[c], wxSizerFlags().Proportion(1).Expand());
	}
#endif 
	wxListCtrl* p_LayoutList = new wxListCtrl(p_LP, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_NO_HEADER | wxNO_BORDER);

	p_LayoutList->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_LayoutList->SetForegroundColour(RGB_COLOURDEF_WHITE);

	p_LayoutList->InsertColumn(0, _("Row Number"), wxLIST_FORMAT_CENTER, (this->GetSize().x/3)-2);
	p_LayoutList->InsertColumn(1, _("Layout Name"), wxLIST_FORMAT_CENTER, (this->GetSize().x/ 3)-2);
	p_LayoutList->InsertColumn(2, _("Description"), wxLIST_FORMAT_CENTER, (this->GetSize().x/ 3)-2);

	// Add header
	int itemIndex = 0;
	p_LayoutList->InsertItem(itemIndex, _("Header"));
	p_LayoutList->SetItem(itemIndex, 0, _("Row Number"));
	p_LayoutList->SetItem(itemIndex, 1, _("Layout Name"));
	p_LayoutList->SetItem(itemIndex, 2, _("Description"));
	p_LayoutList->SetItemBackgroundColour(itemIndex, RGB_COLOURDEF_TRIPLE30);

	p_LayoutListSizer->Add(p_LayoutList, wxSizerFlags().Proportion(1).Expand());
	
	p_LayoutPanelMainSizer->Add(p_ButtonsArea, wxSizerFlags().Proportion(0).Expand());
	p_LayoutPanelMainSizer->Add(p_LayoutListSizer, wxSizerFlags().Proportion(1).Expand());

	p_LayoutPanelMainSizer->FitInside(p_LP);
	p_LP->SetSizer(p_LayoutPanelMainSizer);

	p_LP->SetScrollRate(5, 5);

	return p_LP;
}



CNNBSSAppearancePanel::CNNBSSAppearancePanel(wxWindow* parent,
	wxWindowID winid, wxPoint pos, wxSize size, long style, wxString name)

	: wxScrolledWindow(parent,winid,pos,size,style,name)

{
	p_AP = new wxScrolledCanvas(this, wxID_ANY);

	p_AP->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	p_AP_MainSizer = new wxBoxSizer(wxVERTICAL);

	p_PropGrid = new wxPropertyGrid(p_AP, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxPG_SPLITTER_AUTO_CENTER);

	p_PropGrid->Append(new wxColourProperty("Hover colour",
		NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_HOVERCOLOUR,
		RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT));

	p_PropGrid->Append(new wxColourProperty("Header icons",
		NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_ICONCOLOUR_0,
		RGB_COLOURDEF_NNBSSPURPLE));

	p_PropGrid->Append(new wxColourProperty("Other icons",
		NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_ICONCOLOUR_1,
		RGB_COLOURDEF_WHITE));

	p_PropGrid->Append(new wxColourProperty("Disabled text colour",
		NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_DISABLEDTEXTCOLOUR,
		RGB_COLOURDEF_GRAY));

	p_PropGrid->Append(new wxColourProperty("Selected camera panel underline",
		NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_CAMERAPANELSELECTCOLOUR,
		RGB_COLOURDEF_NNBSSBLUE));

	p_PropGrid->Append(new wxColourProperty("Background Colour",
		NNBSS_VIEWPAGE_APPEARANCE_PROPERTYNAME_BACKGROUNDCOLOUR,
		RGB_COLOURDEF_TRIPLE20));

	p_AP_MainSizer->AddSpacer(FromDIP(20));
	p_AP_MainSizer->Add(p_PropGrid, wxSizerFlags().Proportion(1).Expand());

	p_AP_MainSizer->FitInside(this);
	//p_AP_MainSizer->Layout();
	p_AP->SetSizerAndFit(p_AP_MainSizer);

	// Move these into setup design
	p_PropGrid->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_PropGrid->SetCaptionBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_PropGrid->SetCaptionTextColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	p_PropGrid->SetCellBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_PropGrid->SetCellDisabledTextColour(RGB_COLOURDEF_JET);
	p_PropGrid->SetCellTextColour(RGB_COLOURDEF_WHITE);
	p_PropGrid->SetEmptySpaceColour(RGB_COLOURDEF_TRIPLE20);
	p_PropGrid->SetLineColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	p_PropGrid->SetMarginColour(RGB_COLOURDEF_TRIPLE20);
	p_PropGrid->SetSelectionBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_PropGrid->SetSelectionTextColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);

	p_PropGrid->CenterSplitter(true);

	p_PropGrid->Bind(wxEVT_PG_CHANGING, &CNNBSSAppearancePanel::OnPropertyGridChanged, this);

}

void CNNBSSAppearancePanel::OnPropertyGridChanged(wxPropertyGridEvent& event)
{
	wxPGProperty* property = event.GetProperty();
	// Do nothing if event did not have associated property
	if (!property)
		return;
}