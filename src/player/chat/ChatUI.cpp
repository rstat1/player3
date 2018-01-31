/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/common.h>
#include <player/chat/ChatService.h>
#include <ui/ember/EmberService.h>

using namespace player3::ui;
using namespace player3::ember;

namespace player3 { namespace chat
{
	void ChatUI::InitChatUI()
	{
		EventHandler connectedEvent(true, "UI", [&](void* args) {
			std::string message("Connected to TwitchIRC");

			std::map<std::string, boost::any> bindings;
			bindings["ChatLocation"] = this->GetChatPosition();
			bindings["ListItems"] = message;

			NativeUIHost::Get()->RenderScreen("ChatUI", bindings, false);
		});
		EventHandler msgReceivedEvent(true, "UI", [&](void* args) {
			ChatMessage* chatMessage = (ChatMessage*)args;

			std::string received(chatMessage->sender);
			received.append(":").append(" ").append(chatMessage->message);

			std::map<std::string, boost::any> bindings;
			bindings["ListItems"] = received;
			bindings["ChatLocation"] = this->GetChatPosition();

			NativeUIHost::Get()->RenderScreen("ChatUI", bindings, false);
		});
		EventHandler channelChangedEvent(true, "UI", [&](void* args) {
			std::map<std::string, boost::any> bindings;
			bindings["ListItems"] = "";
			NativeUIHost::Get()->RenderScreen("ChatUI", bindings, true);
		});
		HANDLE_EVENT(ChatUIPositionChange, true, "UI", HANDLER {
			this->HandleChatPositionEvent((ChatUIPositionEventArgs*)args);
		})

		//EventHub::Get()->RegisterEventHandler("Connected", connectedEvent);
		EventHub::Get()->RegisterEventHandler("MessageReceived", msgReceivedEvent);
	}
	void ChatUI::HandleChatPositionEvent(ChatUIPositionEventArgs* args)
	{
		if (args->GetValue() == "top-left") { this->SetChatPosition(AnchorPoint::TopLeft); }
		else if (args->GetValue() == "top-right") { this->SetChatPosition(AnchorPoint::TopRight); }
		else if (args->GetValue() == "bottom-left") { this->SetChatPosition(AnchorPoint::BottomLeft); }
		else if (args->GetValue() == "bottom-right") { this->SetChatPosition(AnchorPoint::BottomRight); }

		if (EmberService::Get()->GetEmberIsPlaying())
		{
			std::map<std::string, boost::any> bindings;
			bindings["ChatLocation"] = this->GetChatPosition();
			NativeUIHost::Get()->RenderScreen("ChatUI", bindings, false);
		}

	}
}}