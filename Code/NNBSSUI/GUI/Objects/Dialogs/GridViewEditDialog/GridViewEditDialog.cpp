// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "GridViewEditDialog.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS UI
#include <NNBSSUI/GUI/Objects/Controls/Button/Button.h>

#define UNIQUE_ID 10000

void CNNBSSGridViewEditDialog::_SetupInterface()
{
	p_GeneralSizerH = new wxBoxSizer(wxHORIZONTAL);
	p_GeneralSizerV = new wxBoxSizer(wxVERTICAL);

	// Create objects
	p_Title = new wxStaticText(this, wxID_ANY, _("Edit Gridview"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);

	PanelGridSizer = new wxGridSizer(NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER, NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER,0,0);

	m_ExampleButtons = new CNNBSSControlButton * [NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER];

	for (int y = 0; y < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; y++)
	{
		for (int x = 0; x < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; x++)
		{
			m_ExampleButtons[y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x] = new CNNBSSControlButton(this, UNIQUE_ID + (y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x),
				wxEmptyString,wxDefaultPosition, wxSize(30,30));
			PanelGridSizer->Add(m_ExampleButtons[y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x], wxSizerFlags().Proportion(1).Expand().Border());
		}
	}

	p_GridSizeInfo = new wxStaticText(this, wxID_ANY, _("No value"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);

	p_AcceptButton = new CNNBSSControlButton(this, NNBSS_EVT_OK, _("Set"), wxDefaultPosition, wxSize(64,32), wxNO_BORDER);
	p_DeclineButton = new CNNBSSControlButton(this, NNBSS_EVT_CANCEL, _("Cancel"), wxDefaultPosition, wxSize(64, 32), wxNO_BORDER);
	p_DialogButtonsHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	p_DialogButtonsHorizontalSizer->Add(p_AcceptButton, wxSizerFlags().CenterVertical());
	p_DialogButtonsHorizontalSizer->AddSpacer(FromDIP(10));
	p_DialogButtonsHorizontalSizer->Add(p_DeclineButton, wxSizerFlags().CenterVertical());

	p_GeneralSizerV->AddSpacer(FromDIP(10));
	p_GeneralSizerV->Add(p_Title, wxSizerFlags().Proportion(0).CenterHorizontal());
	p_GeneralSizerV->AddSpacer(FromDIP(10));
	p_GeneralSizerV->Add(PanelGridSizer, wxSizerFlags().Proportion(0).CenterHorizontal());
	p_GeneralSizerV->AddSpacer(FromDIP(10));
	p_GeneralSizerV->Add(p_GridSizeInfo, wxSizerFlags().Proportion(0).CenterHorizontal());
	p_GeneralSizerV->AddSpacer(FromDIP(10));
	p_GeneralSizerV->Add(p_DialogButtonsHorizontalSizer, wxSizerFlags().Proportion(0).CenterHorizontal());


	p_GeneralSizerH->Add(p_GeneralSizerV, wxSizerFlags().Proportion(1).CenterVertical());
	SetSizer(p_GeneralSizerH);
	SetSize(p_GeneralSizerH->CalcMin() + wxSize(40, 40));

	_SetupDesign();

	_BindEvents(true);

	//select grid size

	switch (((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->GetCurrentContent())
	{
	case NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW:
	{
		_SelectExampleGridByPos(wxPoint(CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridWidth(),
		CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridHeight()));
	}
	break;
	case NNBSS_CONTENTPAGE_CURRENTCONTENT_PLAYBACK:
	{
		_SelectExampleGridByPos(wxPoint(CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridWidth(),
			CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridHeight()));
	}
	break;
	default:
		break;
	}

	// Set focus on cancel button
	p_DeclineButton->SetFocus();
}

void CNNBSSGridViewEditDialog::_SetupDesign()
{
	this->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	p_Title->SetForegroundColour(RGB_COLOURDEF_WHITE);

	wxFont TitleFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	p_Title->SetFont(TitleFont);

	for (int y = 0; y < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; y++)
	{
		for (int x = 0; x < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; x++)
		{
			m_ExampleButtons[y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x]->SetBackgroundColour(RGB_COLOURDEF_NNBSSBLUE);
		}
	}

	p_GridSizeInfo->SetForegroundColour(RGB_COLOURDEF_WHITE);

	p_AcceptButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE30);
	p_AcceptButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_AcceptButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_IDLE));
	p_AcceptButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
	p_AcceptButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGADD_DISABLED), CNNBSSControlButton::ButtonStates::BS_DISABLED);

	p_DeclineButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE30);
	p_DeclineButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_DeclineButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_IDLE));
	p_DeclineButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
}

void CNNBSSGridViewEditDialog::_BindEvents(bool bind)
{
	if (bind)
	{
		// Grid panels
		for (int y = 0; y < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; y++)
			for (int x = 0; x < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; x++)
				m_ExampleButtons[y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x]->Bind(wxEVT_LEFT_UP, &CNNBSSGridViewEditDialog::OnAnyPanelClick, this);

		// buttons
		p_AcceptButton->Bind(wxEVT_LEFT_UP, &CNNBSSGridViewEditDialog::OnAnyButtonClick, this);
		p_DeclineButton->Bind(wxEVT_LEFT_UP, &CNNBSSGridViewEditDialog::OnAnyButtonClick, this);
	}
	else
	{
		// Grid panels
		for (int y = 0; y < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; y++)
			for (int x = 0; x < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; x++)
				m_ExampleButtons[y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x]->Bind(wxEVT_LEFT_UP, &CNNBSSGridViewEditDialog::OnAnyPanelClick, this);

		// buttons
		p_AcceptButton->Unbind(wxEVT_LEFT_UP, &CNNBSSGridViewEditDialog::OnAnyButtonClick, this);
		p_DeclineButton->Unbind(wxEVT_LEFT_UP, &CNNBSSGridViewEditDialog::OnAnyButtonClick, this);
	}
}

bool CNNBSSGridViewEditDialog::CheckDialogDemandMeet()
{
	switch (((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->GetCurrentContent())
	{
	case NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW:
	{
		if (xSize == CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridWidth()
			&& ySize == CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridHeight())
		{
			NNBSSMessageBoxSimple(_("Error"), _("You have not changed the grid size!"), wxICON_ERROR);
			return false;
		}
	}
	break;
	case NNBSS_CONTENTPAGE_CURRENTCONTENT_PLAYBACK:
	{
		if (xSize == CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridWidth()
			&& ySize == CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridHeight())
		{
			NNBSSMessageBoxSimple(_("Error"), _("You have not changed the grid size!"), wxICON_ERROR);
			return false;
		}
	}
	break;
	default:
		break;
	}
	return true;
}

void CNNBSSGridViewEditDialog::OnAnyButtonClick(wxMouseEvent& event)
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

			// change gridview size
			switch (((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->GetCurrentContent())
			{
			case NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW:
			{
				CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->ChangeGridSize(wxSize(xSize, ySize));
			}
			break;
			case NNBSS_CONTENTPAGE_CURRENTCONTENT_PLAYBACK:
			{
				CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->ChangeGridSize(wxSize(xSize, ySize));
			}
			break;
			default:
				break;
			}
			

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
		CNNBSSGridViewEditDialogAddressHandle(true);
	}
}

void CNNBSSGridViewEditDialog::_SelectExampleGridByID(const int& id)
{
	int xPos = 0, yPos = 0;
	// Grid panels
	for (int y = 0; y < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; y++)
		for (int x = 0; x < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; x++)
		{
			if (m_ExampleButtons[y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x]->GetId() == id)
			{
				xPos = x + 1; yPos = y + 1;
			}
			m_ExampleButtons[y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x]->SetBackgroundColour(RGB_COLOURDEF_TRIPLE30);
		}

	for (int y = NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER - 1; y > -1; y--)
		for (int x = NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER - 1; x > -1; x--)
			if (x < xPos && y < yPos)
				m_ExampleButtons[y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x]->SetBackgroundColour(RGB_COLOURDEF_NNBSSBLUE);

	xSize = xPos; ySize = yPos;

	p_GridSizeInfo->SetLabel(wxString(std::string(std::to_string(xSize) + "x" + std::to_string(ySize)).c_str(), wxConvUTF8) +
		wxString::Format(" | %i %s", xSize * ySize, xSize * ySize < 2 ? "camera" : "cameras"));
	

	switch (((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->GetCurrentContent())
	{
	case NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW:
	{
		if (xSize == CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridWidth()
			&& ySize == CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridHeight())
		{
			p_AcceptButton->Enable(false);
		}
		else
		{
			p_AcceptButton->Enable();
		}
	}
	break;
	case NNBSS_CONTENTPAGE_CURRENTCONTENT_PLAYBACK:
	{
		if (xSize == CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridWidth()
			&& ySize == CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridHeight())
		{
			p_AcceptButton->Enable(false);
		}
		else
		{
			p_AcceptButton->Enable();
		}
	}
	break;
	default:
		break;
	}

	GetSizer()->Layout(); Layout();
	Refresh();
	Update();
}

void CNNBSSGridViewEditDialog::_SelectExampleGridByPos(const wxPoint& pos)
{
	int xPos = pos.x, yPos = pos.y;

	// Grid panels
	for (int y = 0; y < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; y++)
		for (int x = 0; x < NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER; x++)
		{
			m_ExampleButtons[y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x]->SetBackgroundColour(RGB_COLOURDEF_TRIPLE30);
		}

	for (int y = NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER - 1; y > -1; y--)
		for (int x = NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER - 1; x > -1; x--)
			if (x < xPos && y < yPos)
				m_ExampleButtons[y * NNBSS_GRIDVIEW_MAX_CAMERA_SQUARING_NUMBER + x]->SetBackgroundColour(RGB_COLOURDEF_NNBSSBLUE);

	xSize = xPos; ySize = yPos;

	p_GridSizeInfo->SetLabel(wxString(std::string(std::to_string(xSize)+"x" + std::to_string(ySize)).c_str(), wxConvUTF8)+ 
		wxString::Format(" | %i %s", xSize*ySize, xSize*ySize < 2 ? "camera" : "cameras"));

	switch (((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->GetCurrentContent())
	{
	case NNBSS_CONTENTPAGE_CURRENTCONTENT_GRIDVIEW:
	{
		if (xSize == CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridWidth()
			&& ySize == CNNBSSCameraGridAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridHeight())
		{
			p_AcceptButton->Enable(false);
		}
		else
		{
			p_AcceptButton->Enable();
		}
	}
	break;
	case NNBSS_CONTENTPAGE_CURRENTCONTENT_PLAYBACK:
	{
		if (xSize == CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridWidth()
			&& ySize == CNNBSSPlaybackPanelAddressHandle((CNNBSSContentPanel*)CNNBSSControlPanelAddressHandle()->GetCurrentPage())->_GetGridHeight())
		{
			p_AcceptButton->Enable(false);
		}
		else
		{
			p_AcceptButton->Enable();
		}
	}
	break;
	default:
		break;
	}

	GetSizer()->Layout(); Layout();
	Refresh();
	Update();
}

void CNNBSSGridViewEditDialog::OnAnyPanelClick(wxMouseEvent& event)
{
	_SelectExampleGridByID(event.GetId());
	
	event.Skip();

	Refresh();
	Update();
}
