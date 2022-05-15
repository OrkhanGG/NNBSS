// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/slider.h>// we dont need this
#include <wx/combobox.h>
#include <wx/textdlg.h>
#endif
#include <wx/combobox.h>
#include <wx/listbox.h>
#include <wx/richtooltip.h>

#include "DeviceProperties.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS UI
#include <NNBSSUI/GUI/Objects/Controls/Button/Button.h>
#include <NNBSSUI/GUI/Objects/Controls/CheckBox/CheckBox.h>
#include <NNBSSUI/GUI/Objects/Controls/DraggableHeader/DraggableHeader.h>
#include <NNBSSUI/GUI/Objects/Controls/Slider/Slider.h>
#include <NNBSSUI/GUI/Objects/Controls/StaticBox/StaticBox.h>
#include <NNBSSUI/GUI/Objects/Controls/Knob/Knob.h>
#include <NNBSSUI/GUI/Objects/wxFlatNotebook/headers/wxFlatNotebook.h>
// Boost
#include <boost/regex.hpp>

#define NNBSS_EVT_SET_NOTIFICATION_MESSAGE_BUTTON 12000
#define NNBSS_EVT_SET_NOTIFICATION_ADDITIONAL_NOTE_BUTTON 12001


CNNBSSDevicePropertiesDialog::CNNBSSDevicePropertiesDialog(SNNBSSMVMData* DeviceParams,
	wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: m_DeviceParams(DeviceParams)
{
	Create(parent, id, title, pos, size, style, name);

	_SetupInterface();

	Center();
};

void CNNBSSDevicePropertiesDialog::_SetupInterface()
{
	_SetupDesign();

	wxInitAllImageHandlers();

	// Create
	p_MainSizer = new wxBoxSizer(wxVERTICAL);

	p_DevicePropertiesNotebookSizer = new wxBoxSizer(wxVERTICAL);

	CNNBSSControlDraggableHeader* p_DraggableHeader = new CNNBSSControlDraggableHeader(this, wxID_ANY, _("MVM Properties")
	,wxDefaultPosition, wxSize(1,32));
	p_DraggableHeader->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	p_SubButtonAreaSizer = new wxBoxSizer(wxHORIZONTAL);
	p_SubButtonAreaVerticalSizer = new wxBoxSizer(wxVERTICAL);
	p_SubButtonArea = new wxPanel(this, wxID_ANY);

	p_CloseButton = new CNNBSSControlButton(p_SubButtonArea, NNBSS_EVT_CANCEL, _("Close"), wxDefaultPosition, wxSize(75, 32), wxNO_BORDER);
	p_ResetButton = new CNNBSSControlButton(p_SubButtonArea, NNBSS_EVT_RESET, _("Reset"), wxDefaultPosition, wxSize(75, 32), wxNO_BORDER);

	p_DevicePropertiesNotebook = new wxFlatNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	p_DevicePropertiesNotebookIcons = new wxFlatNotebookImageList();

	// Add pages with icons
	p_DevicePropertiesNotebookIcons->Add(NNBSS_PNG2BMP(NNBSSART_GRIDVIEW_2x2_IDLE));
	p_DevicePropertiesNotebook->SetImageList(p_DevicePropertiesNotebookIcons);

	{
		// Object Detection Starts

		p_ObjectDetectionPanel = new wxPanel(p_DevicePropertiesNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
		wxBoxSizer* p_ObjectDetectionPanelSizer = new wxBoxSizer(wxVERTICAL);

		// Elements of object detection
		wxStaticText* p_ObjectDetectionTitleLabel = new wxStaticText(p_ObjectDetectionPanel, wxID_ANY, wxString::Format("%s for device: %s", _("Object Detection"), m_DeviceParams->m_MVMName),
			wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxALIGN_CENTRE_HORIZONTAL | wxTE_MULTILINE);
		p_ObjectDetectionTitleLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		wxFont TitleFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
		p_ObjectDetectionTitleLabel->SetFont(TitleFont);

		CNNBSSControlStaticBox* p_ObjectDetectionSamplesBox = new CNNBSSControlStaticBox(p_ObjectDetectionPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, 100));
		p_ObjectDetectionSamplesBox->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_ObjectDetectionSamplesBox->SetOwnForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
		wxBoxSizer* p_ObjectDetectionSamplesBoxSizer = new wxBoxSizer(wxVERTICAL);
		p_ObjectDetectionSamples = new wxListBox(p_ObjectDetectionSamplesBox, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxArrayString(), wxNO_BORDER | wxLB_SINGLE | wxLB_HSCROLL | wxLB_NEEDED_SB);
		p_ObjectDetectionSamples->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_ObjectDetectionSamples->SetForegroundColour(RGB_COLOURDEF_WHITE);
		UpdateObjectDetectionCascadeList();// Add all available cascades into the list
		// Add to the sizer
		p_ObjectDetectionSamplesBoxSizer->Add(p_ObjectDetectionSamples, wxSizerFlags().Proportion(1).Expand().Border());
		// Set Sizer for the box container
		p_ObjectDetectionSamplesBox->SetSizer(p_ObjectDetectionSamplesBoxSizer);

		p_DetectionModelActivateCheckBox = new CNNBSSControlCheckBox(p_ObjectDetectionPanel, wxID_ANY, _("Activate"), wxDefaultPosition, wxSize(96, 25), wxNO_BORDER);
		p_DetectionModelActivateCheckBox->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_DetectionModelActivateCheckBox->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_DetectionModelActivateCheckBox->SetToolTip(_("Check if you want to activate this detection for this device."));

		// Detection Area Scale Factor
		wxStaticText* p_DetectionAreaScaleFactorLabel = new wxStaticText(p_ObjectDetectionPanel, wxID_ANY, _("Detection Area Scale Factor:"));
		p_DetectionAreaScaleFactorLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_DetectionAreaScaleFactor = new CNNBSSControlSlider(p_ObjectDetectionPanel, wxID_ANY, CNNBSSControlSlider::SliderType::MONO, 20, 0, 1, 100,
			wxDefaultPosition, wxSize(200, 32), wxSL_VALUE_LABEL | wxSL_MIN_MAX_LABELS);
		p_DetectionAreaScaleFactor->SetToolTip(_("Parameter specifying how much the frame size is reduced. A high value will result in fast but low accuracy."));
		p_DetectionAreaScaleFactor->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_DetectionAreaScaleFactor->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);

		// Detection Scale Factor
		wxStaticText* p_DetectionScaleFactorLabel = new wxStaticText(p_ObjectDetectionPanel, wxID_ANY, _("Detection Scale Factor:"));
		p_DetectionScaleFactorLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_DetectionScaleFactor = new CNNBSSControlSlider(p_ObjectDetectionPanel, wxID_ANY, CNNBSSControlSlider::SliderType::MONO, 11, 0, 1, 100,
			wxDefaultPosition, wxSize(200, 32), wxSL_VALUE_LABEL | wxSL_MIN_MAX_LABELS);
		p_DetectionScaleFactor->SetToolTip(_("Parameter specifying how much the image size is reduced at each image scale. A low value will result in slow but high accuracy."));
		p_DetectionScaleFactor->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_DetectionScaleFactor->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);

		// Minimum neighbor
		wxStaticText* p_MinNeighborsLabel = new wxStaticText(p_ObjectDetectionPanel, wxID_ANY, _("Minimum neighbors:"));
		p_MinNeighborsLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_MinNeighbors = new CNNBSSControlSlider(p_ObjectDetectionPanel, wxID_ANY, CNNBSSControlSlider::SliderType::MONO, 3, 0, 0, 20,
			wxDefaultPosition, wxSize(200, 32), wxSL_VALUE_LABEL | wxSL_MIN_MAX_LABELS);
		p_MinNeighbors->SetToolTip(_("Parameter specifying how many neighbors each candidate rectangle should to retain it.\nBy increasing this number you can eliminate false positives but be careful, by increasing it you can also lose true positives too.\nHigher values means less accuracy but more reliability.\n3~6 is recommeded."));
		p_MinNeighbors->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_MinNeighbors->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);

		// Minimum Rectangle Size
		wxStaticText* p_MinRectangleSizeLabel = new wxStaticText(p_ObjectDetectionPanel, wxID_ANY, _("Ignore if smaller than:"));
		p_MinRectangleSizeLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_MinRectangleSize = new CNNBSSControlSlider(p_ObjectDetectionPanel, wxID_ANY, CNNBSSControlSlider::SliderType::RANGE_BASED, 200, 0, 0, 2048,
			wxDefaultPosition, wxSize(200, 32), wxSL_VALUE_LABEL | wxSL_MIN_MAX_LABELS);
		p_MinRectangleSize->SetToolTip(_("Minimum possible object size. Objects smaller than that are ignored. Set 0 to ignore."));
		p_MinRectangleSize->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_MinRectangleSize->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);

		// Maximum Rectangle Size
		wxStaticText* p_MaxRectangleSizeLabel = new wxStaticText(p_ObjectDetectionPanel, wxID_ANY, _("Ignore if bigger than:"));
		p_MaxRectangleSizeLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_MaxRectangleSize = new CNNBSSControlSlider(p_ObjectDetectionPanel, wxID_ANY, CNNBSSControlSlider::SliderType::RANGE_BASED, 2048, 0, 0, 2048,
			wxDefaultPosition, wxSize(200, 32), wxSL_VALUE_LABEL | wxSL_MIN_MAX_LABELS);
		p_MaxRectangleSize->SetToolTip(_("Maximum possible object size. Objects bigger than that are ignored. Set 0 to ignore."));
		p_MaxRectangleSize->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_MaxRectangleSize->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);

		// Notification Related
		wxStaticText* p_NotificationMessageLabel = new wxStaticText(p_ObjectDetectionPanel, wxID_ANY, _("Notification Message:"));
		p_NotificationMessageLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_NotificationMessageButton = new CNNBSSControlButton(p_ObjectDetectionPanel, NNBSS_EVT_SET_NOTIFICATION_MESSAGE_BUTTON, _("Set Message"), wxDefaultPosition, wxSize(200, 32), wxBORDER_NONE);
		p_NotificationMessageButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE240);
		p_NotificationMessageButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

		wxStaticText* p_NotificationAdditionalNoteLabel = new wxStaticText(p_ObjectDetectionPanel, wxID_ANY, _("Notification Notes:"));
		p_NotificationAdditionalNoteLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_NotificationAdditionalNoteButton = new CNNBSSControlButton(p_ObjectDetectionPanel, NNBSS_EVT_SET_NOTIFICATION_ADDITIONAL_NOTE_BUTTON, _("Set Note"), wxDefaultPosition, wxSize(200, 32), wxBORDER_NONE);
		p_NotificationAdditionalNoteButton->SetForegroundColour(RGB_COLOURDEF_TRIPLE240);
		p_NotificationAdditionalNoteButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

		// Disable controls if we have no detection model
		_HaveAnyDetectionModel = m_CascadeClassifierCacheDataDisplayNames.empty() ? false : true;
		p_DetectionModelActivateCheckBox->Show(_HaveAnyDetectionModel);
		p_DetectionAreaScaleFactorLabel->Show(_HaveAnyDetectionModel);
		p_DetectionAreaScaleFactor->Show(_HaveAnyDetectionModel);
		p_DetectionScaleFactorLabel->Show(_HaveAnyDetectionModel);
		p_DetectionScaleFactor->Show(_HaveAnyDetectionModel);
		p_MinNeighborsLabel->Show(_HaveAnyDetectionModel);
		p_MinNeighbors->Show(_HaveAnyDetectionModel);
		p_MinRectangleSizeLabel->Show(_HaveAnyDetectionModel);
		p_MinRectangleSize->Show(_HaveAnyDetectionModel);
		p_MaxRectangleSizeLabel->Show(_HaveAnyDetectionModel);
		p_MaxRectangleSize->Show(_HaveAnyDetectionModel);
		p_NotificationMessageLabel->Show(_HaveAnyDetectionModel);
		p_NotificationMessageButton->Show(_HaveAnyDetectionModel);
		p_NotificationAdditionalNoteLabel->Show(_HaveAnyDetectionModel);
		p_NotificationAdditionalNoteButton->Show(_HaveAnyDetectionModel);


		EnableDetectionModelControls(_HaveAnyDetectionModel);

		if (!_HaveAnyDetectionModel)
		{
			// Hide list if there's no detection model
			p_ObjectDetectionSamplesBox->Hide();

			// And Set Label
			p_ObjectDetectionTitleLabel->SetLabel("You have no detection model added.\n\nPlease, go to: Services > Detection to add one.");

			p_ObjectDetectionPanelSizer->AddStretchSpacer();
			p_ObjectDetectionPanelSizer->Add(p_ObjectDetectionTitleLabel, wxSizerFlags().Proportion(1).CenterHorizontal().CenterVertical());
			p_ObjectDetectionPanelSizer->AddStretchSpacer();
		}
		else
		{
			// If we have detection model(s), select first one
			p_ObjectDetectionSamples->Select(0);
			//-----------------------------SIZER--------------------------------------------------------------------------------------
			wxFlexGridSizer* p_ElementsSizer = new wxFlexGridSizer(8, 2, 5, 5);
			p_ElementsSizer->Add(p_DetectionAreaScaleFactorLabel, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_DetectionAreaScaleFactor, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_DetectionScaleFactorLabel, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_DetectionScaleFactor, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_MinNeighborsLabel, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_MinNeighbors, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_MinRectangleSizeLabel, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_MinRectangleSize, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_MaxRectangleSizeLabel, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_MaxRectangleSize, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_NotificationMessageLabel, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_NotificationMessageButton, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_NotificationAdditionalNoteLabel, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());
			p_ElementsSizer->Add(p_NotificationAdditionalNoteButton, wxSizerFlags().Proportion(1).CenterVertical().CenterHorizontal());

			p_ObjectDetectionPanelSizer->AddSpacer(FromDIP(25));
			p_ObjectDetectionPanelSizer->Add(p_ObjectDetectionTitleLabel, wxSizerFlags().Proportion(0).CenterHorizontal());
			p_ObjectDetectionPanelSizer->AddSpacer(FromDIP(10));
			p_ObjectDetectionPanelSizer->Add(p_ObjectDetectionSamplesBox, wxSizerFlags().Proportion(0).CenterHorizontal());
			p_ObjectDetectionPanelSizer->AddSpacer(FromDIP(10));
			p_ObjectDetectionPanelSizer->Add(p_DetectionModelActivateCheckBox, wxSizerFlags().Proportion(0).CenterHorizontal());
			p_ObjectDetectionPanelSizer->AddSpacer(FromDIP(10));
			p_ObjectDetectionPanelSizer->Add(p_ElementsSizer, wxSizerFlags().Proportion(0).CenterHorizontal());
			p_ObjectDetectionPanelSizer->AddSpacer(FromDIP(25));
		}

		p_ObjectDetectionPanel->SetSizer(p_ObjectDetectionPanelSizer);

		// Object Detection Ends
	}

	//wxPanel* p_FaceRecognitionPanel = new wxPanel(p_DevicePropertiesNotebook, wxID_ANY);
	//wxPanel* p_MotionDetectionPanel = new wxPanel(p_DevicePropertiesNotebook, wxID_ANY);
	//wxPanel* p_NotificationsPanel = new wxPanel(p_DevicePropertiesNotebook, wxID_ANY);

	// General Properties
	p_GeneralProperties = new wxPanel(p_DevicePropertiesNotebook, wxID_ANY);
	{
		wxBoxSizer* p_GeneralPropertiesBaseSizer = new wxBoxSizer(wxHORIZONTAL);

		CNNBSSControlStaticBox* p_MVMParametersBox = new CNNBSSControlStaticBox(p_GeneralProperties, wxID_ANY, _("Moving Visual Media Parameters"));
		{
			wxGridSizer* p_MVMParametersGridSizer = new wxGridSizer(3, 2, 5, 5);

			p_UseCustomMVMParams = new CNNBSSControlCheckBox(p_MVMParametersBox, wxID_ANY, _("Use Custom"), wxDefaultPosition, wxSize(96, 25));
			p_UseCustomMVMParams->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_UseCustomMVMParams->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

			wxStaticText* p_MVMResolutionLabel = new wxStaticText(p_MVMParametersBox, wxID_ANY, _("Resolution:"));
			p_MVMResolutionLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
			/*Create Instance Array Encapsulates All Resolution Types*/wxArrayString ResolutionList; for (const auto& res : CNNBSSDisplayResolutionAddressHandle().GetResolutions())ResolutionList.Add(res.first);
			const std::string thisRes = CNNBSSDisplayResolutionAddressHandle().GetResolutionAbbreviation(m_DeviceParams->m_MVMWidth, m_DeviceParams->m_MVMHeight);
			p_MVMResolution = new wxComboBox(p_MVMParametersBox, wxID_ANY, thisRes.empty() ? ResolutionList[0] : thisRes, wxDefaultPosition, wxDefaultSize, ResolutionList);
			p_MVMResolution->SetEditable(false);
			p_MVMResolution->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_MVMResolution->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

			wxStaticText* p_MVMFPSLabel = new wxStaticText(p_MVMParametersBox, wxID_ANY, _("FPS:"));
			p_MVMFPSLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_MVMFPS = new CNNBSSControlSlider(p_MVMParametersBox, wxID_ANY, CNNBSSControlSlider::SliderType::MONO, 25, 0, 1, 60);
			p_MVMFPS->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_MVMFPS->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

			p_MVMParametersGridSizer->Add(p_UseCustomMVMParams, wxSizerFlags().CenterVertical().CenterHorizontal().Border(240, 15));
			p_MVMParametersGridSizer->AddSpacer(1);
			p_MVMParametersGridSizer->Add(p_MVMResolutionLabel, 1, wxALL | wxEXPAND, 10);
			p_MVMParametersGridSizer->Add(p_MVMResolution, 1, wxALL | wxEXPAND, 10);
			p_MVMParametersGridSizer->Add(p_MVMFPSLabel, 1, wxALL | wxEXPAND, 10);
			p_MVMParametersGridSizer->Add(p_MVMFPS, 1, wxALL | wxEXPAND, 10);

			wxBoxSizer* p_MVMParametersBoxSizer = new wxBoxSizer(wxVERTICAL);
			p_MVMParametersBoxSizer->Add(0, 15);
			p_MVMParametersBoxSizer->Add(p_MVMParametersGridSizer);

			p_MVMParametersBox->SetAutoLayout(true);
			p_MVMParametersBox->SetSizer(p_MVMParametersBoxSizer);
			p_MVMParametersBox->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
			p_MVMParametersBox->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		}

		// Record Options
		CNNBSSControlStaticBox* p_RecordOptionsBox = new CNNBSSControlStaticBox(p_GeneralProperties, wxID_ANY, _("Record Options"));
		{
			wxGridSizer* p_RecordOptionsGridSizer = new wxGridSizer(4, 2, 5, 5);

			p_Record = new CNNBSSControlCheckBox(p_RecordOptionsBox, wxID_ANY, _("Record"), wxDefaultPosition, wxSize(64, 25));
			p_Record->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_Record->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

			wxStaticText* p_RecordEncodeTypeLabel = new wxStaticText(p_RecordOptionsBox, wxID_ANY, _("Encode Type:"));
			p_RecordEncodeTypeLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);

			wxArrayString EncodeTypes;
			for (int c = 0; c < NNBSSFFMPEG::Encoder_GetList().size(); c++)
				EncodeTypes.Add(NNBSSFFMPEG::Encoder_GetList()[c]);

			p_RecordEncodeType = new wxComboBox(p_RecordOptionsBox, wxID_ANY,
				m_DeviceParams->m_RecordEncoderName.empty() ? EncodeTypes[0] : m_DeviceParams->m_RecordEncoderName,
				wxDefaultPosition, wxDefaultSize, EncodeTypes);
			p_RecordEncodeType->SetEditable(false);
			p_RecordEncodeType->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_RecordEncodeType->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

			wxStaticText* p_RecordResolutionLabel = new wxStaticText(p_RecordOptionsBox, wxID_ANY, _("Resolution:"));
			p_RecordResolutionLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
			/*Create Instance Array Encapsulates All Resolution Types*/wxArrayString ResolutionList; for (const auto& res : CNNBSSDisplayResolutionAddressHandle().GetResolutions())ResolutionList.Add(res.first);
			const std::string thisRes = CNNBSSDisplayResolutionAddressHandle().GetResolutionAbbreviation(m_DeviceParams->m_RecordResolutionWidth, m_DeviceParams->m_RecordResolutionHeight);
			p_RecordResolution = new wxComboBox(p_RecordOptionsBox, wxID_ANY, thisRes.empty() ? ResolutionList[0] : thisRes,
				wxDefaultPosition, wxDefaultSize, ResolutionList);
			p_RecordResolution->SetEditable(false);
			p_RecordResolution->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_RecordResolution->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
			wxStaticText* p_RecordFPSLabel = new wxStaticText(p_RecordOptionsBox, wxID_ANY, _("FPS:"));
			p_RecordFPSLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_RecordFPS = new CNNBSSControlSlider(p_RecordOptionsBox, wxID_ANY, CNNBSSControlSlider::SliderType::MONO,
				m_DeviceParams->m_RecordFPS, 1, 1, 60);
			p_RecordFPS->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_RecordFPS->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

			p_OpenRecordOutputFolder = new CNNBSSControlButton(p_RecordOptionsBox, EID_OPEN_RECORD_OUTPUT_FOLDER, _("Open Output Folder"), wxDefaultPosition, wxSize(120, 32), wxNO_BORDER);

			p_RecordOptionsGridSizer->Add(p_Record, 1, wxALL | wxEXPAND, 15);
			p_RecordOptionsGridSizer->AddSpacer(1);
			p_RecordOptionsGridSizer->Add(p_RecordEncodeTypeLabel, 1, wxALL | wxEXPAND, 10);
			p_RecordOptionsGridSizer->Add(p_RecordEncodeType, 1, wxALL | wxEXPAND, 10);
			p_RecordOptionsGridSizer->Add(p_RecordResolutionLabel, 1, wxALL | wxEXPAND, 10);
			p_RecordOptionsGridSizer->Add(p_RecordResolution, 1, wxALL | wxEXPAND, 10);
			p_RecordOptionsGridSizer->Add(p_RecordFPSLabel, 1, wxALL | wxEXPAND, 10);
			p_RecordOptionsGridSizer->Add(p_RecordFPS, 1, wxALL | wxEXPAND, 10);

			wxBoxSizer* p_RecordOptionsBoxSizer = new wxBoxSizer(wxVERTICAL);
			p_RecordOptionsBoxSizer->Add(0, 15);
			p_RecordOptionsBoxSizer->Add(p_RecordOptionsGridSizer);
			p_RecordOptionsBoxSizer->Add(p_OpenRecordOutputFolder, 1, wxALL | wxEXPAND, 15);

			p_RecordOptionsBox->SetAutoLayout(true);
			p_RecordOptionsBox->SetSizer(p_RecordOptionsBoxSizer);
			p_RecordOptionsBox->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
			p_RecordOptionsBox->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		}

		// Image Processing Options
		CNNBSSControlStaticBox* p_ImgProcOptionsBox = new CNNBSSControlStaticBox(p_GeneralProperties, wxID_ANY, _("Image Processing Options"));
		{
			wxBoxSizer* p_ImgProcOptionsBoxSizer = new wxBoxSizer(wxVERTICAL);
			wxGridSizer* p_ImgProcOptionsGridSizer = new wxGridSizer(2, 2, 5, 5);

			p_UseImgProc = new CNNBSSControlCheckBox(p_ImgProcOptionsBox, wxID_ANY, _("Activate"), wxDefaultPosition, wxSize(96, 25));
			p_UseImgProc->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_UseImgProc->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

			wxStaticText* p_BrightnessControlLabel = new wxStaticText(p_ImgProcOptionsBox, wxID_ANY, _("Brightness:"));
			p_BrightnessControlLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_BrightnessControl = new CNNBSSControlKnob(p_ImgProcOptionsBox, wxID_ANY, m_DeviceParams->m_ImageProcessingBrightness,
				m_DeviceParams->m_ImageProcessingBrightnessDefault, -150, 150, 270, 359);
			p_BrightnessControl->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
			p_BrightnessControl->SetForegroundColour(RGB_COLOURDEF_WHITE);
			wxStaticText* p_ContrastControlLabel = new wxStaticText(p_ImgProcOptionsBox, wxID_ANY, _("Contrast:"));
			p_ContrastControlLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
			p_ContrastControl = new CNNBSSControlKnob(p_ImgProcOptionsBox, wxID_ANY, m_DeviceParams->m_ImageProcessingContrast,
				m_DeviceParams->m_ImageProcessingContrastDefault, -1, 20, 270, 359);
			p_ContrastControl->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
			p_ContrastControl->SetForegroundColour(RGB_COLOURDEF_WHITE);

			p_ImgProcOptionsGridSizer->Add(p_BrightnessControlLabel, 1, wxALL | wxEXPAND, 10);
			p_ImgProcOptionsGridSizer->Add(p_BrightnessControl, 1, wxALL | wxEXPAND, 10);
			p_ImgProcOptionsGridSizer->Add(p_ContrastControlLabel, 1, wxALL | wxEXPAND, 10);
			p_ImgProcOptionsGridSizer->Add(p_ContrastControl, 1, wxALL | wxEXPAND, 10);

			p_ImgProcOptionsBoxSizer->Add(0, 15);
			p_ImgProcOptionsBoxSizer->AddSpacer(FromDIP(15));
			p_ImgProcOptionsBoxSizer->Add(p_UseImgProc, wxSizerFlags().CenterHorizontal());
			p_ImgProcOptionsBoxSizer->Add(p_ImgProcOptionsGridSizer, wxSizerFlags().Proportion(0).CenterHorizontal());
			p_ImgProcOptionsBoxSizer->AddStretchSpacer();

			p_ImgProcOptionsBox->SetAutoLayout(true);
			p_ImgProcOptionsBox->SetSizer(p_ImgProcOptionsBoxSizer);
			p_ImgProcOptionsBox->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
			p_ImgProcOptionsBox->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		}
		p_GeneralPropertiesBaseSizer->AddStretchSpacer();
		p_GeneralPropertiesBaseSizer->Add(p_MVMParametersBox, wxSizerFlags().CenterVertical());
		p_GeneralPropertiesBaseSizer->AddStretchSpacer();
		p_GeneralPropertiesBaseSizer->Add(p_RecordOptionsBox, wxSizerFlags().CenterVertical());
		p_GeneralPropertiesBaseSizer->AddStretchSpacer();
		p_GeneralPropertiesBaseSizer->Add(p_ImgProcOptionsBox, wxSizerFlags().CenterVertical());
		p_GeneralPropertiesBaseSizer->AddStretchSpacer();
		p_GeneralProperties->SetAutoLayout(true);
		p_GeneralProperties->SetSizer(p_GeneralPropertiesBaseSizer);
	}

	wxPanel* p_StreamPanel = new wxPanel(p_DevicePropertiesNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	{
		wxBoxSizer* p_StreamPanelSizer = new wxBoxSizer(wxVERTICAL);

		// Stream starts
		CNNBSSControlStaticBox* p_StreamPanelBox = new CNNBSSControlStaticBox(p_StreamPanel, wxID_ANY, wxEmptyString);
		p_StreamPanelBox->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_StreamPanelBox->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
		wxBoxSizer* p_StreamPanelBoxSizer = new wxBoxSizer(wxVERTICAL);

		p_StartStream = new CNNBSSControlCheckBox(p_StreamPanelBox, wxID_ANY, _("Start Stream"), wxDefaultPosition, wxSize(100, 25));
		p_StartStream->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_StartStream->SetForegroundColour(RGB_COLOURDEF_WHITE);

		wxGridSizer* p_StreamElementsGridSizer = new wxGridSizer(5, 2, 5, 5);

		// Address
		wxStaticText* p_StreamAddressLabel = new wxStaticText(p_StreamPanelBox, wxID_ANY, _("Address:"));
		p_StreamAddressLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		CNNBSSControlStaticBox* p_StreamAddressBox = new CNNBSSControlStaticBox(p_StreamPanelBox, wxID_ANY, wxEmptyString);
		p_StreamAddressBox->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_StreamAddressBox->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
		p_StreamAddress = new wxTextCtrl(p_StreamAddressBox, wxID_ANY,
			m_DeviceParams->m_StreamAddress.empty() ? wxEmptyString : wxString(m_DeviceParams->m_StreamAddress), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
		p_StreamAddress->SetHint(_("Example:127.0.0.1"));
		p_StreamAddress->SetMaxLength(25);
		p_StreamAddress->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_StreamAddress->SetForegroundColour(RGB_COLOURDEF_WHITE);
		wxBoxSizer* p_StreamAddressBoxSizer = new wxBoxSizer(wxVERTICAL);
		p_StreamAddressBoxSizer->Add(p_StreamAddress, wxSizerFlags().Proportion(1).Expand().Border());
		p_StreamAddressBox->SetSizer(p_StreamAddressBoxSizer);

		// Port 
		wxStaticText* p_StreamPortLabel = new wxStaticText(p_StreamPanelBox, wxID_ANY, _("Port:"));
		p_StreamPortLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		CNNBSSControlStaticBox* p_StreamPortBox = new CNNBSSControlStaticBox(p_StreamPanelBox, wxID_ANY, wxEmptyString);
		p_StreamPortBox->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_StreamPortBox->SetForegroundColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
		p_StreamPort = new wxTextCtrl(p_StreamPortBox, wxID_ANY,
			m_DeviceParams->m_StreamAddressPort.empty() ? wxEmptyString : wxString(m_DeviceParams->m_StreamAddressPort), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
		p_StreamPort->SetHint(_("Example:39009"));
		p_StreamPort->SetMaxLength(5);
		p_StreamPort->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
		p_StreamPort->SetForegroundColour(RGB_COLOURDEF_WHITE);
		wxBoxSizer* p_StreamPortBoxSizer = new wxBoxSizer(wxVERTICAL);
		p_StreamPortBoxSizer->Add(p_StreamPort, wxSizerFlags().Proportion(1).Expand().Border());
		p_StreamPortBox->SetSizer(p_StreamPortBoxSizer);

		wxStaticText* p_StreamResolutionLabel = new wxStaticText(p_StreamPanelBox, wxID_ANY, _("Resolution:"));
		p_StreamResolutionLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		/*Create Instance Array Encapsulates All Resolution Types*/wxArrayString ResolutionList; for (const auto& res : CNNBSSDisplayResolutionAddressHandle().GetResolutions())ResolutionList.Add(res.first);
		const std::string thisRes = CNNBSSDisplayResolutionAddressHandle().GetResolutionAbbreviation(m_DeviceParams->m_RecordResolutionWidth, m_DeviceParams->m_RecordResolutionHeight);
		p_StreamResolution = new wxComboBox(p_StreamPanelBox, wxID_ANY, thisRes.empty() ? ResolutionList[0] : thisRes
			, wxDefaultPosition, wxDefaultSize, ResolutionList);
		p_StreamResolution->SetEditable(false);
		p_StreamResolution->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_StreamResolution->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

		wxStaticText* p_StreamCompressionLabel = new wxStaticText(p_StreamPanelBox, wxID_ANY, _("Compression:"));
		p_StreamCompressionLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);

		p_StreamCompression = new CNNBSSControlSlider(p_StreamPanelBox, wxID_ANY, CNNBSSControlSlider::SliderType::MONO,
			m_DeviceParams->m_StreamCompressPercentage, 0, 0, 100, wxDefaultPosition, wxDefaultSize);
		p_StreamCompression->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_StreamCompression->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

		wxStaticText* p_StreamFPSLabel = new wxStaticText(p_StreamPanelBox, wxID_ANY, _("FPS Limit:"));
		p_StreamFPSLabel->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_StreamFPS = new CNNBSSControlSlider(p_StreamPanelBox, wxID_ANY, CNNBSSControlSlider::SliderType::MONO,
			m_DeviceParams->m_StreamFPS, 1, 1, 60);
		p_StreamFPS->SetForegroundColour(RGB_COLOURDEF_WHITE);
		p_StreamFPS->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

		p_StreamElementsGridSizer->Add(p_StreamAddressLabel, wxSizerFlags().Proportion(1).CenterHorizontal().CenterVertical());
		p_StreamElementsGridSizer->Add(p_StreamAddressBox, wxSizerFlags().Proportion(0).Expand());
		p_StreamElementsGridSizer->Add(p_StreamPortLabel, wxSizerFlags().Proportion(1).CenterHorizontal().CenterVertical());
		p_StreamElementsGridSizer->Add(p_StreamPortBox, wxSizerFlags().Proportion(0).Expand());
		p_StreamElementsGridSizer->Add(p_StreamResolutionLabel, wxSizerFlags().Proportion(1).CenterHorizontal().CenterVertical());
		p_StreamElementsGridSizer->Add(p_StreamResolution, wxSizerFlags().Proportion(1).Expand().Border());
		p_StreamElementsGridSizer->Add(p_StreamCompressionLabel, wxSizerFlags().Proportion(1).CenterHorizontal().CenterVertical());
		p_StreamElementsGridSizer->Add(p_StreamCompression, wxSizerFlags().Proportion(1).Expand().Border());
		p_StreamElementsGridSizer->Add(p_StreamFPSLabel, wxSizerFlags().Proportion(1).CenterHorizontal().CenterVertical());
		p_StreamElementsGridSizer->Add(p_StreamFPS, wxSizerFlags().Proportion(1).Expand().Border());

		p_StreamPanelBoxSizer->Add(p_StartStream, wxSizerFlags().Proportion(0).CenterHorizontal().Border());
		p_StreamPanelBoxSizer->Add(p_StreamElementsGridSizer, wxSizerFlags().Proportion(0).CenterHorizontal().Border());

		p_StreamPanelBox->SetSizerAndFit(p_StreamPanelBoxSizer);

		p_StreamPanelSizer->AddStretchSpacer();
		p_StreamPanelSizer->Add(p_StreamPanelBox, wxSizerFlags().CenterHorizontal());
		p_StreamPanelSizer->AddStretchSpacer();
		p_StreamPanel->SetSizer(p_StreamPanelSizer);
	}

	//p_DevicePropertiesNotebook->AddPage(p_FaceRecognitionPanel, _("Facial Recognition"), false, 0);
	//p_DevicePropertiesNotebook->AddPage(p_MotionDetectionPanel, _("Motion Detection"), false, 0);
	//p_DevicePropertiesNotebook->AddPage(p_NotificationsPanel, _("Notifications"), false, 0);
	p_DevicePropertiesNotebook->AddPage(p_ObjectDetectionPanel, _("Object Detection"), true, 0);
	p_DevicePropertiesNotebook->AddPage(p_StreamPanel, _("Stream"), false, 0);
	p_DevicePropertiesNotebook->AddPage(p_GeneralProperties, _("Properties"), false, 0);

	// SubButtonAreaSizers handling
	p_SubButtonAreaSizer->Add(p_CloseButton, wxSizerFlags().Proportion(0));
	p_SubButtonAreaSizer->AddSpacer(FromDIP(10));
	p_SubButtonAreaSizer->Add(p_ResetButton, wxSizerFlags().Proportion(0));
	p_SubButtonAreaVerticalSizer->Add(p_SubButtonAreaSizer, wxSizerFlags().Proportion(0).Align(wxALIGN_CENTER_HORIZONTAL));
	p_SubButtonArea->SetSizer(p_SubButtonAreaVerticalSizer);

	p_DevicePropertiesNotebookSizer->Add(p_DevicePropertiesNotebook, wxSizerFlags().Proportion(1).Expand());
	p_DevicePropertiesNotebookSizer->AddSpacer(1);
	p_DevicePropertiesNotebookSizer->Add(p_SubButtonArea, wxSizerFlags().Proportion(0).Expand());

	p_MainSizer->Add(p_DraggableHeader, wxSizerFlags().Expand());
	p_MainSizer->AddSpacer(1);
	p_MainSizer->Add(p_DevicePropertiesNotebookSizer, wxSizerFlags().Proportion(1).Expand());
	p_MainSizer->FitInside(this);
	SetSizer(p_MainSizer);

	_SetupDesign();

	_BindEvents(true);
	{
		wxCommandEvent fake;// to fake call to update

		// after binding events, controls will be able to react set value commands:
		p_UseCustomMVMParams->Check(m_DeviceParams->m_MVMUseDefault);
		MVMParamsActivation(fake);

		p_Record->Check(m_DeviceParams->m_Record);
		RecordActivation(fake);

		p_StartStream->CheckSilent(m_DeviceParams->m_Streaming);
		StreamActivation(fake);

		p_UseImgProc->Check(m_DeviceParams->m_ImageProcessingActivate);// And function binded to the event will be called
		ImageProcessingActivation(fake);
	}

	SetSize(wxSize(860, 600));

	// Selects the first Object Detection Model element if exist any (also assigns the init values)
	OnStartHandleObjectDetectionCascadeList();
}

void CNNBSSDevicePropertiesDialog::_SetupDesign()
{
	if (!p_DevicePropertiesNotebook)
		return;

	SetBackgroundColour(RGB_COLOURDEF_PLACEHOLDER);

	p_DevicePropertiesNotebook->SetThemeEnabled(true);
	p_DevicePropertiesNotebook->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_DevicePropertiesNotebook->SetTabAreaColour(RGB_COLOURDEF_TRIPLE20);
	p_DevicePropertiesNotebook->SetActiveTabColour(RGB_COLOURDEF_TRIPLE20);
	p_DevicePropertiesNotebook->SetActiveTabTextColour(RGB_COLOURDEF_WHITE);
	p_DevicePropertiesNotebook->SetNonActiveTabTextColour(RGB_COLOURDEF_TRIPLE100);
	p_DevicePropertiesNotebook->SetSingleLineBorderColor(RGB_COLOURDEF_NNBSSBLUE);
	p_DevicePropertiesNotebook->SetWindowStyle(wxFNB_NO_X_BUTTON | wxFNB_NO_NAV_BUTTONS | wxFNB_NODRAG);


	// SubButtons
	p_SubButtonArea->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	p_CloseButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_CloseButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_CloseButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_DEVICESLIST_DIALOGCANCEL_IDLE));

	p_ResetButton->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	p_ResetButton->SetForegroundColour(RGB_COLOURDEF_WHITE);
	p_ResetButton->SetButtonBitmap(NNBSS_PNG2BMP(NNBSSART_GLOBAL_RESET_16x16p_IDLE));
	/// End subbuttons
}

void CNNBSSDevicePropertiesDialog::_BindEvents(bool bind)
{
	// Binding
	if (bind)
	{
		// Object Detection
		p_ObjectDetectionPanel->Bind(wxEVT_LISTBOX, &CNNBSSDevicePropertiesDialog::OnObjectDetectionSelectModel, this);
		p_DetectionModelActivateCheckBox->Bind(wxEVT_CHECKBOX, &CNNBSSDevicePropertiesDialog::OnObjectDetectionModelCheckbox, this);
		p_DetectionAreaScaleFactor->Bind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSDevicePropertiesDialog::OnAreaScaleFactorChange, this);
		p_DetectionScaleFactor->Bind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSDevicePropertiesDialog::OnScaleFactorChange, this);
		p_MinNeighbors->Bind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSDevicePropertiesDialog::OnMinNeighborsChange, this);
		p_MinRectangleSize->Bind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSDevicePropertiesDialog::OnMinRectangleSizeChange, this);
		p_MaxRectangleSize->Bind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSDevicePropertiesDialog::OnMaxRectangleSizeChange, this);
		p_NotificationMessageButton->Bind(wxEVT_LEFT_UP, &CNNBSSDevicePropertiesDialog::OnAnyButtonClick, this);
		p_NotificationAdditionalNoteButton->Bind(wxEVT_LEFT_UP, &CNNBSSDevicePropertiesDialog::OnAnyButtonClick, this);

		// General Properties
				// MVM Params such as input resolution, fps and so on
		p_UseCustomMVMParams->Bind(wxEVT_CHECKBOX, &CNNBSSDevicePropertiesDialog::MVMParamsActivation, this);
		// Record Options
		p_Record->Bind(wxEVT_CHECKBOX, &CNNBSSDevicePropertiesDialog::RecordActivation, this);
		p_OpenRecordOutputFolder->Bind(wxEVT_LEFT_UP, &CNNBSSDevicePropertiesDialog::OnAnyButtonClick, this);
		// Image Processing
		p_UseImgProc->Bind(wxEVT_CHECKBOX, &CNNBSSDevicePropertiesDialog::ImageProcessingActivation, this);
		p_BrightnessControl->Bind(wxEVT_SCROLL_THUMBRELEASE, &CNNBSSDevicePropertiesDialog::ImageProcessingBrightnessChange, this);
		p_ContrastControl->Bind(wxEVT_SCROLL_THUMBRELEASE, &CNNBSSDevicePropertiesDialog::ImageProcessingContrastChange, this);

		// Stream
		p_StartStream->Bind(wxEVT_CHECKBOX, &CNNBSSDevicePropertiesDialog::StreamActivation, this);


		p_CloseButton->Bind(wxEVT_LEFT_UP, &CNNBSSDevicePropertiesDialog::OnAnyButtonClick, this);
		p_ResetButton->Bind(wxEVT_LEFT_UP, &CNNBSSDevicePropertiesDialog::OnAnyButtonClick, this);
	}
	else
	{
		p_ObjectDetectionPanel->Unbind(wxEVT_LISTBOX, &CNNBSSDevicePropertiesDialog::OnObjectDetectionSelectModel, this);
		p_DetectionModelActivateCheckBox->Unbind(wxEVT_CHECKBOX, &CNNBSSDevicePropertiesDialog::OnObjectDetectionModelCheckbox, this);
		p_DetectionAreaScaleFactor->Unbind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSDevicePropertiesDialog::OnAreaScaleFactorChange, this);
		p_DetectionScaleFactor->Unbind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSDevicePropertiesDialog::OnScaleFactorChange, this);
		p_MinNeighbors->Unbind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSDevicePropertiesDialog::OnMinNeighborsChange, this);
		p_MinRectangleSize->Unbind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSDevicePropertiesDialog::OnMinRectangleSizeChange, this);
		p_MaxRectangleSize->Unbind(EVT_CONTROLSLIDER_ON_CHANGE, &CNNBSSDevicePropertiesDialog::OnMaxRectangleSizeChange, this);

		// General Properties
		// MVM Params such as input resolution, fps and so on
		p_UseCustomMVMParams->Unbind(wxEVT_CHECKBOX, &CNNBSSDevicePropertiesDialog::MVMParamsActivation, this);
		// Record Options
		p_Record->Unbind(wxEVT_CHECKBOX, &CNNBSSDevicePropertiesDialog::RecordActivation, this);
		// Image Processing
		p_UseImgProc->Unbind(wxEVT_CHECKBOX, &CNNBSSDevicePropertiesDialog::ImageProcessingActivation, this);
		p_BrightnessControl->Unbind(wxEVT_SCROLL_THUMBRELEASE, &CNNBSSDevicePropertiesDialog::ImageProcessingBrightnessChange, this);
		p_ContrastControl->Unbind(wxEVT_SCROLL_THUMBRELEASE, &CNNBSSDevicePropertiesDialog::ImageProcessingContrastChange, this);

		// Stream
		p_StartStream->Unbind(wxEVT_CHECKBOX, &CNNBSSDevicePropertiesDialog::StreamActivation, this);

		p_NotificationMessageButton->Unbind(wxEVT_LEFT_UP, &CNNBSSDevicePropertiesDialog::OnAnyButtonClick, this);
		p_NotificationAdditionalNoteButton->Unbind(wxEVT_LEFT_UP, &CNNBSSDevicePropertiesDialog::OnAnyButtonClick, this);

		p_CloseButton->Unbind(wxEVT_LEFT_UP, &CNNBSSDevicePropertiesDialog::OnAnyButtonClick, this);
		p_ResetButton->Unbind(wxEVT_LEFT_UP, &CNNBSSDevicePropertiesDialog::OnAnyButtonClick, this);
	}
}

// Manually select 0 index of Object Detection Samples listbox on start of this dialog
void CNNBSSDevicePropertiesDialog::OnStartHandleObjectDetectionCascadeList()
{
	// sanity check
	if (!p_ObjectDetectionSamples || p_ObjectDetectionSamples->IsEmpty())
		return;

	p_DetectionModelActivateCheckBox->CheckSilent(false);

	// Here, we'll check whether detection
	for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
	{
		// (Device name and Address is enough to validate I think)
		if (p.DeviceName == m_DeviceParams->m_MVMName && p.Address == m_DeviceParams->m_MVMSourceAddress)
		{
			if (p._AssignedModelPath ==
				NNBSSDetectionModel::CNNBSSDetectionModelDataList[0].Path)
			{
				p_DetectionModelActivateCheckBox->CheckSilent(true);

				p_DetectionAreaScaleFactor->SetValue((int)((double)p.m_arrayScaleFactor * 10.0), CNNBSSControlSlider::ThumbType::HIGH);
				p_DetectionScaleFactor->SetValue((int)((double)p.m_scaleFactor * 10.0), CNNBSSControlSlider::ThumbType::HIGH);
				p_MinNeighbors->SetValue(p.m_minNeighbors, CNNBSSControlSlider::ThumbType::HIGH);

				p_MinRectangleSize->SetValue(p.m_minSize.GetWidth(), CNNBSSControlSlider::ThumbType::LOW);
				p_MinRectangleSize->SetValue(p.m_minSize.GetHeight(), CNNBSSControlSlider::ThumbType::HIGH);

				p_MaxRectangleSize->SetValue(p.m_maxSize.GetWidth(), CNNBSSControlSlider::ThumbType::LOW);
				p_MaxRectangleSize->SetValue(p.m_maxSize.GetHeight(), CNNBSSControlSlider::ThumbType::HIGH);
			}

		}
	}

	EnableDetectionModelControls(p_DetectionModelActivateCheckBox->GetValue());

}

void CNNBSSDevicePropertiesDialog::UpdateObjectDetectionCascadeList()
{
	m_CascadeClassifierCacheDataDisplayNames.clear();

	for (const auto& p : NNBSSDetectionModel::CNNBSSDetectionModelDataList)
	{
		m_CascadeClassifierCacheDataDisplayNames.Add(p.DisplayName);
	}

	// Display in list widget
	// 
	// sanity check
	if (!p_ObjectDetectionSamples)
		return;
	// If exist, remove all elements first
	p_ObjectDetectionSamples->Clear();
	// Then appen all
	p_ObjectDetectionSamples->Append(m_CascadeClassifierCacheDataDisplayNames);
}

void CNNBSSDevicePropertiesDialog::EnableDetectionModelControls(bool enable)
{
	p_DetectionAreaScaleFactor->Enable(enable);
	p_DetectionScaleFactor->Enable(enable);
	p_MinNeighbors->Enable(enable);
	p_MinRectangleSize->Enable(enable);
	p_MaxRectangleSize->Enable(enable);
	p_NotificationMessageButton->Enable(enable);
	p_NotificationAdditionalNoteButton->Enable(enable);
}

void CNNBSSDevicePropertiesDialog::OnAnyButtonClick(wxMouseEvent& event)
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

			destroy = true;
		}
		break;
	case NNBSS_EVT_CANCEL:
		EndModal(NNBSS_EVT_CANCEL);
		destroy = true;
		break;
	case NNBSS_EVT_SET_NOTIFICATION_MESSAGE_BUTTON:
	{
		for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
		{
			if (p.DeviceName == GetCurrentParams().DeviceName && p.Address == GetCurrentParams().Address
				&& p._AssignedModelPath == GetCurrentParams()._AssignedModelPath)
			{
				wxTextEntryDialog* p_LabelDialog = new wxTextEntryDialog(GetActiveUXFrame(), _("Change the label of this detection model"),
					_("Change Detection ModelLabel"), p._NotificationMessage);

				if (p_LabelDialog->ShowModal() == wxID_OK)
				{
					p._NotificationMessage = p_LabelDialog->GetValue();
				}
				p_LabelDialog->Destroy();

				break;
			}
		}
	}
	break;
	case NNBSS_EVT_SET_NOTIFICATION_ADDITIONAL_NOTE_BUTTON:
	{
		for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
		{
			if (p.DeviceName == GetCurrentParams().DeviceName && p.Address == GetCurrentParams().Address
				&& p._AssignedModelPath == GetCurrentParams()._AssignedModelPath)
			{
				wxTextEntryDialog* p_LabelDialog = new wxTextEntryDialog(GetActiveUXFrame(), _("Change the label of this detection model"),
					_("Change Detection ModelLabel"), p._AdditionalUserNotes);

				if (p_LabelDialog->ShowModal() == wxID_OK)
				{
					p._AdditionalUserNotes = p_LabelDialog->GetValue();
				}
				p_LabelDialog->Destroy();

				break;
			}
		}
	}
	break;
	// Record Options
	case EID_OPEN_RECORD_OUTPUT_FOLDER:
	{
		std::string directory = NNBSS_DIR__PLAYBACK_DIR.ToStdString();
		nnbsstd::FileExplorerAt(directory);
	}
	break;
	default:
		break;
	}

	event.Skip();

	if (destroy)
	{
		_BindEvents(false);

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
		CNNBSSDevicePropertiesDialogAddressHandle(m_DeviceParams, GetActiveUXFrame(), true);
	}
}

void CNNBSSDevicePropertiesDialog::OnObjectDetectionModelCheckbox(wxCommandEvent& evt)
{

	if (p_DetectionModelActivateCheckBox->GetValue())
	{
		// First check if we have that value already
		for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
		{
			if (p.DeviceName == GetCurrentParams().DeviceName && p.Address == GetCurrentParams().Address
				&& p._AssignedModelPath == GetCurrentParams()._AssignedModelPath)
			{
				p = GetCurrentParams();

				evt.Skip();
				return;
			}
		}

		// If we could not find it
		// Append it
		NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.emplace_back(GetCurrentParams());
	}
	else
	{
		int index = -1;

		for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
		{
			if (p.DeviceName == GetCurrentParams().DeviceName && p.Address == GetCurrentParams().Address
				&& p._AssignedModelPath == GetCurrentParams()._AssignedModelPath)
			{
				index++;
			}
		}

		if (index != -1)
		{
			NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.erase(NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.begin() + index);
		}

	}

	EnableDetectionModelControls(p_DetectionModelActivateCheckBox->GetValue());

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::OnObjectDetectionSelectModel(wxCommandEvent& event)
{
	p_ObjectDetectionSamples->GetString(p_ObjectDetectionSamples->GetSelection());

	p_DetectionModelActivateCheckBox->CheckSilent(false);

	for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
	{
		// Device name and Address is enough to validate I think
		if (p.DeviceName == m_DeviceParams->m_MVMName && p.Address == m_DeviceParams->m_MVMSourceAddress)
		{
			if (p._AssignedModelPath ==
				NNBSSDetectionModel::CNNBSSDetectionModelDataList[p_ObjectDetectionSamples->GetSelection()].Path)
			{
				p_DetectionModelActivateCheckBox->CheckSilent(true);

				p_DetectionAreaScaleFactor->SetValue((int)((double)p.m_arrayScaleFactor * 10.0), CNNBSSControlSlider::ThumbType::HIGH);
				p_DetectionScaleFactor->SetValue((int)((double)p.m_scaleFactor * 10.0), CNNBSSControlSlider::ThumbType::HIGH);
				p_MinNeighbors->SetValue(p.m_minNeighbors, CNNBSSControlSlider::ThumbType::HIGH);

				p_MinRectangleSize->SetValue(p.m_minSize.GetWidth(), CNNBSSControlSlider::ThumbType::LOW);
				p_MinRectangleSize->SetValue(p.m_minSize.GetHeight(), CNNBSSControlSlider::ThumbType::HIGH);

				p_MaxRectangleSize->SetValue(p.m_maxSize.GetWidth(), CNNBSSControlSlider::ThumbType::LOW);
				p_MaxRectangleSize->SetValue(p.m_maxSize.GetHeight(), CNNBSSControlSlider::ThumbType::HIGH);
			}

		}
	}

	EnableDetectionModelControls(p_DetectionModelActivateCheckBox->GetValue());

	event.Skip();
}

void CNNBSSDevicePropertiesDialog::OnAreaScaleFactorChange(CNNBSSEventsControlSlider& evt)
{

	for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
	{
		if (p.DeviceName == GetCurrentParams().DeviceName && p.Address == GetCurrentParams().Address
			&& p._AssignedModelPath == GetCurrentParams()._AssignedModelPath)
		{
			p = GetCurrentParams();

			evt.Skip();
			return;
		}
	}

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::OnScaleFactorChange(CNNBSSEventsControlSlider& evt)
{

	for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
	{
		if (p.DeviceName == GetCurrentParams().DeviceName && p.Address == GetCurrentParams().Address
			&& p._AssignedModelPath == GetCurrentParams()._AssignedModelPath)
		{
			p = GetCurrentParams();

			evt.Skip();
			return;
		}
	}

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::OnMinNeighborsChange(CNNBSSEventsControlSlider& evt)
{

	for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
	{
		if (p.DeviceName == GetCurrentParams().DeviceName && p.Address == GetCurrentParams().Address
			&& p._AssignedModelPath == GetCurrentParams()._AssignedModelPath)
		{
			p = GetCurrentParams();

			evt.Skip();
			return;
		}
	}

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::OnMinRectangleSizeChange(CNNBSSEventsControlSlider& evt)
{

	for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
	{
		if (p.DeviceName == GetCurrentParams().DeviceName && p.Address == GetCurrentParams().Address
			&& p._AssignedModelPath == GetCurrentParams()._AssignedModelPath)
		{
			p = GetCurrentParams();

			evt.Skip();
			return;
		}
	}

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::OnMaxRectangleSizeChange(CNNBSSEventsControlSlider& evt)
{

	for (auto& p : NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList)
	{
		if (p.DeviceName == GetCurrentParams().DeviceName && p.Address == GetCurrentParams().Address
			&& p._AssignedModelPath == GetCurrentParams()._AssignedModelPath)
		{
			p = GetCurrentParams();

			evt.Skip();
			return;
		}
	}

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::MVMParamsActivation(wxCommandEvent& evt)
{
	p_MVMResolution->Enable(!p_UseCustomMVMParams->GetValue());
	p_MVMFPS->Enable(!p_UseCustomMVMParams->GetValue());

	// If use custom settings
	if (p_UseCustomMVMParams->GetValue())
	{
		// Get Record Width & Height
		int mvmWidth, mvmHeight;
		CNNBSSDisplayResolutionAddressHandle().GetResolution(p_MVMResolution->GetValue().ToStdString(),
			mvmWidth, mvmHeight);

		m_DeviceParams->m_MVMWidth = mvmWidth;
		m_DeviceParams->m_MVMHeight = mvmHeight;

		m_DeviceParams->m_MVMFPS = p_MVMFPS->GetValue(CNNBSSControlSlider::ThumbType::HIGH);
	}
	else
	{
		m_DeviceParams->m_MVMWidth = 0;
		m_DeviceParams->m_MVMHeight = 0;

		m_DeviceParams->m_MVMFPS = 0;
	}

	m_DeviceParams->m_MVMUseDefault = p_UseCustomMVMParams->GetValue();

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::RecordActivation(wxCommandEvent& evt)
{
	// There's a !(not) operator before condition because at record time record settings can't be changed@
	p_RecordEncodeType->Enable(!p_Record->GetValue());
	p_RecordResolution->Enable(!p_Record->GetValue());
	p_RecordFPS->Enable(!p_Record->GetValue());

	if (p_Record->GetValue())// No need to set variables when it gets to be unchecked
	{
		// Set Encoder
		{
			m_DeviceParams->m_RecordEncoderName = p_RecordEncodeType->GetValue();
		}

		// Get Record Width & Height
		int recWidth, recHeight;
		CNNBSSDisplayResolutionAddressHandle().GetResolution(p_RecordResolution->GetValue().ToStdString(),
			recWidth, recHeight);
		m_DeviceParams->m_RecordResolutionWidth = recWidth;
		m_DeviceParams->m_RecordResolutionHeight = recHeight;
		//--------------------------

		// Not debugged, but should be
		const int _RecFPS = p_RecordFPS->GetValue(CNNBSSControlSlider::ThumbType::HIGH);
		m_DeviceParams->m_RecordFPS = _RecFPS;
	}

	m_DeviceParams->m_Record = p_Record->GetValue();// Record or not

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::ImageProcessingActivation(wxCommandEvent& evt)
{
	p_BrightnessControl->Enable(p_UseImgProc->GetValue());
	p_ContrastControl->Enable(p_UseImgProc->GetValue());

	m_DeviceParams->m_ImageProcessingActivate = p_UseImgProc->GetValue();

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::ImageProcessingBrightnessChange(wxScrollEvent& evt)
{
	m_DeviceParams->m_ImageProcessingBrightness = evt.GetInt();

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::ImageProcessingContrastChange(wxScrollEvent& evt)
{
	m_DeviceParams->m_ImageProcessingContrast = evt.GetInt();

	evt.Skip();
}

void CNNBSSDevicePropertiesDialog::StreamActivation(wxCommandEvent& evt)
{
	auto checkDemands = [&]()
	{
		const wxString address = p_StreamAddress->GetValue();
		const wxString port = p_StreamPort->GetValue();

		if (address.empty())
		{
			wxRichToolTip tip("Wait", _("Address can't be empty"));
			tip.SetIcon(wxICON_ERROR);
			tip.ShowFor(p_StreamAddress);// 'this' keyword is just used as place holder
			return false;
		}

		if (port.empty())
		{
			wxRichToolTip tip("Wait", _("Port can't be empty"));
			tip.SetIcon(wxICON_ERROR);
			tip.ShowFor(p_StreamPort);// 'this' keyword is just used as place holder
			return false;
		}

		auto validateIP = [&address]()
		{
			const std::string number_0_255 = "((([0-9])|([1-9][0-9])|(1[0-9][0-9]|2[0-4][0-9]|25[0-5])){1})";
			const std::string dot = "(\\.){1}";
			static const boost::regex e(number_0_255 + dot + number_0_255 + dot + number_0_255 + dot + number_0_255);
			if (!regex_match(address.ToStdString(), e))
			{
				return false;
			}
			return true;
		};

		if (!validateIP())
		{
			wxRichToolTip tip("Wait", _("Enter valid address"));
			tip.SetIcon(wxICON_ERROR);
			tip.ShowFor(p_StreamAddress);// 'this' keyword is just used as place holder
			return false;
		}

		auto validatePort = [&port]()
		{
			int _port = -1;
			try
			{
				_port = std::stoi(port.ToStdString());
			}
			catch (...)
			{
				_port = -1;
			}

			if (_port < 0 || _port > 65535)
				return false;

			return true;
		};

		if (!validatePort())
		{
			wxRichToolTip tip("Wait", _("Enter valid port"));
			tip.SetIcon(wxICON_ERROR);
			tip.ShowFor(p_StreamPort);// 'this' keyword is just used as place holder
			return false;
		}

		return true;
	};

	if (!checkDemands()) {
		evt.Skip();
		p_StartStream->CheckSilent(false);/*Check silent does not fire an event, so there won't be a call loop for this function*/
		return;
	}

	// There's a !(not) operator before condition because at record time record settings can't be changed
	p_StreamAddress->Enable(!p_StartStream->GetValue());
	p_StreamPort->Enable(!p_StartStream->GetValue());
	p_StreamCompression->Enable(!p_StartStream->GetValue());
	p_StreamResolution->Enable(!p_StartStream->GetValue());
	p_StreamFPS->Enable(!p_StartStream->GetValue());

	if (p_StartStream->GetValue())// No need to set variables when it gets to be unchecked
	{
		m_DeviceParams->m_StreamAddress = p_StreamAddress->GetValue();
		m_DeviceParams->m_StreamAddressPort = p_StreamPort->GetValue();

		// Get Stream Width & Height
		int recWidth, recHeight;
		CNNBSSDisplayResolutionAddressHandle().GetResolution(p_StreamResolution->GetValue().ToStdString(),
			recWidth, recHeight);
		m_DeviceParams->m_StreamResolutionWidth = recWidth;
		m_DeviceParams->m_StreamResolutionHeight = recHeight;
		//--------------------------

		// Set Encoder
		{
			m_DeviceParams->m_StreamCompressPercentage = p_StreamCompression->GetValue(CNNBSSControlSlider::ThumbType::HIGH);
		}

		// Not debugged, but should be
		const int _StreamFPS = p_StreamFPS->GetValue(CNNBSSControlSlider::ThumbType::HIGH);
		m_DeviceParams->m_StreamFPS = _StreamFPS;
	}

	m_DeviceParams->m_Streaming = p_StartStream->GetValue();// Stream or not

	evt.Skip();
}

NNBSSDetectionModel::SCNNBSSDeviceAssignedDetectionModels CNNBSSDevicePropertiesDialog::GetCurrentParams()
{
	NNBSSDetectionModel::SCNNBSSDeviceAssignedDetectionModels currentParams;

	// get label
	for (const auto& P : NNBSSDetectionModel::CNNBSSDetectionModelDataList)
	{
		currentParams._Label = P.Label;
	}

	currentParams.DeviceName = m_DeviceParams->m_MVMName;
	currentParams.Address = m_DeviceParams->m_MVMSourceAddress;
	currentParams.DeviceNetName = m_DeviceParams->m_DeviceNetName;
	currentParams.DevicePath = m_DeviceParams->m_DevicePath;
	currentParams.m_arrayScaleFactor = (double)p_DetectionAreaScaleFactor->GetValue(CNNBSSControlSlider::ThumbType::HIGH) / 10.0;
	currentParams.m_scaleFactor = (double)p_DetectionScaleFactor->GetValue(CNNBSSControlSlider::ThumbType::HIGH) / 10.0;
	currentParams.m_minNeighbors = p_MinNeighbors->GetValue(CNNBSSControlSlider::ThumbType::HIGH);
	currentParams.m_minSize = wxSize(p_MinRectangleSize->GetValue(CNNBSSControlSlider::ThumbType::LOW), p_MinRectangleSize->GetValue(CNNBSSControlSlider::ThumbType::HIGH));
	currentParams.m_maxSize = wxSize(p_MaxRectangleSize->GetValue(CNNBSSControlSlider::ThumbType::LOW), p_MaxRectangleSize->GetValue(CNNBSSControlSlider::ThumbType::HIGH));

	// Each Detection Model/cascade classifier is considered to be working when it's added here.
	// If the thread that uses this Detection model throws an exception, Works variable becomes false.
	// (And for now) -Thread itself notifies CNNBSSDetectionModelDataList that this model is not working-(Should admit that this is not the best idea)
	currentParams.Works = true;

	currentParams._AssignedModelPath =
		NNBSS_PROJECT_DIRECTORY + NNBSS_DETECTIONMODELS_DIRECTORY + NNBSS_SEPARATOR + p_ObjectDetectionSamples->GetString(p_ObjectDetectionSamples->GetSelection()).ToStdString();

	return currentParams;
}

bool CNNBSSDevicePropertiesDialog::CheckDialogDemandMeet()
{
	return true;
}