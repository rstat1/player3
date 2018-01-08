/*
* Copyright (c) 2018 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <thread>
#include <iterator>
#include <cpr/cpr.h>
#include <json/json.h>
#include <base/Utils.h>
#include <boost/filesystem.hpp>
#include <ui/native/EventHub.h>
#include <ui/ember/EmberService.h>
#include <ui/native/elements/LayoutManagerBase.h>

using namespace player3::ui;
using namespace base::utils;
using namespace boost::filesystem;

namespace player3 { namespace ember
{
	std::shared_ptr<EmberService> EmberService::ref;
	void EmberService::Init()
	{
		EventHub::Get()->RegisterEvent("EnberActivated");
		EventHub::Get()->RegisterEvent("EmberConnected");
		EventHub::Get()->RegisterEvent("EmberAuthenticated");

		path devIDFile("deviceid");
		if (exists(devIDFile) == false)
		{
			std::ofstream devIDFile("deviceid");
			deviceID = GenerateRandomString(32);
			devIDFile << deviceID;
			devIDFile.close();
		}
		else
		{
			std::ifstream devIDFile("deviceid");
			devIDFile >> this->deviceID;
			devIDFile.close();
		}
		Log("Ember::Init", deviceID.c_str());
	}
	void EmberService::ConnectToEmber()
	{
		this->GetClientToken();
		emberHub.onConnection([&](WebSocket<CLIENT> *ws, HttpRequest req) {
			this->emberClientSocket = ws;
		});

	}
	void EmberService::GetClientToken()
	{
		EmberAuthenticatedEventArgs* args = new EmberAuthenticatedEventArgs("");

		Json::Reader reader;
		Json::Value root, accessResp;
		root["emberClientID"] = this->emberClientID;
		root["emberClientSecret"] = this->emberClientSecret;

		cpr::Response resp = cpr::Post(cpr::Url("http://192.168.1.12:1999/api/ember/client/connect"), cpr::Header{
				{"ember-device-id", this->deviceID},
		}, cpr::Body{
			root.toStyledString()
		});
		if (resp.status_code == 200) {
			if (reader.parse(resp.text.c_str(), accessResp))
			{
				this->SetEmberDeviceName(accessResp["emberDeviceName"].asString());
				this->SetEmberClientToken(accessResp["emberClientToken"].asString());

				args->DeviceName.assign(accessResp["emberDeviceName"].asString());

				EventHub::Get()->TriggerEvent("EmberAuthenticated", args);
			}
		} else {
			Log("EmberService", "GetClientToken failed: %i %s", resp.status_code, resp.text.c_str());
		}
	}
}}