// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "Profiles.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// wxWidgets
#include <wx/wrapsizer.h>
#include <wx/simplebook.h>

#define NNBSS_EVT_FACEIDBUTTON_LOCAL 1256
#define NNBSS_EVT_MANAGEFACEIDS_LOCAL 1257
#define NNBSS_EVT_BACKBUTTON_LOCAL 1258

CNNBSSProfilesInterface::CNNBSSProfilesInterface(wxWindow* parent)
{

	Create(parent);

	_SetupInterface();
}

CNNBSSProfilesInterface::~CNNBSSProfilesInterface()
{

}

void CNNBSSProfilesInterface::_SetupInterface()
{
	// Create Page Container
	m_P.p_PageContainer = new wxSimplebook(this, wxID_ANY);

	// Create && Add Pages
	for (int id = 0; id < 3; id++)
	{
		m_P.p_Pages[id] = new wxPanel(m_P.p_PageContainer, wxID_ANY);

		m_P.p_PageContainer->AddPage(m_P.p_Pages[id], wxString::Format("Page%i", id), false);
	}
	CreatePageContent(0);

	// Create Sizer && Fit Page Container
	wxSizer* p_PageContainerSizer = new wxBoxSizer(wxVERTICAL);
	p_PageContainerSizer->Add(m_P.p_PageContainer, wxSizerFlags().Proportion(1).Expand());
	this->SetSizer(p_PageContainerSizer);
}

void CNNBSSProfilesInterface::OnAnyButtonClick(wxMouseEvent& evt)
{
	CNNBSSMouseEventsAddressHandle()->OnAnyButtonClick(evt);

	switch (evt.GetId())
	{
	case NNBSS_EVT_BACKBUTTON_LOCAL:
		CreatePageContent(0);
		m_P.p_PageContainer->ChangeSelection(0);
		m_P.currentPageID = 0;
		break;
	case NNBSS_EVT_FACEIDBUTTON_LOCAL:
		CreatePageContent(1);
		m_P.p_PageContainer->ChangeSelection(1);
		m_P.currentPageID = 1;
		break;
	case NNBSS_EVT_MANAGEFACEIDS_LOCAL:
		CreatePageContent(2);
		m_P.p_PageContainer->ChangeSelection(2);
		m_P.currentPageID = 2;
		break;
	default:
		break;
	}

	evt.Skip();
}

void CNNBSSProfilesInterface::OnAnyThumbnailClick(wxMouseEvent& evt)
{
	wxPanel* currentPanel = m_P.p_Pages[m_P.currentPageID];

	m_FIDA.currentBitmapPanel = dynamic_cast<CNNBSSBitmapPanel*>(evt.GetEventObject());

	wxWindowList m_CurPanelChildren = currentPanel->GetChildren();

	if (currentPanel->GetSizer())
		currentPanel->GetSizer()->Clear();

	for (const auto& p : m_CurPanelChildren)
	{
		if (p != m_FIDA.currentBitmapPanel)
		{
			p->Destroy();
		}
	}

	wxBoxSizer* p_NewSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* p_PageLabel = new wxStaticText(currentPanel, wxID_ANY, _("Position your head in the frame"));
	p_PageLabel->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
	wxFont TitleFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	p_PageLabel->SetFont(TitleFont);

	p_NewSizer->Add(p_PageLabel,wxSizerFlags().Proportion(0).CenterHorizontal());
	p_NewSizer->Add(m_FIDA.currentBitmapPanel, wxSizerFlags().Proportion(1).Expand());
	currentPanel->SetSizerAndFit(p_NewSizer);

	evt.Skip();

	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSProfilesInterface::CreatePageContent(const int pageID)
{
	wxPanel* currentPanel = m_P.p_Pages[pageID];

	switch (pageID)
	{
	case 0:
	{
		wxStaticText* p_PageLabel = new wxStaticText(currentPanel, wxID_ANY, _("You can either add or edit profiles."));
		wxStaticText* p_PageLabelDesc = new wxStaticText(currentPanel, wxID_ANY, _("Adding a profile will require an active device."));
		p_PageLabel->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		p_PageLabelDesc->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		wxFont TitleFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
		wxFont DescFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
		p_PageLabel->SetFont(TitleFont);
		p_PageLabelDesc->SetFont(DescFont);

		wxButton * p_AddProfile = new wxButton(currentPanel, NNBSS_EVT_FACEIDBUTTON_LOCAL, _("Add FaceID"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		wxButton* p_ManageProfiles = new wxButton(currentPanel, NNBSS_EVT_MANAGEFACEIDS_LOCAL, _("Manage FaceIDs"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		p_AddProfile->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_AddProfile->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		p_AddProfile->SetBitmap(NNBSS_PNG2BMP(NNBSSART_PROFILES_FACEID_IDLE), wxTOP);
		p_AddProfile->SetBitmapHover(NNBSS_PNG2BMP(NNBSSART_PROFILES_FACEID_HOVER));
		p_ManageProfiles->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_ManageProfiles->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		p_ManageProfiles->SetBitmap(NNBSS_PNG2BMP(NNBSSART_PROFILES_MANAGEROLES_IDLE), wxTOP);
		p_ManageProfiles->SetBitmapHover(NNBSS_PNG2BMP(NNBSSART_PROFILES_MANAGEROLES_HOVER));

		wxSizer* p_PageSizer = new wxBoxSizer(wxHORIZONTAL);
		p_PageSizer->Add(p_AddProfile, wxSizerFlags().CenterVertical());
		p_PageSizer->AddSpacer(FromDIP(70));
		p_PageSizer->Add(p_ManageProfiles, wxSizerFlags().CenterVertical());


		wxBoxSizer* p_PageGeneralSizer = new wxBoxSizer(wxVERTICAL);
		p_PageGeneralSizer->AddStretchSpacer();
		p_PageGeneralSizer->Add(p_PageLabel, wxSizerFlags().CenterHorizontal());
		p_PageGeneralSizer->AddSpacer(FromDIP(15));
		p_PageGeneralSizer->Add(p_PageLabelDesc, wxSizerFlags().CenterHorizontal());
		p_PageGeneralSizer->AddSpacer(FromDIP(70));
		p_PageGeneralSizer->Add(p_PageSizer, wxSizerFlags().CenterHorizontal());
		p_PageGeneralSizer->AddStretchSpacer();

		currentPanel->SetSizer(p_PageGeneralSizer);

		p_AddProfile->Bind(wxEVT_LEFT_UP, &CNNBSSProfilesInterface::OnAnyButtonClick, this);
		p_ManageProfiles->Bind(wxEVT_LEFT_UP, &CNNBSSProfilesInterface::OnAnyButtonClick, this);
		break;
	}
	case 1:
	{
		wxStaticText* p_PageLabel = new wxStaticText(currentPanel,wxID_ANY, _("Select a camera to create a FaceID"));
		p_PageLabel->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		wxFont TitleFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
		p_PageLabel->SetFont(TitleFont);
		wxBoxSizer* p_PageLabelSizer = new wxBoxSizer(wxHORIZONTAL);
		p_PageLabelSizer->AddStretchSpacer();
		p_PageLabelSizer->Add(p_PageLabel, wxSizerFlags().CenterVertical());
		p_PageLabelSizer->AddSpacer(FromDIP(20));
		wxButton* p_BackButton = new wxButton(currentPanel, NNBSS_EVT_BACKBUTTON_LOCAL, _("Back"),wxDefaultPosition,wxDefaultSize, wxBORDER_NONE);
		p_BackButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_BackButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE140);
		p_BackButton->Bind(wxEVT_LEFT_UP, &CNNBSSProfilesInterface::OnAnyButtonClick, this);
		p_PageLabelSizer->Add(p_BackButton, wxSizerFlags().CenterVertical());
		p_PageLabelSizer->AddStretchSpacer();

		int ActiveCameraCount = 0;
		for (const auto& p : SCNNBSSMVMParametersList)
			if (p->m_MVMConnectionStatus == NNBSS_DEVICE_CONNECTION_STATUS_STRING_ONLINE)
				ActiveCameraCount++;
		if (ActiveCameraCount <= 0)
		{
			p_PageLabel->SetLabel("No camera");
			currentPanel->SetSizer(p_PageLabelSizer);
			return;
		}
		wxWrapSizer* p_WrapSizer = new wxWrapSizer(wxHORIZONTAL);
		for (int c = 0; c < ActiveCameraCount; c++)
		{
			m_FIDA.p_BitmapPanels.emplace_back(new CNNBSSBitmapPanel(currentPanel));

			//SCNNBSSMVMParametersList[c]->m_MVMThread->AppendThumbnail(props);
		}
		
		for (const auto& p : m_FIDA.p_BitmapPanels)
		{
			p_WrapSizer->Add(p, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
		}

		wxSizer* p_PageMainSizer = new wxBoxSizer(wxVERTICAL);
		p_PageMainSizer->AddStretchSpacer();
		p_PageMainSizer->Add(p_PageLabelSizer, wxSizerFlags().CenterHorizontal());
		p_PageMainSizer->AddSpacer(FromDIP(20));
		p_PageMainSizer->Add(p_WrapSizer, wxSizerFlags().Proportion(0).CenterHorizontal());
		p_PageMainSizer->AddStretchSpacer();
		currentPanel->SetSizer(p_PageMainSizer); 
	}
		break;
	}
}