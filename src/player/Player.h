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
#include <platform/Platforms.h>
#include <player/infooverlay/InfoOverlay.h>

using namespace base::threading;
using namespace player3::overlay;
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
			void Stop();
			void Decode();
			void StartDecodeThread();
			void InitSDLAudio(int sampleRate);
			int ProcessAudio(AVPacket* pkt);

			static void LogDeltaMemory(const char* action);
			static uint32_t RefreshOverlay(uint32_t interval, void* opaque);
			static void SDLAudioCallback(void* userdata, uint8_t* stream, int len);

			InfoOverlay* overlay;
			InternalPlayerState* state;
			static double lastMemoryUse;
			static PlatformInterface* platformInterface;
	};
}}

#endif