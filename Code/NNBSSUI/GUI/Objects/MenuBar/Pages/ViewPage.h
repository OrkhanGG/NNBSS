#ifndef VIEWPAGE_H
#define VIEWPAGE_H

// wxWidgets
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

class CNNBSSAppearancePanel;

class CNNBSSViewPage : public wxDialog
{
public:
	CNNBSSViewPage() {};
	~CNNBSSViewPage() {};

	void _Create(wxWindow* parent = NULL, wxWindowID id = wxID_ANY,
		const wxString& title = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0L,
		const wxString& name = wxT("DefaultGridViewEditDialog"))

	{
		Create(parent, id, title, pos, size, style, name);

		_SetupInterface();

		Center();
	};

	void _SetupInterface();

	void _SetupDesign();

private:
	void _BindEvents(bool bind = true);

protected:
	bool CheckDialogDemandMeet();

	void OnAnyButtonClick(wxMouseEvent& event);

private:
	wxFlatNotebook* p_ViewOptions = nullptr;
	wxFlatNotebookImageList* p_ViewOptionsImageList = nullptr;

	wxBoxSizer* p_MainSizer = nullptr;
	wxBoxSizer* p_NotebookSizer = nullptr;
	wxBoxSizer* p_SubButtonAreaSizer = nullptr;
	wxBoxSizer* p_SubButtonAreaVerticalSizer = nullptr;
	wxPanel* p_SubButtonArea = nullptr;
	wxButton* p_AcceptButton = nullptr;
	wxButton* p_DeclineButton = nullptr;
	wxButton* p_ResetButton = nullptr;

protected:
	CNNBSSAppearancePanel* p_AppearancePanel = nullptr;

	// LayoutPanel specials
	wxScrolledWindow* _LayoutPanel();
	wxScrolledWindow* p_LP = nullptr;
};

// Pages

class CNNBSSAppearancePanel : public wxScrolledWindow
{
public:
	CNNBSSAppearancePanel(wxWindow* parent, wxWindowID winid = -1, wxPoint pos = wxDefaultPosition,
		wxSize size = wxDefaultSize, long style = 0L, wxString name = wxT("DAppearancePanelName"));
	~CNNBSSAppearancePanel() {};

private:
	wxScrolledCanvas* p_AP = nullptr;
	wxBoxSizer* p_AP_MainSizer = nullptr;
	wxPropertyGrid* p_PropGrid = nullptr;

	void OnPropertyGridChanged(wxPropertyGridEvent& event);
};

#endif // ! VIEWPAGE_H