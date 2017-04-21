/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef P3DECINT
#define P3DECINT

namespace player3 { namespace platform
{
	class PlatformInterface
	{
		public:
			virtual void DecoderReset() = 0;
			virtual int GetAudioSampleCount() = 0;
			virtual void DecoderShutdown() = 0;
			virtual bool DecodeVideoFrame(uint8_t* frameData, int size) = 0;
	};
}}

#endif