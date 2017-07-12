/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef EVNTHUB
#define EVNTHUB

#include <map>
#include <vector>
#include <functional>
#include <base/common.h>

namespace player3 { namespace ui
{
	struct EventHandler
	{
		public:
			bool runHandlerAsTask = false;
			const char* owningThread = "PlayerApp";
			std::function<void(void*)> handler;
			EventHandler(bool runAsTask, const char* onThread, std::function<void(void*)> handlerFunc) :
				runHandlerAsTask(runAsTask), owningThread("PlayerApp"), handler(handlerFunc)
			{}
	};
	typedef std::vector<EventHandler> EventHandlers;
	struct ThreadedEventHandlerArgs : std::enable_shared_from_this<ThreadedEventHandlerArgs>
	{
		public:
			EventHandler eventHandler;
			void* args;
			ThreadedEventHandlerArgs(EventHandler handler, void* eventArgs) :
				eventHandler(handler), args(eventArgs)
			{}
	};
	class EventHub
	{
		public:
			void RegisterEvent(const char* name);
			void RegisterEventHandler(const char* name, EventHandler handler);
			void TriggerEvent(const char* name, void* args);
		private:
			std::map<const char*, EventHandlers> eventHandlers;

		SINGLETON(EventHub)
	};
}}

#endif