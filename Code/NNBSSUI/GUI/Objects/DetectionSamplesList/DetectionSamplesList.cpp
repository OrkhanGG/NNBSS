///////////////////////////////////////////////////////////////////////////////
// Name:        DetectionSamplesList.cpp
// Purpose:     Displays and let the user add or remove detection models
// Author:      Orkhan Aliyev
// Created:     2022-01-29 5:58AM
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif
#include <wx/utils.h>
#include <wx/wupdlock.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/textdlg.h>

#include "DetectionSamplesList.h"
// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS UI
#include <NNBSSUI/GUI/Objects/Controls/Button/Button.h>

CNNBSSDetectionSamplesList::CNNBSSDetectionSamplesList(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	// Main Sizer
	p_MainSizer = new wxBoxSizer(wxVERTICAL);
	// GridView GridSizer
	p_GridSizer = new wxGridSizer(_GetGridHeight(), _GetGridWidth(), 0, 0);

	p_MainSizer->Add(p_GridSizer, wxSizerFlags().Proportion(1).Expand());

	SetSizer(p_MainSizer);

	_SetupDesign();

	ChangeGridSize(wxSize(_GetGridWidth(), _GetGridHeight()));

	UpdateList();
}

CNNBSSDetectionSamplesList::~CNNBSSDetectionSamplesList()
{
}

// Commands
void CNNBSSDetectionSamplesList::_SetupDesign()
{
	SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
}

int CNNBSSDetectionSamplesList::_GetGridWidth()
{
	return _GridWidth;
}

int CNNBSSDetectionSamplesList::_GetGridHeight()
{
	return _GridHeight;
}

void CNNBSSDetectionSamplesList::_SetGridWidth(int width)
{
	_GridWidth = width;
}

void CNNBSSDetectionSamplesList::_SetGridHeight(int height)
{
	_GridHeight = height;
}

void CNNBSSDetectionSamplesList::_SetBalancedGridScale(int balancedScale)
{
	_GridWidth = balancedScale;
	_GridHeight = balancedScale;
}

bool CNNBSSDetectionSamplesList::ChangeGridSize(wxSize size)
{
	{
		// min value
		if (size.x < 2 || size.y < 2)
			return false;
	}

	for (size_t c = m_ListElements.size();
		c < (size_t)(size.x * size.y);
		c++)
	{
		SListElements	      element;
		// Create
		element.p_Button = new CNNBSSControlButton(this, wxID_ANY, _ButtonMainLabel,
			wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
		element.p_Button->SetButtonHelpLabel(_ButtonNote);
		// Set stance
		element.IsEmpty = false;
		element.Works = false;
		// Set position on grid
		element.Position = c;
		// Set colours
		element.p_Button->SetBackgroundColour(RGB_COLOURDEF_TRIPLE25);
		element.p_Button->SetForegroundColour(RGB_COLOURDEF_WHITE);
		// Bind
		element.p_Button->Bind(wxEVT_LEFT_UP, &CNNBSSDetectionSamplesList::OnAnyButtonClick, this);
		// Add
		m_ListElements.emplace_back(element);

	}

	// Unload sizer
	p_GridSizer->Clear();
	{
		// Set size variables
		_SetGridWidth(size.x);
		_SetGridHeight(size.y);
		// Commit changes
		p_GridSizer->SetCols(_GetGridWidth());
		p_GridSizer->SetRows(_GetGridHeight());
	}
	// Hide all buttons
	for (const auto& p : m_ListElements)
	{
		p.p_Button->Hide();
	}

	int counter = 0;
	for (const auto& p : m_ListElements)
	{
		counter++;
		// Attach buttons to related sizer
		p.p_Button->Show();
		p_GridSizer->Add(p.p_Button, wxSizerFlags().Proportion(1).Expand().Border());

		// if about to exceed the size limit, break
		if (counter > (size.x * size.y) - 1)
			break;
	}

	// Commit changes
	Refresh(); Update(); Layout();
	p_GridSizer->Layout();
	CNNBSSAuiManagerAddressHandle()->Update();

	return true;
}

// Returns true if given parameters match any of CNNBSSDetectionModelDataList member's parameters
bool CNNBSSDetectionSamplesList::IfExists(const wxString& cascadeName)
{
	if (!NNBSSDetectionModel::CNNBSSDetectionModelDataList.empty())
	{
		for (auto& p : NNBSSDetectionModel::CNNBSSDetectionModelDataList)
		{
			if (p.FileName == cascadeName)
			{
				return true;
			}
		}
	}

	return false;
}

bool CNNBSSDetectionSamplesList::Add(const wxString& path, const wxString& cascadeName, const int& position)
{
	{
		// sanity check
		if (path.empty() || cascadeName.empty())
		{
			return false;
		}
	}

	NNBSSDetectionModel::SNNBSSCascadeClassifierCacheData data;
	data.Path = path;
	data.FileName = cascadeName;
	data.DisplayName = cascadeName;
	data.Label = wxEmptyString;
	data.Works = true;
	data.Position = position;

	NNBSSDetectionModel::CNNBSSDetectionModelDataList.emplace_back(data);

	UpdateList();

	NNBSSInfoShow("Cascade Classifier successfully added", 100);

	return true;
}

void CNNBSSDetectionSamplesList::UpdateList()
{
	// Set to default all of the buttons
	for (auto& p : m_ListElements)
	{
		p.IsEmpty = true;
		p.Works = false;
		p.DisplayName = wxEmptyString;
		p.Label = wxEmptyString;
		p.FileName = wxEmptyString;
		p.FilePath = wxEmptyString;
		p.p_Button->SetButtonLabel(_ButtonMainLabel);
		p.p_Button->SetButtonHelpLabel(_ButtonNote);
		p.p_Button->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GLOBAL_ADD_64x64p_IDLE));
		p.p_Button->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GLOBAL_ADD_64x64p_HOVER), CNNBSSControlButton::ButtonStates::BS_HOVER);
		// Unbind events (if exist)
		p.p_Button->Unbind(wxEVT_CONTEXT_MENU, &CNNBSSDetectionSamplesList::OnContextMenu, this);
	}

	for (const auto& p : NNBSSDetectionModel::CNNBSSDetectionModelDataList)
	{
		SListElements				params;

		params.DisplayName = p.DisplayName;
		params.Label = p.Label;
		params.FileName = p.FileName;
		params.FilePath = p.Path;
		params.Works = p.Works;
		params.IsEmpty = false;

		// Take matching element to change and give back then
		params.p_Button = m_ListElements[p.Position].p_Button;
		params.p_Button->SetButtonLabel(params.DisplayName.MakeUpper().Remove(params.DisplayName.size() - 4, params.DisplayName.size()));
		params.p_Button->SetButtonHelpLabel(_("Click to edit"));
		// Bind Event
		params.p_Button->Bind(wxEVT_CONTEXT_MENU, &CNNBSSDetectionSamplesList::OnContextMenu, this);

		m_ListElements[p.Position] = params;
	}

	if ((int)NNBSSDetectionModel::CNNBSSDetectionModelDataList.size() >= _GetGridWidth() * _GetGridHeight())
	{
		ChangeGridSize(wxSize(_GetGridWidth() + 1, _GetGridHeight() + 1));
	}

	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSDetectionSamplesList::OnAnyButtonClick(wxMouseEvent& evt)
{
	CNNBSSControlButton* p_CLB = dynamic_cast<CNNBSSControlButton*>(evt.GetEventObject());
	int ButtonPosition = 0;
	bool empty = true;
	for (const auto& p : m_ListElements)
	{
		if (p.p_Button == p_CLB)
		{
			empty = p.IsEmpty;
			ButtonPosition = p.Position;
		}
	}

	if (empty)
	{
		wxFileDialog openFileDialog(this, _("Open Cascade XML file"),
			NNBSS_PROJECT_DIRECTORY + NNBSS_DETECTIONMODELS_DIRECTORY,// DefaultDirectory
			""/*Default File*/, "XML files (*.xml)|*.xml",// Only XMLs are allowed
			wxFD_OPEN | wxFD_FILE_MUST_EXIST);// Flags

		if (openFileDialog.ShowModal() == wxID_CANCEL)
		{
			evt.Skip();
			return; // if user changed idea
		}

		wxFileInputStream input_stream(openFileDialog.GetPath());
		if (!input_stream.IsOk())
		{
			evt.Skip();
			NNBSSErrorShow("Cannot open file!", 100);
			return;
		}
		else
		{
			// Check file size
			if (NNBSSFileSystem::NNBSS_GETFILESIZE(std::string(openFileDialog.GetPath().mb_str())) < 1)
			{
				evt.Skip();
				NNBSSErrorShow("The file is empty!", 100);
				return;
			}

			// If we already have it, don't let the same to be added again since instances are not allowed.
			if (IfExists(openFileDialog.GetFilename()))
			{
				evt.Skip();
				NNBSSErrorShow("The file already exists.", 100);
				return;
			}

			NNBSSBusyInfo("Verifying cascade... This won't take long.", true);

			// No need to write 'else' for this condition, CheckCascadeClassifier function does notification stuff
			if (CheckCascadeClassifier(std::string(openFileDialog.GetPath().mb_str())))
			{

				if (!NNBSSFileSystem::NNBSS_COPYPASTE(std::string(openFileDialog.GetPath().mb_str()), NNBSS_PROJECT_DIRECTORY + NNBSS_DETECTIONMODELS_DIRECTORY,
					std::string(openFileDialog.GetFilename().mb_str())))
				{
					NNBSSErrorShow("File could not be copied!", 100);
				}

				Add(openFileDialog.GetPath(), openFileDialog.GetFilename(), ButtonPosition);
			}

			NNBSSBusyInfo("", false);

		}
	}

	evt.Skip();
}

void CNNBSSDetectionSamplesList::OnContextMenu(wxContextMenuEvent& evt)
{
	CNNBSSControlButton* detectionModel = dynamic_cast<CNNBSSControlButton*>(evt.GetEventObject());

	if (!detectionModel)
		detectionModel = dynamic_cast<CNNBSSControlButton*>(wxFindWindowAtPoint(wxGetMousePosition()));

	wxCHECK_RET(detectionModel, "in CNNBSSDetectionSamplesList::OnContextMenu() could not determine the detection model panel");

	wxMenu* ContextMenu = new wxMenu(wxNO_BORDER);
	int    id = wxID_NONE;

	wxMenuItem* SelectMenuItem = new wxMenuItem(ContextMenu, CMEID_LABEL, _("Change label"), _("Change the label that'll be displayed on each detected object"));
	wxMenuItem* RemoveMenuItem = new wxMenuItem(ContextMenu, CMEID_REMOVE, _("Remove"), _("Remove Detection Model"));

	SelectMenuItem->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	SelectMenuItem->SetTextColour(RGB_COLOURDEF_WHITE);
	RemoveMenuItem->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	RemoveMenuItem->SetTextColour(RGB_COLOURDEF_WHITE);

	ContextMenu->Append(SelectMenuItem);
	ContextMenu->Append(RemoveMenuItem);

	// Get row number for function that requires
	int RowNumber = wxID_ANY;
	for (int c = 0; c < (int)m_ListElements.size(); c++)
	{
		if (m_ListElements[c].p_Button == detectionModel)
		{
			RowNumber = c;
		}
	}

	id = detectionModel->GetPopupMenuSelectionFromUser(*ContextMenu);
	switch (id)
	{
	case CMEID_LABEL:
		OnChangeLabel(RowNumber);
		break;
	case CMEID_REMOVE:
		RemoveDM(RowNumber);
		break;
	default:
		break;
	}

	evt.Skip();
}

void CNNBSSDetectionSamplesList::OnChangeLabel(const int& RowNumber)
{
	// Finding by file name is enough I think :)
	int CC_Index = -1;
	for (int c = 0; c < (int)NNBSSDetectionModel::CNNBSSDetectionModelDataList.size(); c++)
	{
		if (m_ListElements[RowNumber].FileName == NNBSSDetectionModel::CNNBSSDetectionModelDataList[c].FileName)
		{
			CC_Index = c;
			break;
		}
	}

	wxTextEntryDialog* p_LabelDialog = new wxTextEntryDialog(GetActiveUXFrame(), _("Change the label of this detection model"),
		_("Change Detection ModelLabel"), NNBSSDetectionModel::CNNBSSDetectionModelDataList[CC_Index].Label);

	if (p_LabelDialog->ShowModal() == wxID_OK)
	{
		// Try to assign gotten values
		for (int c = 0; c < (int)NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.size(); c++)
		{
			if (NNBSSDetectionModel::CNNBSSDetectionModelDataList[CC_Index].Path == NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[c]._AssignedModelPath)
			{
				NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[c]._Label = p_LabelDialog->GetValue();
				break;
			}
		}

		if (CC_Index != -1)
			NNBSSDetectionModel::CNNBSSDetectionModelDataList[CC_Index].Label = p_LabelDialog->GetValue();
	}

	p_LabelDialog->Destroy();

	// Commit UI Changes
	UpdateList();
}

void CNNBSSDetectionSamplesList::RemoveDM(const int& RowNumber)
{
	// Finding by file name is enough I think :)
	int CC_Index = -1;
	for (int c = 0; c < (int)NNBSSDetectionModel::CNNBSSDetectionModelDataList.size(); c++)
	{
		if (m_ListElements[RowNumber].FileName == NNBSSDetectionModel::CNNBSSDetectionModelDataList[c].FileName)
		{
			CC_Index = c;
			break;
		}
	}

	// Then, try to clear assigned detection model so no other open cameras may use that detection model anymore
	for (int c = 0; c < (int)NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.size(); c++)
	{
		if (NNBSSDetectionModel::CNNBSSDetectionModelDataList[CC_Index].Path == NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[c]._AssignedModelPath)
		{
			NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.erase(NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.begin() + c);
			break;
		}
	}

	if (CC_Index != -1)
		NNBSSDetectionModel::CNNBSSDetectionModelDataList.erase(NNBSSDetectionModel::CNNBSSDetectionModelDataList.begin() + CC_Index);

	// Commit UI Changes
	UpdateList();
}

void CNNBSSDetectionSamplesList::PropertiesDM(const int& RowNumber)
{

}
