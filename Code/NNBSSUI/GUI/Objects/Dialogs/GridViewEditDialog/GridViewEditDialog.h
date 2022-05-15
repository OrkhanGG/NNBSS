#ifndef GRIDVIEWEDITDIALOG_H
#define GRIDVIEWEDITDIALOG_H

class CNNBSSControlButton;

class CNNBSSGridViewEditDialog : public wxDialog
{
public:
	CNNBSSGridViewEditDialog() {};
	~CNNBSSGridViewEditDialog() {};

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

	bool CheckDialogDemandMeet();

	void OnAnyButtonClick(wxMouseEvent& event);

	void _SelectExampleGridByID(const int& id);
	void _SelectExampleGridByPos(const wxPoint& pos);

	void OnAnyPanelClick(wxMouseEvent& event);

	wxBoxSizer* p_GeneralSizerH = nullptr;
	wxBoxSizer* p_GeneralSizerV = nullptr;
	wxStaticText* p_Title = nullptr;
	CNNBSSControlButton** m_ExampleButtons = nullptr;
	wxGridSizer* PanelGridSizer = nullptr;
	wxBoxSizer* p_DialogButtonsHorizontalSizer = nullptr;
	wxStaticText* p_GridSizeInfo = nullptr;
	CNNBSSControlButton* p_AcceptButton = nullptr;
	CNNBSSControlButton* p_DeclineButton = nullptr;

	int xSize = 0, ySize = 0;
};

#endif // ! GRIDVIEWEDITDIALOG_H