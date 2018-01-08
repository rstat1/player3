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
	struct EmberAuthenticatedEventArgs
	{
		public:
			std::string DeviceName;
			EmberAuthenticatedEventArgs(std::string devName)
			{
				DeviceName.assign(devName);
			}
	};
	class EmberService
	{
		public:
			void Init();
			void ConnectToEmber();

			PROPERTY(EmberDeviceName, std::string)
			PROPERTY(EmberClientToken, std::string)

		private:
			void GetClientToken();
			std::string GenerateDeviceID();
			void MessageReceived(WebSocket<CLIENT>* connection, char* data, int length);

			Hub emberHub;
			WebSocket<CLIENT>* emberClientSocket;
			std::string deviceID = "";
			std::string deviceName = "";
			std::string emberClientToken = "";
			std::string emberClientID = "4hfPWeaEoQW17VskOnfVgUhx5snOto4r";
			std::string emberClientSecret = "5BOQK5GCvebQTdwcGIUQFJXm7GozReVWy0DG31bBc0DgbzlS";

		SINGLETON(EmberService)
	};
}}

#endif