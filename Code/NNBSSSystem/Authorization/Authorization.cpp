// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "Authorization.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

CNNBSSAuthorization::CNNBSSAuthorization()
{
}

CNNBSSAuthorization::~CNNBSSAuthorization()
{
}

bool CNNBSSAuthorization::_IsServer()
{
	return _Server;
}

bool CNNBSSAuthorization::_IsClient()
{
	// Returns whether the request comes from a client or the server
	return _Client;
}

bool CNNBSSAuthorization::_IsAdmin()
{
	return _Admin;
}

bool CNNBSSAuthorization::_HaveValidProfile()
{
	_DoesCurrentProfileExist = false;

	try
	{
		TiXmlDocument document(NNBSS_DIR__PROFILES_XML_PATH);
		if (!document.LoadFile())
		{
			_DoesCurrentProfileExist = false;
		}
		else
		{
			TiXmlElement* p_Root = document.FirstChildElement("Profiles");
			// if there's a root node lets parse the file
			if (p_Root != 0)
			{
				if (p_Root->FirstChildElement("Profile") != 0)
				{
					_DoesCurrentProfileExist = true;
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		NNBSSMessageBoxSimple(_("Error"), e.what(), wxICON_ERROR);


		_DoesCurrentProfileExist = false;
	}

	return _DoesCurrentProfileExist;
}

bool CNNBSSAuthorization::_Remember()
{
	bool found = false;
	int foundCount = 0;
	wxString rememberedProfileName;
	try
	{
		TiXmlDocument document(NNBSS_DIR__PROFILES_XML_PATH);
		if (!document.LoadFile())
		{
			found = false;
		}
		else
		{
			TiXmlElement* p_Root = document.FirstChildElement("Profiles");
			// if there's a root node lets parse the file
			if (p_Root != 0)
			{
				for (TiXmlElement* e = p_Root->FirstChildElement("Profile");
					e != NULL; e = e->NextSiblingElement("Profile"))
				{
					std::string remember = e->Attribute("Remember");

					if (remember == "true")
					{
						found = true;

						foundCount++;

						std::string profileName = e->Attribute("ProfileName");
						rememberedProfileName = wxString(profileName.c_str(), wxConvUTF8);
					}

				}
#if 0
				// only one profile may be remembered, so, if there are multiple remembered profiles then turn them off
				if (foundCount > 1)
				{
					for (TiXmlElement* e = p_Root->FirstChildElement("Profile");
						e != NULL; e = e->NextSiblingElement("Profile"))
					{
						wxLogMessage("cond:%s cp:%s rp:%s", e->Attribute("ProfileName") == std::string(rememberedProfileName.mb_str())
						? "true" : "false", e->Attribute("ProfileName"), std::string(rememberedProfileName.mb_str()));

						if (e->Attribute("ProfileName") == std::string(rememberedProfileName.mb_str()))
						{
							e->SetAttribute("Remember","false");
						}

					}
					// Save changes
					document.SaveFile();
				}
#endif // TODO, FIX THIS LATER

			}
		}
	}
	catch (const std::exception& e)
	{
		NNBSSMessageBoxSimple(_("Error"), e.what(), wxICON_ERROR);
		_RememberMe = false;
		return _RememberMe;
	}

	_RememberMe = found;

	if (_RememberMe)
	{
		_SetCurrentProfileName(rememberedProfileName);
	}

	return _RememberMe;
}

bool CNNBSSAuthorization::_SetRemember(wxString userName, bool rememberMe)
{
	bool found = false;
	try
	{
		TiXmlDocument document(NNBSS_DIR__PROFILES_XML_PATH);
		if (!document.LoadFile())
		{
			found = false;
		}
		else
		{
			TiXmlElement* p_Root = document.FirstChildElement("Profiles");
			// if there's a root node lets parse the file
			if (p_Root != 0)
			{
				for (TiXmlElement* e = p_Root->FirstChildElement("Profile");
					e != NULL; e = e->NextSiblingElement("Profile"))
				{
					std::string _userName = e->Attribute("UserName");

					if (_userName == std::string(userName.mb_str()))
					{
						e->SetAttribute("Remember", rememberMe ? "true" : false);
						document.SaveFile();
					}

				}

			}
		}
	}
	catch (const std::exception& e)
	{
		NNBSSMessageBoxSimple(_("Error"), e.what(), wxICON_ERROR);
		_RememberMe = false;
		return _RememberMe;
	}


	_RememberMe = rememberMe;
	return _RememberMe;
}

void CNNBSSAuthorization::_SaveProfile(wxString profileName, wxString username, wxString password, bool remember)
{
	TiXmlDocument document(NNBSS_DIR__PROFILES_XML_PATH);
	if (!document.LoadFile())
	{
		document.SaveFile();
	}
	else
	{
		document.Clear();// obviously this is not the correct approach since it must not be reset when it should be saved
	}

	TiXmlDeclaration* p_HeaderNode = new TiXmlDeclaration("1.0", "utf-8", "");

	// Add header node
	document.LinkEndChild(p_HeaderNode);

	TiXmlElement* p_Root = new TiXmlElement("Profiles");

	std::string s_profileName = std::string(profileName.mb_str());
	std::string s_userName = std::string(username.mb_str());
	std::string s_passwordName = std::string(password.mb_str());

	TiXmlElement* p_ProfileElement = new TiXmlElement("Profile");
	p_ProfileElement->SetAttribute("ProfileName", s_profileName.c_str());
	p_ProfileElement->SetAttribute("UserName", s_userName.c_str());
	p_ProfileElement->SetAttribute("Password", s_passwordName.c_str());
	p_ProfileElement->SetAttribute("Remember", remember ? "true" : "false");

	p_Root->LinkEndChild(p_ProfileElement);
	// Then add the root into the file too.
	document.LinkEndChild(p_Root);

	document.SaveFile();
}

void CNNBSSAuthorization::_SetCurrentProfileName(wxString profileName)
{
	_ProfileName = profileName;
}

wxString CNNBSSAuthorization::_GetCurrentProfileName()
{
	// Make corrections. This is not acceptable way to get profile name as it carries heavy authorizations
	if (_ProfileName.empty())
	{
		_SetCurrentProfileName(wxT("NULL"));
	}

	return _ProfileName;
}

bool CNNBSSAuthorization::_CheckLogInInfo(wxString username, wxString password)
{
	bool found = false;
	try
	{
		TiXmlDocument document(NNBSS_DIR__PROFILES_XML_PATH);
		if (!document.LoadFile())
		{
			NNBSSMessageBoxSimple(_("Error"), _("System files couldn't be loaded!"), wxICON_ERROR);
		}
		else
		{
			TiXmlElement* p_Root = document.FirstChildElement("Profiles");
			// if there's a root node lets parse the file
			if (p_Root != 0)
			{
				for (TiXmlElement* e = p_Root->FirstChildElement("Profile");
					e != NULL; e = e->NextSiblingElement("Profile"))
				{
					std::string registeredUserName = e->Attribute("UserName");
					std::string registeredPassword = e->Attribute("Password");

					if (std::string(username.mb_str()) == registeredUserName
						&& std::string(password.mb_str()) == registeredPassword)
						found = true;
				}

				// after checking all, if not found:
				if (!found)
				{
					_SetCurrentProfileName("NULL");
					return false;
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		NNBSSMessageBoxSimple(_("Error"), e.what(), wxICON_ERROR);
		return false;
	}

	// if nothing happened till here, then set the profile name
	_SetCurrentProfileName(username);

	return true;
}

bool CNNBSSAuthorization::_LockScreen(bool light)
{
	if (light)
	{
		// light means just locking the app
		return true;
	}
	else
	{
		// locking the entire computer as it may not be able to get any inputs from the user
		return true;
	}

	return false;
}

void CNNBSSAuthorization::RegisterProcess(std::int64_t& process)
{
}

bool CNNBSSAuthorization::ReadPermissions()
{
	// Can I do this process?
	return true;
}
