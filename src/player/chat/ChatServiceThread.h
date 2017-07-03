/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef CHTSRVTHRD
#define CHTSRVTHRD

#include <player/chat/ChatService.h>
#include <base/threading/common/Thread.h>

using namespace base::threading;

namespace player3 { namespace chat
{
	class ChatServiceThread : public Thread
	{
		public:
			void Init() override;

		SINGLETON(ChatServiceThread)
	};
}}

#endif