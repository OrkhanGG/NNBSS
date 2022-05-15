#ifndef LOCALIZATION_H
#define LOCALIZATION_H

class CNNBSSLocalizationManager
{
public:
	CNNBSSLocalizationManager() {};
	~CNNBSSLocalizationManager() {};

	void OnInit();
protected:
	wxLanguage m_lang;  // language specified by user
	wxLocale m_locale;  // locale we'll be using
};

#endif // ! LOCALIZATION_H