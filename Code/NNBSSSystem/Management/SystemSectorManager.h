#ifndef SYSTEMSECTORMANAGER_H
#define SYSTEMSECTORMANAGER_H

// Standart C Libs
#include <iostream>
#include <stdio.h>
#include <fstream>
// Boost Libs
#include <boost/filesystem.hpp>
// NNBSS System - Authorization
#include <NNBSSSystem/Authorization/Authorization.h>
// NNBSS System - Codec
#include <NNBSSSystem/Codec/Video/FFMPEGCodec.h>
//#include "../Codec/Video/FourCC.h" Deprecated
// NNBSS System - Localization
#include <NNBSSSystem/localization/Localization.h>
// NNBSS System - TinyXML
#include <NNBSSSystem/XML/tinystr.h>
#include <NNBSSSystem/XML/tinyxml.h>

namespace NNBSSLocalization
{
    inline CNNBSSLocalizationManager* CNNBSSLocalizationManagerAddressHandle(bool destroy = false)
    {
        static CNNBSSLocalizationManager* p_CNNBSSLocalizationManager = nullptr;

        if (destroy && p_CNNBSSLocalizationManager)
        {
            delete p_CNNBSSLocalizationManager;
            return nullptr;
        }

        p_CNNBSSLocalizationManager = new CNNBSSLocalizationManager();

        return p_CNNBSSLocalizationManager;
    }
}


namespace NNBSSFileSystem
{
    // Returns true if everything about paths was okay else false.
    // Note: no need to use "\\" in fileName or more accurately, after destinationPath
    inline bool NNBSS_COPYPASTE(std::string filePath, std::string destinationDirectory, std::string fileName)
    {
        {
            // check sanity
            if (filePath.empty() || destinationDirectory.empty() || fileName.empty())
                return false;
        }

        if (filePath == destinationDirectory)
            return false;

        if (boost::filesystem::copy_file(filePath, destinationDirectory + fileName, boost::filesystem::copy_option::overwrite_if_exists))
            return true;

        return false;
    }

    // Returns the given file's size
    inline size_t NNBSS_GETFILESIZE(std::string filePath)
    {
        return (size_t)boost::filesystem::file_size(filePath);
    }
}

#endif // ! SYSTEMSECTORMANAGER_H