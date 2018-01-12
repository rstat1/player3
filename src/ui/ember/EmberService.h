/*
* Copyright (c) 2018 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef EMBRSVC
#define EMBRSVC

#include <string>
#include <uWS/src/uWS.h>
#include <base/common.h>
#include <ui/native/PropertyMacros.h>

using namespace uWS;

namespace player3 { namespace ember
{
	enum MessageType
	{
		MUTE,
		STOP,
		EXIT,
		START,
		INIT,
		DISCONNECT,
		CHATUISTATE,
		QUALITYCHANGE,
	};
	struct EmberAuthenticatedEventArgs
	{
		public:
			std::string DeviceName;
			EmberAuthenticatedEventArgs(std::string devName) { DeviceName.assign(devName); }
	};
	struct EmberStreamEventArgs
	{
		public:
			std::string streamURL;
			EmberStreamEventArgs(std::string URL) { streamURL.assign(URL); }
	};
	class EmberService
	{
		public:
			void Init();
			void ConnectToEmber();
			PROPERTY(EmberConnectURL, std::string)
			PROPERTY(EmberDeviceName, std::string)
			PROPERTY(EmberClientToken, std::string)
			PROPERTY(EmberTwitchToken, std::string)
			PROPERTY(EmberWebSocketURL, std::string)

		private:
			void GetClientToken();
			void RegisterEvents();
			void MessageReceived(WebSocket<CLIENT>* connection, char* data, int length);

			Hub emberHub;
			WebSocket<CLIENT>* emberClientSocket;
			std::string deviceID = "";
			std::string emberClientID = "vyUujyJIhAwLkMPbm4MnVOONYaW7wAo2";
			std::string emberClientSecret = "DePPn78AC7L3jloHXb6mSykU2jVAqzUpHTeAahpU8yLEyk05";
			std::map<std::string, MessageType> messageTypeMappings;

		SINGLETON(EmberService)
	};
}}

#endif