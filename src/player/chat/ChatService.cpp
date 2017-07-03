/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <thread>
#include <base/Utils.h>
#include <player/chat/ChatService.h>

using namespace base::utils;

namespace player3 { namespace chat
{
	std::shared_ptr<ChatService> ChatService::ref;

	TaskResult* ChatService::ConnectToTwitchIRC(void* connectionDetails)
	{
		ConnectionDetails* details = (ConnectionDetails*)connectionDetails;

		std::string token("PASS oauth:");
		std::string username("NICK ");
		token.append(details->token);
		username.append(details->username);

		Log("Chat", "Init chat overaly...");
		uWS::Hub chatHub;
		chatHub.onConnection([&](WebSocket<CLIENT> *ws, HttpRequest req) {
			this->twitchChat = ws;
			ws->send("CAP REQ :twitch.tv/tags");
			ws->send(token.c_str());
			ws->send(username.c_str());
		});
		chatHub.onError([&](void* user) {
			Log("Chat", "connect failed");
		});
		chatHub.onDisconnection([&](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
			Log("Chat", "Disconnected %s", message);
		});
		chatHub.onMessage([&](WebSocket<CLIENT>* ws, char* msg, size_t len, OpCode code) {
			this->MessageReceived(ws, msg, len);
		});
		chatHub.connect("wss://irc-ws.chat.twitch.tv", nullptr, {}, 1000);
		chatHub.run();

		return nullptr;
	}
	TaskResult* ChatService::JoinChannel(void* channel)
	{
		const char* channelName = (const char*)channel;
		Log("Chat", "Joing channel %s", channelName);

		this->currentChannel = "JOIN #";
		this->currentChannel.append(channelName);
		this->twitchChat->send(this->currentChannel.c_str());
		this->currentChannel.assign(channelName);

		return nullptr;
	}
	TaskResult* ChatService::LeaveCurrentChannel()
	{
		std::string partCmd("PART #");
		partCmd.append(this->currentChannel);
		this->twitchChat->send(partCmd.c_str());

		return nullptr;
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
			Log("Chat", "%s: %s", sender.c_str(), actualMessage[actualMessage.size() -1].c_str());
		}
		else { Log("Chat", "msgparts length = %i", msgParts.size()); }
	}
}}