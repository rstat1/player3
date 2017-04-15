/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef PLAY
#define PLAY

#include <string>
#include <vector>
#include <base/common.h>
#include <base/threading/common/ConditionVariable.h>
#include <player/VideoPlayerBase.h>
#include <platform/Decoders.h>

using namespace base::threading;
using namespace player3::platform;

namespace player3 { namespace player
{
	class Player
	{
		public:
			Player();
			void StartStream(std::string url);
		private:
			void Play();
			void Decode();
			void StartDecodeThread();
			void SetRefreshTimer(int delay);
			void InitSDLAudio(int sampleRate);
			int ProcessAudio(AVPacket* pkt);

			static uint32_t RefreshTimer(uint32_t interval, void* opaque);

			DecoderInterface* decoder;
			InternalPlayerState* state;
	};
}}

#endif