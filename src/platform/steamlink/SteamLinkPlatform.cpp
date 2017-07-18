/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <SDL.h>
#include <string.h>
#include <base/logging.h>
#include <ui/native/rendering/NanoVGRenderer.h>
#include <platform/steamlink/SteamLinkPlatform.h>

using namespace player3::ui;

namespace player3 { namespace platform
{
	SteamLinkPlatform::SteamLinkPlatform()
	{
		InitVideoDecoder();
		SLVideo_GetDisplayResolution(slVideoContext, &screenW, &screenH);
	}
	std::vector<int> SteamLinkPlatform::GetScreenSize()
	{
		std::vector<int> size { screenW, screenH };
		return size;
	}
	void SteamLinkPlatform::DecoderReset()
	{
		if (videoStream != nullptr) { SLVideo_FreeStream(videoStream); }
		SLVideo_FreeContext(slVideoContext);
		InitVideoDecoder();
		//videoStream = SLVideo_CreateStream(slVideoContext, k_ESLVideoFormatH264, false);
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
		if (slVideoContext != nullptr)
		{
			if (infoOverlay != nullptr) { SLVideo_FreeOverlay(infoOverlay); }
			infoOverlay = SLVideo_CreateOverlay(slVideoContext, w, h);
			SLVideo_SetOverlayDisplayArea(infoOverlay, 0, 0, (((double)this->w) / screenW) , (((double)this->h) / screenH));
		}
	}
	void SteamLinkPlatform::ShowOverlay(void* pixels, int pitch)
	{
		int dstPitch;
		uint32_t* pixelBuf;
		uint32_t* surface = static_cast<uint32_t*>(pixels);
		if (infoOverlay != nullptr && slVideoContext != nullptr)
		{
			SLVideo_GetOverlayPixels(infoOverlay, &pixelBuf, &dstPitch);

			if (pixelBuf == nullptr) { this->CreateOverlay(this->w, this->h); }

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
	}
	int SteamLinkPlatform::GetAudioSampleCount() { return 1024; }
	int SteamLinkPlatform::GetQueuedVideo() { return SLVideo_GetQueuedVideoMS(videoStream); }
	void SteamLinkPlatform::InitVideoDecoder()
	{
		slVideoContext = SLVideo_CreateContext();
		SLVideo_SetLogLevel(k_ESLVideoLogDebug);
		SLVideo_SetLogFunction(VideoLogFunc, nullptr);
		videoStream = SLVideo_CreateStream(slVideoContext, k_ESLVideoFormatH264, false);
	}
}}