/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/logging.h>
#include <platform/steamlink/SteamLinkDecoder.h>

namespace player3 { namespace platform
{
	SteamLinkDecoder::SteamLinkDecoder()
	{
		slVideoContext = SLVideo_CreateContext();
		SLVideo_SetLogFunction(VideoLogFunc, nullptr);
		SLVideo_SetLogLevel(k_ESLVideoLogDebug);
		videoStream = SLVideo_CreateStream(slVideoContext, k_ESLVideoFormatH264, false);
	}
	void SteamLinkDecoder::DecoderReset()
	{
		if (videoStream != nullptr) { SLVideo_FreeStream(videoStream); }
		videoStream = SLVideo_CreateStream(slVideoContext, k_ESLVideoFormatH264, false);
	}
	void SteamLinkDecoder::DecoderShutdown()
	{
		if (videoStream != nullptr) { SLVideo_FreeStream(videoStream); }
		SLVideo_FreeContext(slVideoContext);
	}
	bool SteamLinkDecoder::DecodeVideoFrame(uint8_t* data, int size)
	{
		if (SLVideo_BeginFrame(videoStream, size) != 0)
		{
			Log("SLVideo", "beginframe failed");
			return false;
		}
		if (SLVideo_WriteFrameData(videoStream, data, size) != 0)
		{
			Log("SLVideo", "writeframe failed");
			return false;
		}
		if (SLVideo_SubmitFrame(videoStream) != 0)
		{
			Log("SLVideo", "submitframe failed");
			return false;
		}
		return true;
	}
	void SteamLinkDecoder::VideoLogFunc(void* pContext, ESLVideoLog eLogLevel, const char *pszMessage)
	{
		Log("SLVideo", pszMessage);
	}
	int SteamLinkDecoder::GetAudioSampleCount() { return 1024; }
}}