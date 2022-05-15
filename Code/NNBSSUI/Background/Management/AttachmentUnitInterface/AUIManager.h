///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef AUIMANAGER_H
#define AUIMANAGER_H

// wxWidgets
#include <wx/aui/framemanager.h>

class CNNBSSAuiManager : public wxAuiManager
{
public:
	CNNBSSAuiManager() {};
	~CNNBSSAuiManager();

	void _SetupUXWindowInterface();

	void _SetDefaultDesign();

	wxAuiDockArt* GetDockArt();
};

#endif // ! AUIMANAGER_H