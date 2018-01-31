/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef CHATUI
#define CHATUI

#include <ui/native/EventHub.h>
#include <ui/native/NativeUIHost.h>
#include <ui/native/PropertyMacros.h>

using namespace player3::ui;

namespace player3 { namespace chat
{
	EVENTARGS(ChatUIPosition, std::string);
	class ChatUI
	{
		public:
			void InitChatUI();
			PROPERTY(ChatPosition, AnchorPoint);
		private:
			void HandleChatPositionEvent(ChatUIPositionEventArgs* args);
	};
}}

#endif