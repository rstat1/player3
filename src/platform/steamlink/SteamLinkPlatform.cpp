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
		SLVideo_SetLogLevel(k_ESLVideoLogDebug);
		SLVideo_SetLogFunction(VideoLogFunc, nullptr);
		videoStream = SLVideo_CreateStream(slVideoContext, k_ESLVideoFormatH264, false);
		SLVideo_GetDisplayResolution(slVideoContext, &screenW, &screenH);
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
		SLVideo_SetOverlayDisplayArea(infoOverlay, 0, 0, (((double)this->w) / screenW) , (((double)this->h) / screenH));
	}
	void SteamLinkPlatform::ShowOverlay(void* pixels, int pitch)
	{
		int dstPitch;
		uint32_t* pixelBuf;
		uint32_t* surface = static_cast<uint32_t*>(pixels);
		SLVideo_GetOverlayPixels(infoOverlay, &pixelBuf, &dstPitch);

		pitch /= sizeof(*surface);
		dstPitch /= sizeof(*pixelBuf);
		for(int row = 0; row < this->h; ++row)
		{
			memcpy(pixelBuf, surface, this->w*sizeof(*surface));
			surface += pitch;
			pixelBuf += dstPitch;
		}

		SLVideo_ShowOverlay(infoOverlay);
	}
	int SteamLinkPlatform::GetAudioSampleCount() { return 1024; }
}}