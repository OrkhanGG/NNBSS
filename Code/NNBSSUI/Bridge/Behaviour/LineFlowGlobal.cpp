///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#include "LineFlowGlobal.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

CNNBSSLineFlow::CNNBSSLineFlow()
{
}

CNNBSSLineFlow::~CNNBSSLineFlow()
{
}

void CNNBSSLineFlow::OnStart()
{
	wxImage::AddHandler(new wxPNGHandler);

	NNBSSLocalization::CNNBSSLocalizationManagerAddressHandle()->OnInit();

	wxString currentProfileName = CNNBSSAuthorizationAddressHandle()->_GetCurrentProfileName();
	// If remember checkbox was not checked
	if (!CNNBSSAuthorizationAddressHandle()->_Remember())
	{
		CNNBSSLogIn* p_CNNBSSLogIn = CNNBSSLogInAddressHandle();
		p_CNNBSSLogIn->_Create(NULL, wxID_ANY, wxT("LogIn"), wxDefaultPosition, wxSize(1280, 720));
		p_CNNBSSLogIn->_SetupLogInInterface();
		if (p_CNNBSSLogIn->ShowModal() != wxID_OK)
		{
			if (p_CNNBSSLogIn->GetChildren().GetCount() > 0)
				p_CNNBSSLogIn->DestroyChildren();
			p_CNNBSSLogIn->Destroy();

			return;
		}
		else
		{
			if (p_CNNBSSLogIn->GetChildren().GetCount() > 0)
				p_CNNBSSLogIn->DestroyChildren();
			p_CNNBSSLogIn->Destroy();
		}
	}// ! REMEMBER

	// UX Frame
	CNNBSSUXAddressHandle()->_Create(NULL, wxID_ANY, wxT("NNBSS"), wxDefaultPosition, wxSize(1280, 720)
		, wxSYSTEM_MENU  |
		  wxRESIZE_BORDER|		 
		  wxCLIP_CHILDREN);
	CNNBSSUXAddressHandle()->SetMinSize(wxSize(1124,768));
	CNNBSSUXAddressHandle()->Show();
	CNNBSSUXAddressHandle()->Maximize();// maximize on start

	CNNBSSAuiManagerAddressHandle()->SetManagedWindow(CNNBSSUXAddressHandle()->GetUXManagedWindow());
	CNNBSSAuiManagerAddressHandle()->_SetupUXWindowInterface();

	// Add services page on start
	CNNBSSControlPanelAddressHandle()->OnAddPage(wxCommandEvent());
}