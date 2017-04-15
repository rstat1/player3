/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef STEAMDEC
#define STEAMDEC

#include <SLVideo.h>
#include <platform/DecoderInterface.h>

namespace player3 { namespace plaform
{
	class SteamLinkDecoder : public DecoderInterface
	{
		public:
			SteamLinkDecoder();
			void Reset() override;
			void Shutdown() override;
			void DecodeVideoFrame(uint8_t* data, int size) override;
		private:
			CSLVideoStream* videoStream;
			CSLAudioStream* audioStream;
			CSLVideoContext* slVideoContext;
			CSLAudioContext* slAudioContext;
	};
}}

#endif