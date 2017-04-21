/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef PLAYBASE
#define PLAYBASE

#include <SDL.h>
#include <queue>
#include <base/threading/common/ConditionVariable.h>

extern "C"
{
	#include <libavutil/avutil.h>
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswresample/swresample.h>
};

#define REFRESH_EVENT SDL_USEREVENT
#define AUDIO_INIT SDL_USEREVENT + 1

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
			int size;
			double pts;
			uint8_t* data;
			Data() {}
			Data(uint8_t* value, int dataSize, double timestamp) :
				size(std::move(dataSize)),
				pts(std::move(timestamp))
			{
				data = static_cast<uint8_t*>(calloc(1, dataSize));
				memmove(data, value, dataSize);
			}
			void DeleteData()
			{
				free(data);
			}
	};
	struct AudioState
	{
		public:
			AVCodec* audioCodec;
			AVCodecContext* aCtx;
			AVStream audioStream;
			SwrContext* convertCtx;
	};
	struct InternalPlayerEvents
	{
		public:
			uint32_t RefreshEventID;
			uint32_t AudioInitEventID;
	};
	struct InternalPlayerState
	{
		public:
			double streamStart;
			PlayerStatus status;
			int videoIdx, audioIdx;
			std::string currentURL;
			AVFormatContext* format;
			std::queue<Data> video;
			std::queue<Data> audio;
			AudioState* audioDecodeState;
			SDL_AudioDeviceID audioDevice;
			ConditionVariable* bufferSignal;
			double videoTimeBase, audioTimeBase;
	};
}}

#endif