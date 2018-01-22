/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <memory>
#include <App.h>
#include <ui/native/EventHub.h>
//#include <base/threading/dispatcher/DispatcherTypes.h>

using namespace app;
//using namespace base::threading;

namespace player3 { namespace ui
{
	std::shared_ptr<EventHub> EventHub::ref;

	void EventHub::RegisterEvent(const char* name)
	{
		if (this->eventHandlers.find(name) == this->eventHandlers.end())
		{
			this->eventHandlers.insert(std::make_pair(name, EventHandlers()));
		}
	}
	void EventHub::RegisterEventHandler(const char* name, EventHandler handler)
	{
		this->eventHandlers[name].push_back(handler);
	}
	void EventHub::TriggerEvent(const char* name, void* args)
	{
		if (this->eventHandlers.find(name) != this->eventHandlers.end())
		{
			EventHandlers handlers = this->eventHandlers[name];
			for (EventHandler eh : handlers)
			{
				if (eh.runHandlerAsTask)
				{
					ThreadedEventHandlerArgs *teArgs = new ThreadedEventHandlerArgs(eh, std::move(args));
					NEW_TASK1(EventDispatch, App, App::Get(), App::ChatUIEvent, teArgs);
					POST_TASK(EventDispatch, eh.owningThread);
				}
				else { eh.handler(args); }
			}
		}
	}
}}