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
		this->RegisterEvents();
		messageTypeMappings["INIT"] = MessageType::INIT;
		messageTypeMappings["MUTE"] = MessageType::MUTE;
		messageTypeMappings["STOP"] = MessageType::STOP;
		messageTypeMappings["START"] = MessageType::START;
		messageTypeMappings["EXIT"] = MessageType::EXIT;
		messageTypeMappings["STREAMINFO"] = MessageType::STREAMINFO;
		Log("Ember::Init", deviceID.c_str());
	}
	void EmberService::RegisterEvents()
	{
		EventHub::Get()->RegisterEvent("EmberConnected");
		EventHub::Get()->RegisterEvent("EmberNeedsActivation");

		EventHub::Get()->RegisterEvent("EmberStopStream");
		EventHub::Get()->RegisterEvent("EmberMuteStream");
		EventHub::Get()->RegisterEvent("EmberStartStream");

		EventHub::Get()->RegisterEvent("EmberChatAction");
	}
	void EmberService::ConnectToEmber()
	{
		this->GetClientToken();
		emberHub.onConnection([&](WebSocket<CLIENT> *ws, HttpRequest req) {
			this->emberClientSocket = ws;
		});
		emberHub.onError([&](void* user) {
			Log("ember", "connect failed (default)");
		});
		emberHub.onDisconnection([&](WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
			Log("ember", "Disconnected %s", message);
		});
		emberHub.onMessage([&](WebSocket<CLIENT>* ws, char* msg, size_t len, OpCode code) {
			this->MessageReceived(ws, msg, len);
		});
		std::thread emberHubRunner([&]{
			emberHub.connect(this->GetEmberWebSocketURL(), nullptr, {
				{"Authorization", "Bearer " + this->GetEmberClientToken()},
				{"ember-device-id", this->deviceID},
			}, 1000);
			emberHub.run();
		});
		emberHubRunner.detach();
	}
	void EmberService::GetClientToken()
	{
		EmberAuthenticatedEventArgs* args = new EmberAuthenticatedEventArgs("");

		Json::Reader reader;
		Json::Value root, accessResp;
		root["emberClientID"] = this->emberClientID;
		root["emberClientSecret"] = this->emberClientSecret;

		cpr::Response resp = cpr::Post(cpr::Url(this->GetEmberConnectURL()), cpr::Header{
				{"ember-device-id", this->deviceID},
		}, cpr::Body{
			root.toStyledString()
		});
		if (resp.status_code == 200) {
			if (reader.parse(resp.text.c_str(), accessResp))
			{
				this->SetEmberDeviceName(accessResp["emberDeviceName"].asString());
				this->SetEmberClientToken(accessResp["emberAccessToken"].asString());

				args->DeviceName.assign(accessResp["emberDeviceName"].asString());

				if (accessResp["emberActivated"].asBool() == false)
				{
					EventHub::Get()->TriggerEvent("EmberNeedsActivation", args);
				}
				else
				{
					EventHub::Get()->TriggerEvent("EmberAuthenticated", args);
				}
			}
		} else {
			Log("EmberService", "GetClientToken failed: %i %s", resp.status_code, resp.text.c_str());
		}
	}
	void EmberService::MessageReceived(WebSocket<CLIENT> *connection, char *data, int length)
	{
		std::string resp("");
		std::string receivedMessage(data);
		receivedMessage = receivedMessage.substr(0, length);
		int endOfCmd = receivedMessage.find_first_of(":");
		std::string command = receivedMessage.substr(0, endOfCmd);
		std::string args = receivedMessage.replace(0, endOfCmd + 1, "");

		switch (messageTypeMappings[command])
		{
			case MUTE:
				EventHub::Get()->TriggerEvent("EmberMuteStream", nullptr);
				break;
			case START:
				EventHub::Get()->TriggerEvent("EmberStartStream", new EmberStreamEventArgs(args));
				break;
			case STOP:
				EventHub::Get()->TriggerEvent("EmberStopStream", nullptr);
				break;
			case STREAMINFO:
				break;
			case EXIT:
				exit(0);
				break;
			case PLAYERSTATE:
				break;
			case INIT:
				this->SetEmberTwitchToken(args);
				EventHub::Get()->TriggerEvent("EmberConnected", nullptr);
				break;
		}
	}
}}