/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <player/chat/ChatService.h>
#include <ui/native/NativeUIHost.h>

using namespace player3::ui;

namespace player3 { namespace chat
{
	void ChatUI::InitChatUI()
	{
		EventHandler connectedEvent(true, "PlayerApp", [&](void* args) {
			Log("ChatUI", "Connected...");

			std::string cl("ChatLocation");
			std::string li("ListItems");
			std::string msg("Connected to TwitchIRC");

			std::map<std::string, boost::any> bindings;
			bindings[cl] = AnchorPoint::BottomLeft;
			bindings[li] = msg;

			NativeUIHost::Get()->RenderLayout("ChatUI", bindings);
		});
		EventHandler msgReceivedEvent(true, "PlayerApp", [&](void* args) {
			ChatMessage* chatMessage = (ChatMessage*)args;
			Log("ChatUI", "message received. %s", chatMessage->message);
		});
		EventHub::Get()->RegisterEventHandler("Connected", connectedEvent);
		EventHub::Get()->RegisterEventHandler("MessageReceived", msgReceivedEvent);
	}
}}