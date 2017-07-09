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
		EventHub::Get()->RegisterEvent("MessageReceived");

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
			this->MessageReceived(ws, msg, len);
		});
		std::thread chatHubRunner([&]{
			chatHub.connect("wss://irc-ws.chat.twitch.tv", nullptr, {}, 1000);
			chatHub.run();
		});
		chatHubRunner.detach();
	}
	void ChatService::JoinChannel(const char* channel)
	{
		const char* channelName = (const char*)channel;
		Log("Chat", "Joing channel %s", channelName);

		this->currentChannel = "JOIN #";
		this->currentChannel.append(channelName);
		chatHub.getDefaultGroup<CLIENT>().broadcast(this->currentChannel.c_str(),
													this->currentChannel.length(), OpCode::TEXT);

		this->currentChannel.assign(channelName);
	}
	void ChatService::LeaveCurrentChannel()
	{
		std::string partCmd("PART #");
		partCmd.append(this->currentChannel);
		chatHub.getDefaultGroup<CLIENT>().broadcast(partCmd.c_str(), partCmd.length(), OpCode::TEXT);
	}
	void ChatService::MessageReceived(uWS::WebSocket<CLIENT>* connection, char* data, int length)
	{
		std::string receivedMessage(data);
		receivedMessage = receivedMessage.substr(0, length);

		std::vector<std::string> msgParts = split(receivedMessage, ';');
		if (msgParts.size() >= 12)
		{
			this->ParseChatMessage(msgParts);
		}
		else
		{
			if (msgParts[0].find("PING :tmi") != std::string::npos)
			{
				chatHub.getDefaultGroup<CLIENT>().broadcast("PONG :tmi.twitch.tv", 20, OpCode::TEXT);
			}
		}
	}
	void ChatService::ParseChatMessage(std::vector<std::string> rawMessage)
	{
		ChatMessage* msg = new ChatMessage();
		bool emoteOnly = false;
		std::vector<std::string> actualMessage = split(rawMessage[rawMessage.size() -1], ':');
		std::string sender(rawMessage[2].c_str());

		sender.replace(0, 13, "");
		if (rawMessage[3] == "emote-only=1")
		{
			emoteOnly = true;
			msg->emotes = rawMessage[4].c_str();
		}
		else { msg->emotes = rawMessage[3].c_str(); }

		boost::trim(actualMessage[actualMessage.size() -1]);

		msg->emotesOnly = emoteOnly;
		msg->sender.assign(sender);
		msg->message.assign(actualMessage[actualMessage.size() -1]);

		EventHub::Get()->TriggerEvent("MessageReceived", msg);
	}
}}