/*
* Copyright (c) 2018 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <SDL.h>
#include <thread>
#include <iterator>
#include <cpr/cpr.h>
#include <json/json.h>
#include <base/Utils.h>
#include <boost/filesystem.hpp>
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
		messageTypeMappings["EXIT"] = MessageType::EXIT;
		messageTypeMappings["START"] = MessageType::START;
		messageTypeMappings["UNMUTE"] = MessageType::MUTE;
		messageTypeMappings["DISCONNECT"] = MessageType::DISCONNECT;
		messageTypeMappings["DEACTIVATE"] = MessageType::DEACTIVATE;
		messageTypeMappings["CHATUICHANGE"] = MessageType::CHATUISTATE;
		messageTypeMappings["QUALITYCHANGE"] = MessageType::QUALITYCHANGE;
		Log("Ember::Init", deviceID.c_str());
	}
	void EmberService::RegisterEvents()
	{
		EVENT("EmberConnected");
		EVENT("EmberConnecting");
		EVENT("EmberDisconnected");
		EVENT("EmberConnectFailed");
		EVENT("EmberAuthenticated");
		EVENT("EmberNeedsActivation");

		EVENT("EmberStopStream");
		EVENT("EmberMuteStream");
		EVENT("EmberStartStream");

		EVENT("EmberChatAction");
		EVENT("EmberStateChange");

		HANDLE_EVENT(EmberStateChange, true, "PlayerApp", HANDLER {
			EmberStateChangeEventArgs* newState = (EmberStateChangeEventArgs*)args;
			Log("ember", "EmberStateChange, playing = %s, muted = %s", newState->GetFirstArgument().c_str(), newState->GetSecondArgument().c_str());

			EmberService::Get()->SetEmberIsMuted(newState->GetSecondArgument() == "muted");
			EmberService::Get()->SetEmberIsPlaying(newState->GetFirstArgument() == "playing");
		})
	}
	void EmberService::ConnectToEmber()
	{
		emberHub.onConnection([&](WebSocket<CLIENT> *ws, HttpRequest req) {
			this->emberClientSocket = ws;
			this->connecting = false;
			this->SetConnectionAttempts(0);
			this->SetEmberIsConnected(true);
		});
		emberHub.onError([&](void* user) {
			Log("ember", "connect failed (default)");
			if (this->connecting == false) {
				this->ActuallyConnectToEmber();
				this->OnEC3Disconnect();
			}
		});
		emberHub.onDisconnection([&](WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
			Log("ember", "Disconnected %s", message);
			this->SetEmberIsConnected(false);
			this->ActuallyConnectToEmber();
			this->OnEC3Disconnect();
		});
		emberHub.onMessage([&](WebSocket<CLIENT>* ws, char* msg, size_t len, OpCode code) {
			this->MessageReceived(ws, msg, len);
		});
		this->ActuallyConnectToEmber();
	}
	void EmberService::ActuallyConnectToEmber()
	{
		this->GetClientToken();

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

				args = new EmberAuthenticatedEventArgs(accessResp["emberDeviceName"].asString());

				if (accessResp["emberActivated"].asBool() == false) { TRIGGER_EVENT(EmberNeedsActivation, args); }
				else { TRIGGER_EVENT(EmberAuthenticated, args); }
			}
			this->SetEmberIsConnected(true);
			this->SetConnectionAttempts(0);
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

		std::vector<std::string> infoBits;
		EmberAuthenticatedEventArgs* authEvent;

		switch (messageTypeMappings[command])
		{
			case MUTE:
				TRIGGER_EVENT(EmberMuteStream, nullptr)
				break;
			case START:
				Log("ember::START", "%s", receivedMessage.c_str());
				TRIGGER_EVENT(EmberStartStream, new EmberStreamEventArgs(args))
				break;
			case STOP:
				Log("ember::STOP", "%s", receivedMessage.c_str());
				TRIGGER_EVENT(EmberStopStream, nullptr)
				break;
			case EXIT:
				TRIGGER_EVENT(EmberStopStream, nullptr)
				this->SendDisconnectMessage();
				sleep(2);
				exit(0);
				break;
			case INIT:
				Log("ember::init", "%s", receivedMessage.c_str());
				infoBits = split(args, ';');
				if (infoBits[0] == "muted") { TRIGGER_EVENT(EmberMuteStream, nullptr) }
				if (infoBits[1] == "playing") { TRIGGER_EVENT(EmberStartStream, nullptr) }
				this->SetEmberTwitchToken(args);
				TRIGGER_EVENT(EmberAuthenticated, nullptr)
				break;
			case UNMUTE:
				TRIGGER_EVENT(EmberUnmuteStream, nullptr)
				break;
			case DEACTIVATE:
				Log("ember::deactivate", "deactivate, %s", args.c_str());
				authEvent = new EmberAuthenticatedEventArgs(args);
				TRIGGER_EVENT(EmberNeedsActivation, authEvent)
				break;
			case CHATUISTATE:
				break;
			case QUALITYCHANGE:
				break;
		}
	}
	void EmberService::OnEC3Disconnect()
	{
		Log("embersvc::OnEC3Disconnect", "%i", EmberService::Get()->GetConnectionAttempts());
		this->connecting = true;
		if (!this->GetEmberIsConnected())
		{
			TRIGGER_EVENT(EmberConnecting, nullptr);
			SDL_AddTimer(5000, EmberService::ReconnectAttempt, nullptr);
		}
	}
	void EmberService::SendStateChange(EmberStateChangeEventArgs *newState)
	{
		std::string message(this->deviceID);
		message.append(" STATECHANGE:");
		message.append(newState->GetFirstArgument());
		message.append(";");
		message.append(newState->GetSecondArgument());

		this->emberClientSocket->send(message.c_str());
	}
	void EmberService::SendDisconnectMessage()
	{
		std::string message(this->deviceID);
		message.append(" DISCONNECT:");
		this->emberClientSocket->close(1000, message.c_str(), message.length());
	}
	uint32_t EmberService::ReconnectAttempt(uint32_t interval, void* opaque)
	{
		int attempts = EmberService::Get()->GetConnectionAttempts();
		if (EmberService::Get()->GetEmberIsConnected()) { return 0; }
		else
		{
			EmberService::Get()->ActuallyConnectToEmber();
			EmberService::Get()->SetConnectionAttempts(attempts += 1);
			if (attempts > 9)
			{
				Log("embersvc::reconnect", "connected failed...");
				TRIGGER_EVENT(EmberConnectFailed, new EmberConnectingEventArgs(attempts));
				return 0;
			}
			else { return interval; }
		}
	}
}}