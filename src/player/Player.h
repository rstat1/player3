/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef PLAY
#define PLAY

#include <thread>
#include <memory>
#include <string>
#include <vector>
#include <base/common.h>
#include <player/VideoPlayerBase.h>
#include <base/threading/common/ConditionVariable.h>
#include <platform/PlatformManager.h>

using namespace std;
using namespace base::threading;
using namespace player3::platform;

namespace player3 { namespace player
{
	class Player
	{
		public:
			Player();
			void Stop();
			void Mute();
			void InitPlayer();
			void SetFPS(int fps);
			void SetBitrate(int bitrate);
			void StartStream(std::string url);
			InternalPlayerState* GetPlayerState() { return state; }
		private:
			void Play();
			void Decode();
			void InitOverlay();
			void ResetPlayer();
			double GetAudioClock();
			bool CheckPlayerState();
			void StartDecodeThread();
			void StartPlaybackThread();
			int ProcessAudio(AVPacket* pkt);
			void InitSDLAudio(int sampleRate, int channels);
			void UpdateInfoOverlay(InternalPlayerState* state);
			static void SigTermHandler(int signum);
			static void SetRefreshTimer(int delay, Player* state);
			static uint32_t RefreshStream(uint32_t interval, void* opaque);
			static uint32_t RefreshOverlay(uint32_t interval, void* opaque);
			static void SDLAudioCallback(void* userdata, uint8_t* stream, int len);

			//ConnectionDetails* details;
			std::thread decode, play;
			InternalPlayerState* state;
			static double lastMemoryUse;
			static PlatformInterface* platformInterface;

		SINGLETON(Player)
	};
}}

#endif