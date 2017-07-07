/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <player/chat/ChatUI.h>
#include <player/chat/ChatService.h>

namespace player3 { namespace chat
{
	void ChatUI::InitChatUI()
	{
		EventHub::Get()->RegisterEventHandler("MessageReceived", [&](void* args) {
			ChatMessage* message = (ChatMessage*)args;
			
		});
	}
}}