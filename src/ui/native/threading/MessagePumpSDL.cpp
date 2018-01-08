/*
* Copyright (c) 2016 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <SDL.h>
#include <base/common.h>
#include <ui/native/NativeUIHost.h>
#include <ui/native/threading/MessagePumpSDL.h>
#include <base/threading/dispatcher/DispatcherTypes.h>

using namespace player3::ui;

namespace base { namespace threading
{
	void MessagePumpSDL::MakeMessagePump(bool isTaskRunner)
	{
		Log("MessagePumpSDL", "MessagePumpSDL::MakeMessagePump()");

		// this->SDLEventID = SDL_RegisterEvents(1);
		// if (this->SDLEventID == ((uint32_t)-1)) {Log("MessagePumpSDL", "Didn't get an SDL event ID"); }

		this->StartMessageLoop(isTaskRunner);
	}
	void MessagePumpSDL::MakeMessagePump(DispatcherTask* InitTask)
	{
		NOTIMPLEMENTED("MessagePumpSDL::MakeMessagePump(DispatcherTask)");
		this->initTask = InitTask;
		this->MakeMessagePump(true);
	}
	void MessagePumpSDL::StartMessageLoop(bool isTaskRunner)
	{
		DispatcherTask* task;
		bool exitLoop = false;
		SDL_Event event;
		if (this->initTask != nullptr)
		{
			this->initTask->Invoke(false);
			this->initTask = nullptr;
		}
		while(!exitLoop && SDL_WaitEvent(&event))
		{
			switch(event.type)
			{
				case SDL_USEREVENT:
					task = (DispatcherTask*)event.user.data1;
					if (task->HasCallback()) { task->InvokeWithCallback(!task->HasArguments()); }
					else { task->Invoke(!task->HasArguments()); }
					break;
				case SDL_QUIT:
					this->Close();
					exitLoop = true;
					break;
				default:
					NativeUIHost::Get()->SDLEvent(event);
					// SDLInterface::Get()->HandleSDLEvent(event);
			}

		}
	}
	void MessagePumpSDL::PostMessageToThread(const char* thread, DispatcherTask *task, bool isTaskRunner)
	{
		SDL_Event newTask;
		SDL_memset(&newTask, 0, sizeof(newTask));
		newTask.type = SDL_USEREVENT;
		newTask.user.data1 = task;
		SDL_PushEvent(&newTask);
	}
	void MessagePumpSDL::RegisterMessageHandler(MessageReceiver* recv)
	{
		NOTIMPLEMENTED("MessagePumpSDL::MakeMessagePump(DispatcherTask)");
	}
	void MessagePumpSDL::Close()
	{
		// NEW_TASK0(QuitClose, SkylightApp, SkylightApp::Get(), ExitApp);
		// POST_TASK(QuitClose, "App");
		// Log("MessagePumpSDL", "TODO: Don't exit here. Do exit from managed bits.");
		exit(0);
	}
}}