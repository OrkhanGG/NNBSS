///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
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

#include "AUIManager.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// wxWidgets
#include <wx/aui/aui.h>
#include <wx/aui/auibar.h>
#include <wx/aui/auibook.h>
#include <wx/aui/barartmsw.h>
#include <wx/aui/dockart.h>
#include <wx/aui/floatpane.h>
#include <wx/aui/tabart.h>
#include <wx/aui/tabartgtk.h>
#include <wx/aui/tabartmsw.h>
#include <wx/aui/tabmdi.h>

CNNBSSAuiManager::~CNNBSSAuiManager()
{
}
void CNNBSSAuiManager::_SetupUXWindowInterface()
{
	wxWindow* ManagedWindow = GetManagedWindow();

	// Control Panel
	CNNBSSControlPanelAddressHandle()->_Create(ManagedWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxNO_BORDER);
	CNNBSSControlPanelAddressHandle()->DisableFocusFromKeyboard();
	CNNBSSAuiManagerAddressHandle()->AddPane(CNNBSSControlPanelAddressHandle(),
		wxAuiPaneInfo().Name("ControlPanel").
		Caption("Control Panel").Center().Layer(2).Floatable(false).GripperTop(false).
		Gripper(false).PaneBorder(false).CaptionVisible(true).Movable(false).Resizable(true)
		.CloseButton(false).MaximizeButton(true));

	// Device Status Bar
	CNNBSSDeviceStatusBarAddressHandle()->_Create(ManagedWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxNO_BORDER);
	CNNBSSAuiManagerAddressHandle()->AddPane(CNNBSSDeviceStatusBarAddressHandle()->GetPanel(),
		wxAuiPaneInfo().Name("StatusBar").
		Caption("Status Bar").Bottom().Layer(1).PinButton().MinSize(100,CNNBSSDeviceStatusBarAddressHandle()->GetMinSize().y)
		.FloatingSize(CNNBSSDeviceStatusBarAddressHandle()->GetBestSize())
		.LeftDockable(false).RightDockable(false));

	// Device List Tree
	CNNBSSDeviceListTreeCtrlAddressHandle()->_Create(ManagedWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxNO_BORDER);
	CNNBSSAuiManagerAddressHandle()->AddPane(CNNBSSDeviceListTreeCtrlAddressHandle()->GetPanel(),
		wxAuiPaneInfo().Name("DeviceManager").
		Caption("Device Manager").Left().Layer(0).PinButton().MinSize(CNNBSSDeviceListTreeCtrlAddressHandle()->GetMinSize()));
	CNNBSSDeviceListTreeCtrlAddressHandle()->_UpdateList();

	// Default design on start
	_SetDefaultDesign();

	// tell the manager to "commit" all the changes just made
	CNNBSSAuiManagerAddressHandle()->Update();
}

void CNNBSSAuiManager::_SetDefaultDesign()
{
	// Dock art colour set
	GetDockArt()->SetColor(wxAUI_DOCKART_BACKGROUND_COLOUR, RGB_COLOURDEF_TRIPLE20);
	GetDockArt()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR, RGB_COLOURDEF_TRIPLE20);
	GetDockArt()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR, RGB_COLOURDEF_TRIPLE20);
	GetDockArt()->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR, RGB_COLOURDEF_TRIPLE20);
	GetDockArt()->SetColor(wxAUI_DOCKART_BORDER_COLOUR, RGB_COLOURDEF_TRIPLE20);
	GetDockArt()->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR, RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	GetDockArt()->SetColor(wxAUI_DOCKART_SASH_COLOUR, RGB_COLOURDEF_PLACEHOLDER);
	GetDockArt()->SetColor(wxAUI_DOCKART_GRIPPER_COLOUR, RGB_COLOURDEF_TRIPLE20);
	GetDockArt()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR, RGB_COLOURDEF_TRIPLE240);
	GetDockArt()->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR, RGB_COLOURDEF_TRIPLE240);

	//_Size_ 
	GetDockArt()->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,
		5);
	GetDockArt()->SetMetric(wxAUI_DOCKART_SASH_SIZE,
		3);

	const int flag = wxAUI_MGR_LIVE_RESIZE | wxAUI_MGR_ALLOW_FLOATING | wxAUI_MGR_TRANSPARENT_DRAG | wxAUI_MGR_HINT_FADE | wxAUI_MGR_TRANSPARENT_HINT | wxAUI_MGR_VENETIAN_BLINDS_HINT;

	CNNBSSAuiManagerAddressHandle()->SetFlags(flag);
}

wxAuiDockArt* CNNBSSAuiManager::GetDockArt()
{
	return GetArtProvider();
}
