/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef PLAY
#define PLAY

#include <SDL.h>
#include <string>
#include <vector>
#include <base/common.h>
#include <base/threading/common/ConditionVariable.h>

extern "C"
{
	#include <libavutil/avutil.h>
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswresample/swresample.h>
};

using namespace base::threading;

namespace player3 { namespace player
{
	enum PlayerStatus
	{
		Playing,
		Stopped
	};
	struct Data
	{
		public:
			uint8_t* data;
			int size;
			Data(uint8_t* value, int dataSize) : data(std::move(value)), size(std::move(dataSize)) {}
	};
	struct AudioState
	{
		public:
			AVCodecContext* aCtx;
			AVCodec* audioCodec;
			SwrContext* convertCtx;
	};
	struct InternalPlayerState
	{
		public:
			PlayerStatus status;
			int videoIdx, audioIdx;
			std::string currentURL;
			AVFormatContext* format;
			std::vector<Data> video;
			std::vector<Data> audio;
			AudioState* audioDecodeState;
			SDL_AudioDeviceID audioDevice;
			ConditionVariable* bufferSignal;
	};
	class Player
	{
		public:
			Player();
			void StartStream(std::string url);
		private:
			void Play();
			void Decode();
			void InitDecode();
			void StartDecodeThread();
			void SetRefreshTimer(int delay);
			void InitSDLAudio(int sampleRate);
			int ProcessAudio(AVPacket* pkt, uint8_t*& buffer);

			static uint32_t RefreshTimer(uint32_t interval, void* opaque);

			InternalPlayerState* state;
	};
}}

#endif