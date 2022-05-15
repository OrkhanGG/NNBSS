///////////////////////////////////////////////////////////////////////////////
// Name:        onecameraframe.h
// Purpose:     Displays full resolution output from a single camera
// Author:      PB
// Created:     2021-11-18, Edited By Orkhan Aliyev: 2021-12-07
// Copyright:   (c) 2021 PB
///////////////////////////////////////////////////////////////////////////////

#ifndef STANDALONEFRAME_H
#define STANDALONEFRAME_H

class CNNBSSBitmapPanel;
class SNNBSSMVMData;
class CNNBSSMovingVisualMediaThread;

class CNNBSSBitmapFrameStandalone : public wxFrame
{
public:
	CNNBSSBitmapFrameStandalone(wxWindow* parent, SNNBSSMVMData* mvmParams);
	~CNNBSSBitmapFrameStandalone();

	void                            SetCameraBitmap(const wxBitmap& bitmap);

	CNNBSSBitmapPanel*				GetBitmapPanel() const { return m_cameraPanel; }
	SNNBSSMVMData*           GetMVMParams() const { return m_mvmParams; }
private:
	enum                            ContextMenuEventID
	{
		CMEID_CLOSE = 404,
		CMEID_RESTORE,
		CMEID_MINIMIZE,

		CMEID_WINDOWPRIORITY,// Not used yet, but will definitely be implemented

		CMEID_WINDOWSTYLE_STAYONTOP,
		CMEID_WINDOWSTYLE_SHOWCAPTION,
		CMEID_WINDOWSTYLE_SETCAPTION,
	};

	struct ContextMenu
	{
		wxMenu*						p_PreferencesSubMenu = nullptr;
		wxMenuItem*					p_StayOnTopItem = nullptr;
		wxMenuItem*					p_ShowCaptionItem = nullptr;
		wxMenuItem*					p_SetCaptionItem = nullptr;

		wxMenu*						p_MainContextMenu = nullptr;
	}m_CM;

	long m_WindowFlags{0x0040};// wxWidgets special wxRESIZE_BORDER

	CNNBSSMovingVisualMediaThread*  GetThumbnailThread();

	void                            OnLeftDown(wxMouseEvent& evt);
	void                            OnLeftUp(wxMouseEvent& evt);
	void                            OnDoubleClick(wxMouseEvent& evt);
	void                            OnMouseMove(wxMouseEvent& evt);
	void                            OnContextMenu(wxContextMenuEvent& evt);

	bool                            m_clientSizeAdjusted{ false };

	CNNBSSBitmapPanel*				m_cameraPanel{ nullptr };

	SNNBSSMVMData*           m_mvmParams;

	// Override MSWWindowProc to catch the events
	WXLRESULT                       MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam) wxOVERRIDE;

	// Mouse var(s)
	wxPoint                         m_delta = wxPoint(0, 0);
};

#endif // ! STANDALONEFRAME_H