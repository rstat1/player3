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

using namespace base::threading;

namespace streamlink { namespace player
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
	struct InternalPlayerState
	{
		public:
			PlayerStatus status;
			int videoIdx, audioIdx;
			std::string currentURL;
			std::vector<Data> video;
			std::vector<Data> audio;
			ConditionVariable* bufferSignal;
	};
	class Player
	{
		public:
			Player();
			void StartStream(std::string url);
		private:
			void Play();
			void StartDecodeThread();
			InternalPlayerState* state;
	};
}}

#endif