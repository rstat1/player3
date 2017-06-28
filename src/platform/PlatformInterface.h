/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef P3DECINT
#define P3DECINT

#include <vector>

namespace player3 { namespace platform
{
	class PlatformInterface
	{
		public:
			virtual void DecoderReset() = 0;
			virtual int GetQueuedVideo() = 0;
			virtual void DecoderShutdown() = 0;
			virtual int GetAudioSampleCount() = 0;
			virtual std::vector<int> GetScreenSize() = 0;
			virtual void CreateOverlay(int w, int h) = 0;
			virtual void ShowOverlay(void* pixels, int pitch) = 0;
			virtual bool DecodeVideoFrame(uint8_t* frameData, int size) = 0;
	};
}}

#endif