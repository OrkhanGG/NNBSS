#ifndef PROFILES_H
#define PROFILES_H

// forward decs
class CNNBSSBitmapPanel;
class wxSimplebook;

// Standard C Lib
#include <array>

class CNNBSSProfilesInterface : public wxPanel
{
public:
	CNNBSSProfilesInterface(wxWindow* parent);
	~CNNBSSProfilesInterface();

	void _SetupInterface();

private:
	void OnAnyButtonClick(wxMouseEvent& evt);

	void OnAnyThumbnailClick(wxMouseEvent& evt);

	void CreatePageContent(const int pageID);

	struct _PAGES
	{
		wxSimplebook* p_PageContainer = nullptr;

		std::array<wxPanel*, 3> p_Pages;
		int currentPageID = 0;

	}m_P;

	struct _FACEIDATTR
	{
		std::vector<CNNBSSBitmapPanel*> p_BitmapPanels;
		
		CNNBSSBitmapPanel* currentBitmapPanel = nullptr;
	}m_FIDA;
};

#endif // ! PROFILES_H