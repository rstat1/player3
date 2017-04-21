/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef DESKDEC
#define DESKDEC

#include <platform/PlatformInterface.h>

namespace player3 { namespace platform
{
	class DesktopPlatform : public PlatformInterface
	{
		public:
			void DecoderReset() override {}
			void DecoderShutdown() override {}
			int GetAudioSampleCount() override { return 2048; }
			bool DecodeVideoFrame(uint8_t* data, int size) override { return true; }
	};
}}

#endif