#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

// NNBSS UI
#include <NNBSSUI/GUI/Objects/LogIn/LogIn.h>// Avoid including this

class CNNBSSAuthorization
{
public:
	CNNBSSAuthorization();
	~CNNBSSAuthorization();

public:
	bool _IsServer();
	bool _IsClient();
	bool _IsAdmin();

	bool _HaveValidProfile();
	bool _Remember();
	bool _SetRemember(wxString userName, bool rememberMe);
	
	void _SaveProfile(wxString profileName, wxString username, wxString password, bool remember = false);
	void _SetCurrentProfileName(wxString profileName);
	wxString _GetCurrentProfileName();

	bool _CheckLogInInfo(wxString username, wxString password);

	bool _LockScreen(bool light = true);
private:
	void RegisterProcess(std::int64_t& process);
	bool ReadPermissions();

protected:
	bool _Server = false;
	bool _Client = false;
	bool _Admin = false;
	bool _DoesCurrentProfileExist = false;
	bool _RememberMe = false;

	wxString _ProfileName;
private:
	friend class CNNBSSLogIn;
};

#endif // ! AUTHORIZATION_H