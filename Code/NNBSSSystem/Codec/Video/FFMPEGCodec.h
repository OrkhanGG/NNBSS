///////////////////////////////////////////////////////////////////////////////
// Name:        FFMPEGCodec.h
// Purpose:     Manager to get the codec names/params.
// Author:      Orkhan Aliyev
// Created:     2022-04-15
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////
#ifndef FFMPEGCODEC_H
#define FFMPEGCODEC_H

// Standard C Lib
#include <vector>
// FFmpeg
extern "C"{
#include <libavcodec/avcodec.h>// Avoid including this
}

namespace NNBSSFFMPEG
{
	inline __declspec(thread) void* iterate_data = NULL;

	inline const char* Encoder_GetNextCodecName()
	{
		AVCodec* current_codec = const_cast<AVCodec*>(av_codec_iterate(&iterate_data));
		while (current_codec != NULL)
		{
			if (!av_codec_is_encoder(current_codec))
			{
				current_codec = const_cast<AVCodec*>(av_codec_iterate(&iterate_data));
				continue;
			}

			return current_codec->name;
		}
		return "";
	}

	inline const char* Encoder_GetFirstCodecName()
	{
		iterate_data = NULL;
		return Encoder_GetNextCodecName();
	}

	inline std::vector<std::string> Encoder_GetList()
	{
		static std::vector<std::string> s_Encoders;
		if (s_Encoders.empty())
		{
			s_Encoders.clear();
			while (Encoder_GetNextCodecName() != "")
			{
				const std::string m_Cur = Encoder_GetNextCodecName();
				if (!m_Cur.empty())
					s_Encoders.emplace_back(m_Cur);
			}
		}
		if (s_Encoders.empty())// second check. if still empty, add default
		{
			s_Encoders.emplace_back("Default");
		}
		return s_Encoders;
	}
}

#endif // ! FFMPEGCODEC_H