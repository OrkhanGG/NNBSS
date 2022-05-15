// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
	// Include your minimal set of headers here, or wx.h
#   include <wx/wx.h>
#endif

#include "Localization.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

// language data
static const wxLanguage langIds[] =
{
	wxLANGUAGE_DEFAULT,
	wxLANGUAGE_FRENCH,
	wxLANGUAGE_ITALIAN,
	wxLANGUAGE_GERMAN,
	wxLANGUAGE_RUSSIAN,
	wxLANGUAGE_BULGARIAN,
	wxLANGUAGE_CZECH,
	wxLANGUAGE_POLISH,
	wxLANGUAGE_SWEDISH,
#if wxUSE_UNICODE || defined(__WXMOTIF__)
	wxLANGUAGE_JAPANESE,
#endif
#if wxUSE_UNICODE
	wxLANGUAGE_GEORGIAN,
	wxLANGUAGE_ENGLISH,
	wxLANGUAGE_ENGLISH_US,
	wxLANGUAGE_ARABIC,
	wxLANGUAGE_ARABIC_EGYPT
#endif
};

// note that it makes no sense to translate these strings, they are
// shown before we set the locale anyhow
const wxString langNames[] =
{
	"System default",
	"French",
	"Italian",
	"German",
	"Russian",
	"Bulgarian",
	"Czech",
	"Polish",
	"Swedish",
#if wxUSE_UNICODE || defined(__WXMOTIF__)
	"Japanese",
#endif
#if wxUSE_UNICODE
	"Georgian",
	"English",
	"English (U.S.)",
	"Arabic",
	"Arabic (Egypt)"
#endif
};

// the arrays must be in sync
wxCOMPILE_TIME_ASSERT(WXSIZEOF(langNames) == WXSIZEOF(langIds),
	LangArraysMismatch);

const std::vector<wxString> localizedStrings =
{
	// Dialogs
	{_("Are you sure?")}
	,{_("Okay")}
	,{_("Cancel")}
	,{_("Accept")}
	,{_("Reject")}

	// Services Panel
	,{_("Services")}
	,{_("Devices")}
	,{_("Grid View")}
	,{_("Notifications")}
	,{_("Playback")}
	,{_("Profiles")}
	,{_("Detection")}

	// Others
	,{_("Device List")}
	,{_("Notification")}
	,{_("Detection Models")}

};


void CNNBSSLocalizationManager::OnInit()
{
	m_lang = wxLANGUAGE_DEFAULT;// always?

	// don't use wxLOCALE_LOAD_DEFAULT flag so that Init() doesn't return
// false just because it failed to load wxstd catalog
	if (!m_locale.Init(m_lang, wxLOCALE_DONT_LOAD_DEFAULT))
	{
		wxLogWarning(_("This language is not supported by the system."));

		// continue nevertheless
	}

	// normally this wouldn't be necessary as the catalog files would be found
	// in the default locations, but when the program is not installed the
	// catalogs are in the build directory where we wouldn't find them by
	// default
	wxLocale::AddCatalogLookupPathPrefix("Data\\Localization\\");

	// Initialize the catalogs we'll be using
	const wxLanguageInfo* pInfo = wxLocale::GetLanguageInfo(m_lang);
	if (!m_locale.AddCatalog("NNBSS_LOCALIZATION"))
	{
		wxLogError(_("Couldn't find/load the 'NNBSS_LOCALIZATION' catalog for locale '%s'."),
			pInfo ? pInfo->GetLocaleName() : _("unknown"));
	}

	// Now try to add wxstd.mo so that loading "NOTEXIST.ING" file will produce
	// a localized error message:
	m_locale.AddCatalog("wxstd");
	// NOTE: it's not an error if we couldn't find it!

	// this catalog is installed in standard location on Linux systems and
	// shows that you may make use of the standard message catalogs as well
	//
	// if it's not installed on your system, it is just silently ignored
#ifdef __LINUX__
	{
		wxLogNull noLog;
		m_locale.AddCatalog("fileutils");
	}
#endif
}