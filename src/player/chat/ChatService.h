/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef CHATUI
#define CHATUI

#include <uWS/src/uWS.h>
#include <base/common.h>
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
	class ChatService
	{
		public:
			void LeaveCurrentChannel();
			void JoinChannel(const char* channel);
			void ConnectToTwitchIRC(const char* token, const char* user);
		private:
			Hub chatHub;
			std::mutex sendBlocker;
			std::string currentChannel;
			WebSocket<CLIENT>* twitchChat;

			void MessageReceived(WebSocket<CLIENT>* connection, char* data, int length);

		SINGLETON(ChatService)
	};
}}

#endif