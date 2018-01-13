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
#include <base/Utils.h>
#include <base/common.h>

#define EVENTARGS(name, type) \
	struct name##EventArgs \
	{\
		public:\
			type arg##name;\
			type GetValue() { return arg##name; }\
			name##EventArgs(type value##name) { arg##name = value##name; } \
	};

#define HANDLER [&](void* args)
#define HANDLE_EVENT(name, asTask, onThread, handler) EventHandler name(asTask, onThread, handler); \
			EventHub::Get()->RegisterEventHandler(#name, name);
#define TRIGGER_EVENT(name, args) EventHub::Get()->TriggerEvent(#name, args);
namespace player3 { namespace ui
{
	struct EventHandler
	{
		public:
			bool runHandlerAsTask = false;
			const char* owningThread = "PlayerApp";
			std::function<void(void*)> handler;
			EventHandler(bool runAsTask, const char* onThread, std::function<void(void*)> handlerFunc) :
				runHandlerAsTask(runAsTask), owningThread(onThread), handler(handlerFunc)
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
			std::map<const char*, EventHandlers, base::utils::CStringComparator> eventHandlers;

		SINGLETON(EventHub)
	};
}}

#endif