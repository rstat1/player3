/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef WEBSOCKHAND
#define WEBSOCKHAND

#include <map>
#include <set>
#include <seasocks/Server.h>
#include <seasocks/WebSocket.h>

using namespace seasocks;

namespace player3 { namespace ui
{
	enum MessageType
	{
		ID,
		MUTE,
		STOP,
		START,
		USHER,
		ACCESS,
		REMOTEAUDIO,
		PLAYERSTATE
	};
	class WebSocketHandler : public WebSocket::Handler
	{
		public:
			WebSocketHandler();
			virtual void onConnect(WebSocket* connection);
			virtual void onDisconnect(WebSocket* connection);
			virtual void onData(WebSocket* connection, const char* data);
		private:
			void InitMessageMap();
			std::string GetUsherToken(std::string url);
			std::string GetAccessToken(std::string url);
			void UpdatePlayerState();

			std::string clientID;
			bool isPlaying = false;
			bool isMuted = false;
			std::set<WebSocket*> clientConnections;
			std::map<std::string, MessageType> messageTypeMappings;
	};
}}

#endif