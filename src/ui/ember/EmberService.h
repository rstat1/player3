/*
* Copyright (c) 2018 An Incredibly Big Red Robot
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
#include <ui/ember/EmberServiceCommon.h>

using namespace uWS;

namespace player3 { namespace ember
{
	class EmberService
	{
		public:
			void Init();
			void ConnectToEmber();
			void ActuallyConnectToEmber();
			void SendStateChange(EmberStateChangeEventArgs* newState);

			PROPERTY(EmberIsMuted, bool)
			PROPERTY(EmberIsPlaying, bool)
			PROPERTY(EmberIsConnected, bool)
			PROPERTY(ConnectionAttempts, int)
			PROPERTY(EmberConnectURL, std::string)
			PROPERTY(EmberDeviceName, std::string)
			PROPERTY(EmberClientToken, std::string)
			PROPERTY(EmberTwitchToken, std::string)
			PROPERTY(EmberWebSocketURL, std::string)
			PROPERTY(EmberTwitchUsername, std::string)
		private:
			void GetClientToken();
			void RegisterEvents();
			void OnEC3Disconnect();
			void SendVersionInfo();
			void SendDisconnectMessage();
			void ParseInitMessage(std::string args, bool activate);
			static uint32_t ReconnectAttempt(uint32_t interval, void* opaque);
			void MessageReceived(WebSocket<CLIENT>* connection, char* data, int length);

			Hub emberHub;
			bool connecting = false;
			bool connected = false;
			std::string deviceID = "";
			WebSocket<CLIENT>* emberClientSocket;
			std::string emberClientID = "22zpJN4k66hgb5sBsvqnzV239IUOCux9";
			std::string emberClientSecret = "znXPhM0PLyHA5jpqs09tluZLJ9s2e092r3kJWS3KztGreV5a";
			std::map<std::string, MessageType> messageTypeMappings;

		SINGLETON(EmberService)

	};
}}

#endif