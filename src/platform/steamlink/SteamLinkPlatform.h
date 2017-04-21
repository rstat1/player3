/*
* Copyright (c) 2017 The Incredibly Big Red Robot
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
	class SteamLinkDecoder : public PlatformInterface
	{
		public:
			SteamLinkDecoder();
			void DecoderReset() override;
			void DecoderShutdown() override;
			int GetAudioSampleCount() override;
			bool DecodeVideoFrame(uint8_t* data, int size) override;
		private:
			static void VideoLogFunc(void* pContext, ESLVideoLog eLogLevel, const char *pszMessage);

			CSLVideoStream* videoStream;
			CSLVideoContext* slVideoContext;
	};
}}

#endif