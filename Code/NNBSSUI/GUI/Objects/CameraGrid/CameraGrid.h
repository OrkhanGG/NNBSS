///////////////////////////////////////////////////////////////////////////////
// Name:        cameragrid.cpp
// Purpose:     Displays thumbnails from multiple cameras
// Author:      PB
// Created:     2021-11-18, Edited By Orkhan Aliyev: 2021-12-07
// Copyright:   (c) 2021 PB
///////////////////////////////////////////////////////////////////////////////

#ifndef CAMERAGRID_H
#define CAMERAGRID_H

// NNBSS UI
#include <NNBSSUI/GUI/Objects/BaseClasses/GridViewPanel/GridViewPanel.h>

class CNNBSSCameraGrid : public CNNBSSGridViewPanelBase
{
public:

	CNNBSSCameraGrid(wxWindow* parent = nullptr);
	virtual ~CNNBSSCameraGrid();

	void															LinkPanel(SNNBSSMVMData* params, const int& rowNumber);
	void															ResetPanel(SNNBSSMVMData* params);

	void															OnDeviceListEvent(CNNBSSEventsDeviceList& evt) wxOVERRIDE;
private:

	void															OnDeviceAddAttempt(MovingVisualMediaEvent& evt) wxOVERRIDE;
	void															OnDeviceConnectionFail(MovingVisualMediaEvent& evt) wxOVERRIDE;
	void															OnDeviceConnectionLevelChange(MovingVisualMediaEvent& evt) wxOVERRIDE;
	void															OnDeviceConnected(MovingVisualMediaEvent& evt) wxOVERRIDE;
	void															OnDeviceDisconnected(MovingVisualMediaEvent& evt) wxOVERRIDE;

	void															OnCameraCaptureStarted(MovingVisualMediaEvent& evt) wxOVERRIDE;
	void															OnCameraCommandResult(MovingVisualMediaEvent& evt) wxOVERRIDE;
	void															OnCameraErrorException(MovingVisualMediaEvent& evt) wxOVERRIDE;
	void															OnCameraErrorOpen(MovingVisualMediaEvent& evt) wxOVERRIDE;
	void															OnCameraErrorEmpty(MovingVisualMediaEvent& evt) wxOVERRIDE;

	void															OnContextMenu(wxContextMenuEvent& evt)wxOVERRIDE;
	void															OnDropCameraOnThumbnail(wxDropFilesEvent& event) wxOVERRIDE;
	void															OnAnyThumbnailClick(wxMouseEvent& evt) wxOVERRIDE;
	void															OnAnyThumbnailDoubleClick(wxMouseEvent& evt) wxOVERRIDE;

	enum ContextMenuEventID
	{
		CMEID_NONE = 304,
		CMEID_STANDALONEFRAME_SHOW,
		CMEID_FULLSCREEN,
		CMEID_REMOVE,
		CMEID_HORIZONTAL_FLIP,
		CMEID_VERTICAL_FLIP,
		CMEID_DISPLAY_HISTOGRAM
	};

	struct ContextMenu
	{
		wxMenu* p_ContextMenu = nullptr;
		wxMenu* p_DisplaySubMenu = nullptr;

		wxMenuItem* StandaloneFrameMenuItem = nullptr;
		wxMenuItem* FullscreenMenuItem = nullptr;
		wxMenuItem* RemoveMenuItem = nullptr;

		wxMenuItem* HorizontalFlipMenuItem = nullptr;
		wxMenuItem* VerticalFlipMenuItem = nullptr;
		wxMenuItem* HistogramToggle = nullptr;
	}m_ContextMenu;

};

#endif // #ifndef CAMERAGRID_H