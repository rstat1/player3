/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef DESKDEC
#define DESKDEC

#include <SDL.h>
#include <stdint.h>
#include <platform/PlatformInterface.h>

namespace player3 { namespace platform
{
	class DesktopPlatform : public PlatformInterface
	{
		public:
			DesktopPlatform();
			void DecoderReset() override;
			void DecoderShutdown() override;
			int GetAudioSampleCount() override;
			void SetVideoFPS(int fps) override {}
			void CreateOverlay(int w, int h) override;
			std::vector<int> GetScreenSize() override;
			int GetQueuedVideo() override { return 0; }
			void SetVideoBitrate(int bitrate) override {}
			void ShowOverlay(void* pixels, int pitch) override;
			bool DecodeVideoFrame(uint8_t* data, int size) override;
		private:
			SDL_Window* win;
			int surfW, surfH;
			SDL_Renderer* render;
			SDL_Texture* overlayTexture = nullptr;

	};
}}

#endif