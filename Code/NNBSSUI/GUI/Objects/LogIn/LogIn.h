///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#ifndef LOGIN_H
#define LOGIN_H

class wxPanel;
class wxStaticText;
class wxTextCtrl;
class wxCheckBox;
class wxButton;
class wxBoxSizer;
class wxBitmapButton;

class CNNBSSLogIn : public wxDialog
{
public:
    CNNBSSLogIn(){};
    ~CNNBSSLogIn();

    void _Create(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxNO_BORDER);

    void _SetupLogInInterface();

protected:
    void OnLogIn(wxMouseEvent& evt);
    void OnQuit(wxMouseEvent& evt);
private:
    bool CheckLogInDemands();

    wxPanel* panel = nullptr;
    wxBitmapButton* p_LOGO = nullptr;
    wxStaticText* m_usernameLabel = nullptr;
    wxStaticText* m_passwordLabel = nullptr;
    wxStaticText* m_informationLabel = nullptr;
    wxTextCtrl* m_usernameEntry = nullptr;
    wxTextCtrl* m_passwordEntry = nullptr;
    wxButton* m_buttonLogin =  nullptr;
    wxButton* m_buttonQuit = nullptr;
    
    // (No-profile found) special
    wxButton* p_WarningTitle = nullptr;
    wxButton* p_PasswordInfo = nullptr;
    wxCheckBox* p_RememberMe = nullptr;

    wxSizer* p_ElementsSizer = nullptr;
    wxSizer* p_ElementSizerHorizontal = nullptr;
    wxBoxSizer* VerticalSizer = nullptr;
    wxBoxSizer* HorizontalSizer = nullptr;
    wxBoxSizer* MainSizer = nullptr;
};

#endif // ! LOGIN_H