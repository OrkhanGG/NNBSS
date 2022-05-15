///////////////////////////////////////////////////////////////////////////////
// Author:      Orkhan Aliyev
// Copyright:   (c) 2021 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef NOTIFICATIONSMANAGER_H
#define NOTIFICATIONSMANAGER_H

#include "Notifications.h"

// Standart C Lib
#include <exception>

inline CNNBSSNotifications* CNNBSSNotificationsAddressHandle(bool destroy = false)
{
    // allocation
	static CNNBSSNotifications* pNotifications = new CNNBSSNotifications();

    if (destroy)
    {
        // deallocation
        // this condition is true when CNNBSSGUIMAINFRAME::~CNNBSSNotifications is calling this function
        if(pNotifications)
            pNotifications->~CNNBSSNotifications();
        return nullptr;
    }

    wxASSERT(pNotifications);
	return pNotifications;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Busy info
template<typename ... Args>
inline void NNBSSBusyInfoHandle(const std::string& format, bool show = true, Args ... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::runtime_error("Error during formatting!"); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);

    std::string fString = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside

    CNNBSSNotifications* pNotifications = CNNBSSNotificationsAddressHandle();
    pNotifications->PopUpBusyInfo(wxString(fString.c_str(), wxConvUTF8)/*convert wxString*/,show);
}
// This will lock the main frame unless you call this function again with show = false argument
#define NNBSSBusyInfo(format, show, ...)  NNBSSBusyInfoHandle(format, show, ## __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ... Args>
inline void NNBSSDisplayInformationFrame(const std::string& format, unsigned int timeout = 100U, Args ... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::runtime_error("Error during formatting!"); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);

    std::string fString = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside

    CNNBSSNotifications* pNotifications = CNNBSSNotificationsAddressHandle();
    pNotifications->PopUpInformationMessage(wxString(fString.c_str(), wxConvUTF8)/*convert wxString*/
    , timeout);
}
#define NNBSSInfoShow(format, timeout, ...)  NNBSSDisplayInformationFrame(format, timeout, ## __VA_ARGS__)


template<typename ... Args>
inline void NNBSSDisplayErrorFrame(const std::string& format, unsigned int timeout = 100U, Args ... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::runtime_error("Error during formatting!"); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);

    std::string fString = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside

    CNNBSSNotifications* pNotifications = CNNBSSNotificationsAddressHandle();
    pNotifications->PopUpErrorMessage(wxString(fString.c_str(), wxConvUTF8)/*convert wxString*/
        , timeout);
}
#define NNBSSErrorShow(format, timeout, ...)  NNBSSDisplayErrorFrame(format, timeout, ## __VA_ARGS__)


/////////////////////////////////////////////////////////////////////////////////////////////////////////

inline int NNBSSMessageBoxSimple(const wxString& caption,
    const wxString& message,
    long style = wxOK | wxCENTRE,
    wxWindow* parent = NULL,
    int x = wxDefaultCoord, int y = wxDefaultCoord)
{
    CNNBSSMessageBox* p_MessageBox = new CNNBSSMessageBox(message,caption,style,parent,x,y);
    const int _answer = p_MessageBox->ShowModal();
    return _answer;
}

#endif // ! NOTIFICATIONSMANAGER_H
