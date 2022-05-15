///////////////////////////////////////////////////////////////////////////////
// Name:        GridViewPanel.h
// Purpose:     Base for all panels that will display multiple cameras with NNBSS standard UX
// Author:      Orkhan Aliyev
// Created:     2021-12-07
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#ifndef GRIDVIEWPANEL_H
#define GRIDVIEWPANEL_H

// Standard C Lib
#include <map>

// Forward decs
class wxSizer;
class wxBoxSizer;
class wxGridSizer;
class wxMouseEvent;
class wxDropFilesEvent;

class CNNBSSBitmapPanel;
class SNNBSSMVMData;
class CNNBSSMovingVisualMediaThread;
class CNNBSSBitmapFrameStandalone;
class MovingVisualMediaEvent;
class CNNBSSEventsDeviceList;

/* Thumbnail Container - Use when you need to display a retrieved frame from any camera*/
struct SStandardThumbnail
{
	CNNBSSBitmapPanel*				m_ThumbnailPanel{ nullptr };
	CNNBSSBitmapFrameStandalone*     p_StandalonePanel{ nullptr };

	wxString						m_DeviceName;
	wxString						m_DeviceAddress;

	bool                            m_IsEmpty{ true };
	bool                            m_IsCreated{ false };
};

class CNNBSSGridViewPanelBase : public wxPanel
{
public:
																	CNNBSSGridViewPanelBase(wxWindow* parent);
	virtual															~CNNBSSGridViewPanelBase() {};

	virtual void													SetupDesign();
	virtual void													_BindEvents(bool bind = true);

	void															ShowStandaloneFrame(CNNBSSBitmapPanel* BitmapPanel);
	CNNBSSBitmapFrameStandalone*									GetStandaloneFrame();

	CNNBSSMovingVisualMediaThread*									GetThumbnailThread(CNNBSSBitmapPanel* panel)const;
	CNNBSSMovingVisualMediaThread*									GetThumbnailThread(SNNBSSMVMData* params)const;

	SNNBSSMVMData*													GetThumbnailMVMParameters(CNNBSSBitmapPanel* panel);

	SStandardThumbnail												GetThumbnailContainer(CNNBSSBitmapPanel* panel);

	virtual void													ThumbnailHandle();

	virtual CNNBSSBitmapPanel*										GetSelectedThumbnail();

	virtual void													FullscreenThumbnail(CNNBSSBitmapPanel* cameraPanel, bool fullscreen = true);

	virtual bool													_IsFullScreen()const;

	virtual int														_GetGridWidth()const;
	virtual int														_GetGridHeight()const;
	virtual void													_SetGridWidth(const int& width);
	virtual void													_SetGridHeight(const int& height);
	virtual void													_SetBalancedGridScale(const int& balancedScale);
	virtual bool													ChangeGridSize(const wxSize& size);

	virtual CNNBSSBitmapPanel*										FindThumbnailPanel(SNNBSSMVMData* params) const;

	/*--------------------------------------------------------------EVENTS------------------------------------------------*/
	virtual void													OnAnyButtonLeftUp(wxMouseEvent& evt);

	virtual void													OnContextMenu(wxContextMenuEvent& evt) = 0;

	virtual void													OnDropCameraOnThumbnail(wxDropFilesEvent& event) = 0;

	virtual void                                                    OnAnyThumbnailClick(wxMouseEvent& evt) = 0;
	virtual void                                                    OnAnyThumbnailDoubleClick(wxMouseEvent& evt) = 0;

	virtual void													OnAnyThumbnailSetFocus(wxFocusEvent& evt);
	virtual void													OnAnyThumbnailKillFocus(wxFocusEvent& evt);
	/*------------------------------------------------------DEVICE-RELATED-EVENTS----------------------------------------*/
	virtual void													OnDeviceListEvent(CNNBSSEventsDeviceList& evt) = 0;

	virtual void                                                    OnDeviceAddAttempt(MovingVisualMediaEvent& evt) = 0;
	virtual void                                                    OnDeviceConnectionFail(MovingVisualMediaEvent& evt) = 0;
	virtual void                                                    OnDeviceConnectionLevelChange(MovingVisualMediaEvent& evt) = 0;
	virtual void                                                    OnDeviceConnected(MovingVisualMediaEvent& evt) = 0;
	virtual void                                                    OnDeviceDisconnected(MovingVisualMediaEvent& evt) = 0;

	virtual void                                                    OnCameraCaptureStarted(MovingVisualMediaEvent& evt) = 0;
	virtual void                                                    OnCameraCommandResult(MovingVisualMediaEvent& evt) = 0;
	virtual void                                                    OnCameraErrorException(MovingVisualMediaEvent& evt) = 0;
	virtual void                                                    OnCameraErrorOpen(MovingVisualMediaEvent& evt) = 0;
	virtual void                                                    OnCameraErrorEmpty(MovingVisualMediaEvent& evt) = 0;
	/*------------------------------------------------------DEVICE-RELATED-EVENTS-END------------------------------------*/

	std::map<unsigned int, SStandardThumbnail>						m_Thumbnails;

private:
	wxPanel*														p_GridView = nullptr;
	wxSizer*														m_FullScreenSizer = nullptr;
	wxGridSizer*													p_GridViewSizer = nullptr;
	wxBoxSizer*														p_GridViewGeneralSizer = nullptr;
	wxPanel*														p_GridViewMenuBar = nullptr;
	CNNBSSControlButton*											p_GridViewMenuBar2x2Btn = nullptr;
	CNNBSSControlButton*											p_GridViewMenuBar4x4Btn = nullptr;
	CNNBSSControlButton*											p_GridViewMenuBarEditBtn = nullptr;
	CNNBSSControlButton*											p_GridViewMenuBarFullScreenBtn = nullptr;
	wxBoxSizer*														p_GridViewMenuBarHorizontalSizer = nullptr;

	bool															_IsExpanded = false;
	int																_GridWidth = 2;
	int																_GridHeight = 2;
};

#endif // ! GRIDVIEWPANEL_H