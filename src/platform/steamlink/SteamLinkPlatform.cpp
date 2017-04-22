/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <string.h>
#include <base/logging.h>
#include <platform/steamlink/SteamLinkPlatform.h>

namespace player3 { namespace platform
{
	SteamLinkPlatform::SteamLinkPlatform()
	{
		slVideoContext = SLVideo_CreateContext();
		SLVideo_SetLogFunction(VideoLogFunc, nullptr);
		SLVideo_SetLogLevel(k_ESLVideoLogDebug);
		videoStream = SLVideo_CreateStream(slVideoContext, k_ESLVideoFormatH264, false);
	}
	void SteamLinkPlatform::DecoderReset()
	{
		if (videoStream != nullptr) { SLVideo_FreeStream(videoStream); }
		videoStream = SLVideo_CreateStream(slVideoContext, k_ESLVideoFormatH264, false);
	}
	void SteamLinkPlatform::DecoderShutdown()
	{
		if (videoStream != nullptr) { SLVideo_FreeStream(videoStream); }
		SLVideo_FreeContext(slVideoContext);
	}
	bool SteamLinkPlatform::DecodeVideoFrame(uint8_t* data, int size)
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
	void SteamLinkPlatform::VideoLogFunc(void* pContext, ESLVideoLog eLogLevel, const char *pszMessage)
	{
		Log("SLVideo", pszMessage);
	}
	void SteamLinkPlatform::CreateOverlay(int w, int h)
	{
		this->w = w;
		this->h = h;
		if (infoOverlay != nullptr) { SLVideo_FreeOverlay(infoOverlay); }
		infoOverlay = SLVideo_CreateOverlay(slVideoContext, w, h);
	}
	void SteamLinkPlatform::ShowOverlay(void* pixels, int x, int y, int pitch)
	{
		int slPitch;
		uint32_t* pixelBuf;
		SLVideo_SetOverlayDisplayArea(infoOverlay, x, y, 1.0, 1.0);
		SLVideo_GetOverlayPixels(infoOverlay, &pixelBuf, &slPitch);

		memcpy(pixelBuf, pixels, pitch * this->h);

		SLVideo_ShowOverlay(infoOverlay);
	}
	int SteamLinkPlatform::GetAudioSampleCount() { return 1024; }
}}