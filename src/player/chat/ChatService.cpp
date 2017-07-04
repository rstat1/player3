/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <mutex>
#include <thread>
#include <base/Utils.h>
#include <player/chat/ChatService.h>

using namespace base::utils;

namespace player3 { namespace chat
{
	std::shared_ptr<ChatService> ChatService::ref;

	void ChatService::ConnectToTwitchIRC(const char* token, const char* user)//void* connectionDetails)
	{
		Log("Chat", "%s %s", token, user);

		chatHub.onConnection([token, user, this](WebSocket<CLIENT> *ws, HttpRequest req) {
			this->twitchChat = ws;
			std::string tokenStr("PASS oauth:");
			std::string username("NICK ");
			tokenStr.append(token);
			username.append(user);
			Log("Chat", "Connected...");
			ws->send("CAP REQ :twitch.tv/tags");
			ws->send(tokenStr.c_str());
			ws->send(username.c_str());
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
		chatHub.onPing([&](WebSocket<CLIENT>* ws, char* msg, size_t len) {
			Log("Chat", "PING-PONG");

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
		this->twitchChat->send(partCmd.c_str());
	}
	void ChatService::MessageReceived(uWS::WebSocket<CLIENT>* connection, char* data, int length)
	{
		std::string receivedMessage(data);
		receivedMessage = receivedMessage.substr(0, length);

		std::vector<std::string> msgParts = split(receivedMessage, ';');
		if (msgParts.size() >= 12)
		{
			std::vector<std::string> actualMessage = split(msgParts[msgParts.size() -1], ':');
			std::string sender(msgParts[2].c_str());
			sender.replace(0, 13, "");
			//TODO: Move this out of the log file.
			Log("Chat", "%s: %s", sender.c_str(), actualMessage[actualMessage.size() -1].c_str());
		}
		else
		{
			if (msgParts[0] == "PING :tmi.twitch.tv")
			{
				chatHub.getDefaultGroup<CLIENT>().broadcast("PONG :tmi.twitch.tv", 19, OpCode::TEXT);
			}
			Log("Chat", "msgparts length = %i, %s", msgParts.size(), msgParts[0].c_str());
		}
	}
}}