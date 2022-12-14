/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef STEAMDEC
#define STEAMDEC

#include <SLVideo.h>
#include <platform/PlatformInterface.h>

namespace player3 { namespace platform
{
	class SteamLinkPlatform : public PlatformInterface
	{
		public:
			SteamLinkPlatform();
			void DecoderReset() override;
			int GetQueuedVideo() override;
			void DecoderShutdown() override;
			int GetAudioSampleCount() override;
			void SetVideoFPS(int fps) override;
			void CreateOverlay(int w, int h) override;
			std::vector<int> GetScreenSize() override;
			void SetVideoBitrate(int bitrate) override;
			void ShowOverlay(void* pixels, int pitch) override;
			bool DecodeVideoFrame(uint8_t* data, int size) override;
		private:
			static void VideoLogFunc(void* pContext, ESLVideoLog eLogLevel, const char *pszMessage);
			void InitVideoDecoder();

			int w, h, screenW, screenH;
			CSLVideoStream* videoStream;
			CSLVideoContext* slVideoContext;
			CSLVideoOverlay* infoOverlay = nullptr;
	};
}}

#endif