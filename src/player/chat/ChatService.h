/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef CHATSERV
#define CHATSERV

#include <uWS/src/uWS.h>
#include <base/common.h>
#include <player/chat/ChatUI.h>
#include <base/threading/dispatcher/DispatcherTypes.h>

using namespace uWS;
using namespace base::threading;

namespace player3 { namespace chat
{
	struct ConnectionDetails
	{
		public:
			const char* token;
			const char* username;
			ConnectionDetails(const char* Token, const char* Username)
				: token(Token), username(Username)
			{}
	};
	struct ChatMessage
	{
		public:
			const char* emotes;
			const char* sender;
			const char* message;
			bool emotesOnly = false;
			const char* senderColor;
	};
	class ChatService
	{
		public:
			void InitChatService();
			void LeaveCurrentChannel();
			void JoinChannel(const char* channel);
			void ConnectToTwitchIRC(const char* token, const char* user);
		private:
			void ParseChatMessage(std::vector<std::string> rawMessage);
			void MessageReceived(WebSocket<CLIENT>* connection, char* data, int length);

			Hub chatHub;
			ChatUI* chatUI;
			std::string currentChannel;
			WebSocket<CLIENT>* twitchChat;


		SINGLETON(ChatService)
	};
}}

#endif