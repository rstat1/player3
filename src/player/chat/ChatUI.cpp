/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/common.h>
#include <player/chat/ChatService.h>
#include <ui/native/NativeUIHost.h>

using namespace player3::ui;

namespace player3 { namespace chat
{
	void ChatUI::InitChatUI()
	{
		EventHandler connectedEvent(true, "PlayerApp", [&](void* args) {
			std::string message("Connected to TwitchIRC");

			std::map<std::string, boost::any> bindings;
			bindings["ChatLocation"] = AnchorPoint::BottomRight;
			bindings["ListItems"] = message;

			NativeUIHost::Get()->RenderLayout("ChatUI", bindings, false);
		});
		EventHandler msgReceivedEvent(true, "PlayerApp", [&](void* args) {
			ChatMessage* chatMessage = (ChatMessage*)args;

			std::string received(chatMessage->sender);
			received.append(":").append(" ").append(chatMessage->message);

			std::map<std::string, boost::any> bindings;
			bindings["ListItems"] = received;

			NativeUIHost::Get()->RenderLayout("ChatUI", bindings, false);
		});
		EventHandler channelChangedEvent(true, "PlayerApp", [&](void* args) {
			std::map<std::string, boost::any> bindings;
			bindings["ListItems"] = "";
			NativeUIHost::Get()->RenderLayout("ChatUI", bindings, true);
		});
		EventHub::Get()->RegisterEventHandler("Connected", connectedEvent);
		EventHub::Get()->RegisterEventHandler("MessageReceived", msgReceivedEvent);
	}
}}