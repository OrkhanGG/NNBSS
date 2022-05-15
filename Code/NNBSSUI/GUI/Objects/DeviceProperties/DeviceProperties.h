#ifndef DEVICEPROPERTIES_H
#define DEVICEPROPERTIES_H

namespace NNBSSDetectionModel 
{
	class SCNNBSSDeviceAssignedDetectionModels;
}
class SNNBSSMVMData;
class CNNBSSControlCheckBox;
class CNNBSSControlSlider;
class CNNBSSControlKnob;
class CNNBSSEventsControlSlider;
class wxFlatNotebook;
class wxFlatNotebookImageList;

class wxComboBox;
class wxListBox;

class CNNBSSDevicePropertiesDialog : public wxDialog
{
public:
	CNNBSSDevicePropertiesDialog(SNNBSSMVMData* DeviceParams, wxWindow* parent = NULL, wxWindowID id = wxID_ANY,
		const wxString& title = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0L,
		const wxString& name = wxT("DefaultDevicePropertiesDialogName"));
	~CNNBSSDevicePropertiesDialog() {};

	void _SetupInterface();

	void _SetupDesign();

private:
	enum EventID
	{
		EID_OPEN_RECORD_OUTPUT_FOLDER = wxID_HIGHEST+1
	};

	void _BindEvents(bool bind = true);

	void OnAnyButtonClick(wxMouseEvent& event);

	// Object Detection Page related
	void OnStartHandleObjectDetectionCascadeList();
	void UpdateObjectDetectionCascadeList();
	void EnableDetectionModelControls(bool enable);
	void OnObjectDetectionModelCheckbox(wxCommandEvent& evt);
	void OnObjectDetectionSelectModel(wxCommandEvent& event);
	void OnAreaScaleFactorChange(CNNBSSEventsControlSlider& evt);
	void OnScaleFactorChange(CNNBSSEventsControlSlider& evt);
	void OnMinNeighborsChange(CNNBSSEventsControlSlider& evt);
	void OnMinRectangleSizeChange(CNNBSSEventsControlSlider& evt);
	void OnMaxRectangleSizeChange(CNNBSSEventsControlSlider& evt);

	// General Properties
			// MVM Params
	void MVMParamsActivation(wxCommandEvent& evt);
			// Record
	void RecordActivation(wxCommandEvent& evt);
			// Image Processing
	void ImageProcessingActivation(wxCommandEvent& evt);
	void ImageProcessingBrightnessChange(wxScrollEvent& evt);
	void ImageProcessingContrastChange(wxScrollEvent& evt);
	// Stream Properties
	void StreamActivation(wxCommandEvent& evt);

	NNBSSDetectionModel::SCNNBSSDeviceAssignedDetectionModels GetCurrentParams();

	bool CheckDialogDemandMeet();

	// Outside
	SNNBSSMVMData* m_DeviceParams = nullptr;

	// Notebook related
	wxFlatNotebook* p_DevicePropertiesNotebook = nullptr;
	wxFlatNotebookImageList* p_DevicePropertiesNotebookIcons = nullptr;

	//Object detection page attrs
	bool _HaveAnyDetectionModel = false;
	wxArrayString m_CascadeClassifierCacheDataDisplayNames;
	wxPanel* p_ObjectDetectionPanel = nullptr;
	CNNBSSControlCheckBox* p_DetectionModelActivateCheckBox = nullptr;
	wxListBox* p_ObjectDetectionSamples = nullptr;
	CNNBSSControlSlider* p_DetectionAreaScaleFactor = nullptr;
	CNNBSSControlSlider* p_DetectionScaleFactor = nullptr;
	CNNBSSControlSlider* p_MinNeighbors = nullptr;
	CNNBSSControlSlider* p_MinRectangleSize = nullptr;
	CNNBSSControlSlider* p_MaxRectangleSize = nullptr;
	CNNBSSControlButton* p_NotificationMessageButton = nullptr;
	CNNBSSControlButton* p_NotificationAdditionalNoteButton = nullptr;

	// Stream Properties
	CNNBSSControlCheckBox* p_StartStream = nullptr;
	wxTextCtrl* p_StreamAddress = nullptr;
	wxTextCtrl* p_StreamPort = nullptr;
	CNNBSSControlSlider* p_StreamCompression = nullptr;
	wxComboBox* p_StreamResolution = nullptr;
	CNNBSSControlSlider* p_StreamFPS = nullptr;

	// General Properties
	wxPanel* p_GeneralProperties = nullptr;
			// MVM
	CNNBSSControlCheckBox* p_UseCustomMVMParams = nullptr;
	wxComboBox* p_MVMResolution = nullptr;
	CNNBSSControlSlider* p_MVMFPS = nullptr;
			// Record
	CNNBSSControlCheckBox* p_Record = nullptr;
	wxComboBox* p_RecordEncodeType = nullptr;
	wxComboBox* p_RecordResolution = nullptr;
	CNNBSSControlSlider* p_RecordFPS = nullptr;
	CNNBSSControlButton* p_OpenRecordOutputFolder = nullptr;
			// Image Processing
	CNNBSSControlCheckBox* p_UseImgProc = nullptr;
	CNNBSSControlKnob* p_BrightnessControl = nullptr;
	CNNBSSControlKnob* p_ContrastControl = nullptr;

	// Main
	wxBoxSizer* p_MainSizer = nullptr;
	wxBoxSizer* p_DevicePropertiesNotebookSizer = nullptr;
	wxBoxSizer* p_SubButtonAreaSizer = nullptr;
	wxBoxSizer* p_SubButtonAreaVerticalSizer = nullptr;
	wxPanel* p_SubButtonArea = nullptr;
	CNNBSSControlButton* p_CloseButton = nullptr;
	CNNBSSControlButton* p_ResetButton = nullptr;
};

#endif // ! DEVICEPROPERTIES_H