/*
* Copyright (c) 2016 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef SDLPMP 
#define SDLPMP

#include <base/common.h>
#include <base/threading/dispatcher/DispatcherMessagePump.h>

namespace base { namespace threading
{
	class DispatcherTask;

	class MessagePumpSDL : public DispatcherMessagePump
	{
		public:
			MessagePumpSDL(const char* winIDExt) : DispatcherMessagePump(winIDExt) {}
			~MessagePumpSDL() {}
			void MakeMessagePump(bool isTaskRunner) override;
			void MakeMessagePump(DispatcherTask* InitTask) override;
			void StartMessageLoop(bool isTaskRunner) override;
			void PostMessageToThread(const char* thread, DispatcherTask *task, bool isTaskRunner) override;
			void RegisterMessageHandler(MessageReceiver* recv);
		private:
			void Close();
	};
}}

#endif