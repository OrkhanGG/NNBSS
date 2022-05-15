///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef TITLEBAR_H
#define TITLEBAR_H

class CNNBSSControlButton;

class CNNBSS_SC_TitleBar : public wxPanel
{
public:
	CNNBSS_SC_TitleBar() {};
	~CNNBSS_SC_TitleBar();

	void _Create(wxWindow* parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = wxT("DefaultNameTitleBar"))
    {
        Create(parent, winid, pos, size, style, name);
        _SetupTitleBarInterface();
    }

    wxPanel* GetPanel()
    {
        return this;
    }

    void _SetupTitleBarInterface();
private:
    enum TitleBarLocal
    {
        TB_ID_QUIT = 0,
        TB_ID_RESTORE,
        TB_ID_MINIMIZE
    };

    CNNBSSControlButton* QuitButton = nullptr;
    CNNBSSControlButton* RestoreButton = nullptr;
    CNNBSSControlButton* MinimizeButton = nullptr;

    wxPoint m_MouseDelta = wxPoint(0, 0);
    void TitleBarOnLeftDown(wxMouseEvent& evt);
    void TitleBarOnLeftDClick(wxMouseEvent& evt);
    void TitleBarOnLeftUp(wxMouseEvent& evt);
    void OnAnyButtonClick(wxMouseEvent& evt);
    void TitleBarOnMouseMove(wxMouseEvent& evt);
};

#endif // ! TITLEBAR_H