/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <PlayerApp.h>
#include <ui/native/EventHub.h>
#include <base/threading/dispatcher/DispatcherTypes.h>

using namespace base::threading;
using namespace player3::player;

namespace player3 { namespace ui
{
	std::shared_ptr<EventHub> EventHub::ref;

	void EventHub::RegisterEvent(const char* name)
	{
		if (this->eventHandlers.find(name) == this->eventHandlers.end())
		{
			Log("EventHub", "Registering event %s", name);
			this->eventHandlers.insert(std::make_pair(name, EventHandlers()));
		}
	}
	void EventHub::RegisterEventHandler(const char* name, EventHandler handler)
	{
		Log("EventHub", "Registering eventhandler for event %s", name);
		this->eventHandlers[name].push_back(handler);
	}
	void EventHub::TriggerEvent(const char* name, void* args)
	{
		if (this->eventHandlers.find(name) != this->eventHandlers.end())
		{
			Log("EventHub", "Triggering eventhandler for event %s", name);
			EventHandlers handlers = this->eventHandlers[name];
			for (EventHandler eh : handlers)
			{
				if (eh.runHandlerAsTask)
				{
					ThreadedEventHandlerArgs *teArgs = new ThreadedEventHandlerArgs(eh, args);
					NEW_TASK1(EventDispatch, PlayerApp, PlayerApp::Get(), PlayerApp::ChatUIEvent, teArgs);
					POST_TASK(EventDispatch, eh.owningThread);
				}
				else { eh.handler(args); }
			}
		}
	}
}}