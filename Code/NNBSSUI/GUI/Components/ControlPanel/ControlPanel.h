///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

// forward declaration
class CNNBSSContentPanel;

// NNBSS UI
#include <NNBSSUI/GUI/Objects/wxFlatNotebook/headers/wxFlatNotebook.h>

class CNNBSSControlPanel : public wxFlatNotebook
{
public:
	CNNBSSControlPanel() : m_bShowImages(false), m_bVCStyle(false) {};
	~CNNBSSControlPanel();

	void _Create(wxWindow* parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxT("DefaultNameControlPanel"));

	CNNBSSControlPanel* GetPanel();

	void _SetupInterface();

private:
	void _DefaultTheme();

public:
	wxWindow* CreatePage(long showPage = 0);
	long _GetCurrentPageIndex();
	CNNBSSContentPanel* _GetCurrentContentPanel();
	wxFlatNotebookImageList m_ImageList;

	// Menu
	virtual void OnInsertPage(wxCommandEvent& WXUNUSED(event));
	virtual void OnInsertBeforeGivenPage(wxCommandEvent& WXUNUSED(event));
	virtual void OnQuit(wxCommandEvent& WXUNUSED(event));
	virtual void OnDeleteAll(wxCommandEvent& WXUNUSED(event));
	virtual void OnAddPage(wxCommandEvent& WXUNUSED(event));
	virtual void OnDeletePage(wxCommandEvent& WXUNUSED(event));
	virtual void OnDeleteGivenPage(wxCommandEvent& WXUNUSED(event));
	virtual void OnSetSelection(wxCommandEvent& WXUNUSED(event));
	virtual void OnAdvanceSelectionFwd(wxCommandEvent& WXUNUSED(event));
	virtual void OnAdvanceSelectionBack(wxCommandEvent& WXUNUSED(event));
	virtual void OnShowImages(wxCommandEvent& WXUNUSED(event));
	virtual void OnVC71Style(wxCommandEvent& WXUNUSED(event));
	virtual void OnFF2Style(wxCommandEvent& WXUNUSED(event));
	virtual void OnVC8Style(wxCommandEvent& WXUNUSED(event));
	virtual void OnDefaultStyle(wxCommandEvent& WXUNUSED(event));
	virtual void OnFancyStyle(wxCommandEvent& WXUNUSED(event));
	virtual void OnSelectColor(wxCommandEvent& event);
	virtual void OnSetPageImageIndex(wxCommandEvent& event);
	virtual void OnStyle(wxCommandEvent& event);
	virtual void OnDrawTabX(wxCommandEvent& event);
	virtual void OnGradientBack(wxCommandEvent& event);
	virtual void OnColorfullTabs(wxCommandEvent& event);
	virtual void OnSmartTabs(wxCommandEvent& event);
	virtual void OnDropDownArrow(wxCommandEvent& event);
	virtual void OnDropDownArrowUI(wxUpdateUIEvent& event);
	virtual void OnHideNavigationButtonsUI(wxUpdateUIEvent& event);

	virtual void OnEnableTab(wxCommandEvent& event);
	virtual void OnDisableTab(wxCommandEvent& event);
	virtual void OnEnableDrag(wxCommandEvent& event);
	virtual void OnDClickCloseTab(wxCommandEvent& event);
	virtual void OnAllowForeignDnd(wxCommandEvent& event);
	virtual void OnAllowForeignDndUI(wxUpdateUIEvent& event);

	virtual void OnPageChanging(wxFlatNotebookEvent& event);
	virtual void OnPageChanged(wxFlatNotebookEvent& event);
	virtual void OnPageClosing(wxFlatNotebookEvent& event);
	virtual void OnSetAllPagesShapeAngle(wxCommandEvent& WXUNUSED(event));

private:
	long GetTabIndexFromUser(const wxString& title, const wxString& prompt) const;

	void DeleteNotebookPage(long tabIdx);
	bool InsertNotebookPage(long tabIdx);
	bool m_bShowImages;
	bool m_bVCStyle;

	DECLARE_EVENT_TABLE()
};

#endif // ! CONTROLPANEL_H