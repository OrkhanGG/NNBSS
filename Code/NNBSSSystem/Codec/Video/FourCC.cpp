///////////////////////////////////////////////////////////////////////////////
// Name:        FourCC.h
// Purpose:     Deprecated! Don't use anymore
// Author:      Orkhan Aliyev
// Created:     2022-01-15
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#include "FourCC.h"

#include <NNBSSBridge/MainBridge.h>
//
//void SNNBSSFourCC::CacheAvailableCodecs()
//{
//	if (s_Codecs.empty() || s_Codecs == m_DefaultCodecs)
//	{
//		s_Codecs.clear();
//		for (auto& currentCodec : m_Codecs)
//		{
//			// if OS is linux or OSX then use FFmpeg with grep command
//#if defined (OS_LINUX) || defined (OS_OSX)
//			std::string _command = "ffmpeg -codecs | grep -i ";
//			// if WINDOWS then FFmpeg with findstr
//#elif defined (OS_WINDOWS)
//			std::string _command = "ffmpeg -codecs | findstr /i MPEG";
//#endif
//
//			// execute the system command and get the output
//			std::string str = nnbsstd::ExecuteCommand(_command);
//
//			// if the output string contains the letters DEV then FFmpeg supports the codec
//			// D: Decoding supported
//			// E: Encoding supported
//			// V: Video codec
//			std::size_t found = str.find("DEV");
//
//			if (found != std::string::npos)
//				s_Codecs.emplace_back(currentCodec);//std::cout << "Codec Found" << std::endl;
//		}
//	}
//}