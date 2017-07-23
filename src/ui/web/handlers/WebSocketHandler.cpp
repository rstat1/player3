/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <App.h>
#include <cpr/cpr.h>
#include <base/Utils.h>
#include <base/common.h>
#include <player/Player.h>
#include <player/chat/ChatService.h>
#include <platform/PlatformManager.h>
#include <ui/web/handlers/WebSocketHandler.h>
#include <base/threading/dispatcher/DispatcherTypes.h>

using namespace app;
using namespace cpr;
using namespace base::utils;
using namespace player3::chat;
using namespace base::threading;
using namespace player3::player;
using namespace player3::platform;

namespace player3 { namespace ui
{
	WebSocketHandler::WebSocketHandler()
	{
		this->InitMessageMap();
	}
	void WebSocketHandler::InitMessageMap()
	{
		messageTypeMappings["ID"] = MessageType::ID;
		messageTypeMappings["MUTE"] = MessageType::MUTE;
		messageTypeMappings["STOP"] = MessageType::STOP;
		messageTypeMappings["START"] = MessageType::START;
		messageTypeMappings["USHER"] = MessageType::USHER;
		messageTypeMappings["ACCESS"] = MessageType::ACCESS;
		messageTypeMappings["EXIT"] = MessageType::EXIT;
		messageTypeMappings["JOIN"] = MessageType::JOIN;
		messageTypeMappings["STREAMINFO"] = MessageType::STREAMINFO;
	}
 	void WebSocketHandler::onDisconnect(uWS::WebSocket<SERVER>* connection)
	{
		this->clientConnections.erase(connection);
	}
	void WebSocketHandler::onData(uWS::WebSocket<SERVER>* connection, char* data, int length)
	{
		std::string resp("");
		std::string receivedMessage(data);
		receivedMessage = receivedMessage.substr(0, length);
		int endOfCmd = receivedMessage.find_first_of(":");
		std::string command = receivedMessage.substr(0, endOfCmd);
		std::string args = receivedMessage.replace(0, endOfCmd + 1, "");

		switch (messageTypeMappings[command])
		{
			case MessageType::ID:
				if (args != "RAD")
				{
					if (this->clientID == "") { this->clientID = std::move(args); }
					this->clientConnections.insert(connection);
					this->UpdatePlayerState();
				}
				break;
 			case MessageType::ACCESS:
 				connection->send(this->GetAccessToken(args).c_str());
 				break;
			case MessageType::USHER:
 				connection->send(this->GetUsherToken(args).c_str());
				break;
			case MessageType::MUTE:
				Player::Get()->Mute();
				this->isMuted = !this->isMuted;
				this->UpdatePlayerState();
				break;
			case MessageType::STOP:
				this->isPlaying = false;
				//POST_TASK(stopStream, "PlayerApp");
				Player::Get()->Stop();
				ChatService::Get()->LeaveCurrentChannel();
				this->UpdatePlayerState();
				break;
			case MessageType::START:
				this->isPlaying = true;
				Player::Get()->StartStream(std::move(args));
				this->UpdatePlayerState();
				break;
			case MessageType::JOIN:
				ChatService::Get()->JoinChannel(std::move(args.c_str()));
				break;
			case MessageType::PLAYERSTATE:
				break;
			case MessageType::EXIT:
				PlatformManager::Get()->GetPlatformInterface()->DecoderShutdown();
				exit(0);
				break;
			case MessageType::STREAMINFO:
				std::vector<std::string> infoBits = split(args, ';');
				Player::Get()->SetBitrate(atoi(infoBits[0].c_str()));
				Player::Get()->SetFPS(atoi(infoBits[1].c_str()));
				break;
 		}
 	}
	std::string WebSocketHandler::GetAccessToken(std::string url)
	{
		std::string cprResp("");
		cpr::Response resp = cpr::Get(cpr::Url(url), cpr::Header{{"Client-Id", this->clientID.c_str()}});
		if (resp.status_code == 200)
		{
			cprResp.append("ACCESS:");
			cprResp.append(resp.text);
			return cprResp;
		}
		else
		{
			Log("UIServer", "GetAccessToken failed: %i %s", resp.status_code, resp.text.c_str());
			return std::to_string(resp.status_code);
		}
	}
	std::string WebSocketHandler::GetUsherToken(std::string url)
	{
		std::string cprResp("");
		cpr::Response resp = cpr::Get(cpr::Url(url));
		if (resp.status_code == 200)
		{
			cprResp.append("USHER:");
			cprResp.append(resp.text);
			return cprResp;
		}
		else
		{
			Log("UIServer", "GetUsherToken failed: %i %s", resp.status_code, resp.text.c_str());
			return std::to_string(resp.status_code);
		}
	}
	void WebSocketHandler::UpdatePlayerState()
	{
		std::string resp("PLAYERSTATE:");
		this->isMuted == true ? resp.append("Muted;") : resp.append("NotMuted;");
		this->isPlaying == true ? resp.append("Playing") : resp.append("Stopped");
 		for (auto *con : this->clientConnections) { con->send(resp.c_str()); }
	}
}}