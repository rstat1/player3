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
#include <uWS/src/uWS.h>

using namespace uWS;
// #include <seasocks/Server.h>
// #include <seasocks/WebSocket.h>

// using namespace seasocks;

namespace player3 { namespace ui
{
	enum MessageType
	{
		ID,
		MUTE,
		STOP,
		EXIT,
		JOIN,
		START,
		USHER,
		ACCESS,
		STREAMINFO,
		PLAYERSTATE
	};
 	class WebSocketHandler
 	{
 		public:
			WebSocketHandler();
			void onDisconnect(uWS::WebSocket<SERVER>* connection);
			void onData(uWS::WebSocket<SERVER>* connection, char* data, int length);
		private:
			void InitMessageMap();
			std::string GetUsherToken(std::string url);
			std::string GetAccessToken(std::string url);
			void UpdatePlayerState();

			std::string clientID;
			bool isPlaying = false;
			bool isMuted = false;
 			std::set<uWS::WebSocket<SERVER>*> clientConnections;
			std::map<std::string, MessageType> messageTypeMappings;
	};
}}

#endif