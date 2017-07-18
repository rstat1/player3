/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <mutex>
#include <thread>
#include <base/Utils.h>
#include <ui/native/EventHub.h>
#include <player/chat/ChatService.h>
#include <boost/algorithm/string/trim.hpp>

using namespace player3::ui;
using namespace base::utils;

namespace player3 { namespace chat
{
	std::shared_ptr<ChatService> ChatService::ref;

	void ChatService::InitChatService()
	{
		EventHub::Get()->RegisterEvent("Connected");
		EventHub::Get()->RegisterEvent("ChnagedChannel");
		EventHub::Get()->RegisterEvent("MessageReceived");
		this->currentChannel = "";
		this->chatUI = new ChatUI();
		this->chatUI->InitChatUI();
	}
	void ChatService::ConnectToTwitchIRC(const char* token, const char* user)
	{
		chatHub.onConnection([token, user, this](WebSocket<CLIENT> *ws, HttpRequest req) {
			this->twitchChat = ws;
			std::string tokenStr("PASS oauth:");
			std::string username("NICK ");
			tokenStr.append(token);
			username.append(user);
			ws->send("CAP REQ :twitch.tv/tags");
			ws->send(tokenStr.c_str());
			ws->send(username.c_str());
			EventHub::Get()->TriggerEvent("Connected", nullptr);
		});
		chatHub.onError([&](void* user) {
			Log("Chat", "connect failed");
		});
		chatHub.onDisconnection([&](WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
			Log("Chat", "Disconnected %s", message);
		});
		chatHub.onMessage([&](WebSocket<CLIENT>* ws, char* msg, size_t len, OpCode code) {
			MICROPROFILE_SCOPEI("CHAT", "MessageRecv", MP_PINK);
			this->MessageReceived(ws, msg, len);
			MicroProfileFlip(0);
		});
		std::thread chatHubRunner([&]{
			chatHub.connect("wss://irc-ws.chat.twitch.tv", nullptr, {}, 1000);
			chatHub.run();
		});
		chatHubRunner.detach();
	}
	void ChatService::JoinChannel(const char* channel)
	{
		Log("Chat", "Joing channel %s", channel);
		if (this->currentChannel != "")
		{
			this->LeaveCurrentChannel();
			EventHub::Get()->TriggerEvent("ChangedChannel", nullptr);
		}
		this->currentChannel = "JOIN #";
		this->currentChannel.append(channel);
		this->currentChannel = str_tolower(this->currentChannel);
		chatHub.getDefaultGroup<CLIENT>().broadcast(this->currentChannel.c_str(),
													this->currentChannel.length(), OpCode::TEXT);


		this->currentChannel.assign(channel);
		this->currentChannel.assign(str_tolower(this->currentChannel).c_str());
	}
	void ChatService::LeaveCurrentChannel()
	{
		std::string partCmd("PART #");
		partCmd.append(this->currentChannel);
		chatHub.getDefaultGroup<CLIENT>().broadcast(partCmd.c_str(), partCmd.length(), OpCode::TEXT);
		this->currentChannel = "";
	}
	void ChatService::MessageReceived(uWS::WebSocket<CLIENT>* connection, char* data, int length)
	{
		std::string receivedMessage(data);
		receivedMessage = receivedMessage.substr(0, length);
		std::vector<std::string> msgParts = split(receivedMessage, ';');

		if (msgParts.size() >= 12)
		{
			this->ParseChatMessage(receivedMessage);
		}
		else
		{
			if (msgParts[0].find("PING :tmi") != std::string::npos)
			{
				chatHub.getDefaultGroup<CLIENT>().broadcast("PONG :tmi.twitch.tv", 20, OpCode::TEXT);
			}
		}

		if (msgParts[0].find("JOIN #") != std::string::npos) { Log("Chat", "%s", receivedMessage.c_str()); }
		if (msgParts[0].find("PART #") != std::string::npos) { Log("Chat", "%s", receivedMessage.c_str()); }
	}
	void ChatService::ParseChatMessage(std::string rawMessage)
	{
		MICROPROFILE_SCOPEI("CHAT", "ParseChatMessage", MP_ORANGE);

		bool emoteOnly = false;
		std::string sender, color;
		ChatMessage* msg = new ChatMessage();
		std::vector<std::string> rawParts = split(rawMessage, ';');
		std::vector<std::string> actualMessage = split(rawParts[rawParts.size() -1], ':');
		for (std::string part : rawParts)
		{
			if (part.find("display-name=") != std::string::npos) { sender.assign(part.c_str()); }
			else if (part.find("color=") != std::string::npos) { color.assign(part.c_str()); }
			else if (part.find("emotes=") != std::string::npos) {}
			else if (part.find("emote-only=1") != std::string::npos) { emoteOnly = true; }
		}

		boost::trim(sender);
		boost::trim(color);

		if (color != "") { color.replace(0, 7, ""); }
		if (sender != "") { sender.replace(0, 13, ""); }
		// else
		// {
		// 	std::vector<std::string> senderBits = split(rawParts[rawParts.size() - 1], '!');
		// 	for (auto part : senderBits)
		// 	{
		// 		Log("ChatService", "%s", part.c_str());
		// 	}
		// 	sender.assign(senderBits[0].replace(0, 12, "").c_str());
		// }

		if (sender == "")
		{
			std::vector<std::string> senderBits = split(rawParts[rawParts.size() - 1], '!');
			sender.assign(senderBits[0].replace(0, 12, "").c_str());
			for (auto s : rawParts)
			{
				Log("ChatService", "%s", s.c_str());
			}
			Log("ChatService", "Message has no sender %s", rawParts[rawParts.size() - 1].c_str());
		}
		boost::trim(actualMessage[actualMessage.size() - 1]);
		msg->emotesOnly = emoteOnly;
		msg->sender.assign(sender);
		msg->senderColor = color;
		msg->message.assign(actualMessage[actualMessage.size() - 1]);
		EventHub::Get()->TriggerEvent("MessageReceived", msg);
	}
}}