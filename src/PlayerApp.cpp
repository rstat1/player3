/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <PlayerApp.h>
#include <player/Player.h>
#include <ui/web/UIServer.h>
#include <ui/native/EventHub.h>
#include <ui/native/NativeUIHost.h>
#include <platform/PlatformManager.h>
#include <player/chat/ChatService.h>

using namespace player3::chat;
using namespace player3::ui;

namespace player3 { namespace player
{
	std::shared_ptr<PlayerApp> PlayerApp::ref;

	PlayerApp::PlayerApp() {}
	void PlayerApp::Start()
	{
		NEW_TASK0(initTask, PlayerApp, PlayerApp::Get(), OnInitComplete);
		this->taskRunner = new TaskRunner();
		this->taskRunner->Init("PlayerApp", initTask);
		this->taskRunner->Start();
	}
	void PlayerApp::OnInitComplete()
	{
		Log("PlayerApp", "init complete");
		SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO);

		UIServer* server = new UIServer();
		PlatformManager::Get()->InitPlatformInterface();
		NativeUIHost::Get()->InitUIHost();
		ChatService::Get()->InitChatService();
		Player::Get()->InitPlayer();
		//TODO: Not hard-coded oauth token and username.
		ChatService::Get()->ConnectToTwitchIRC("csx62qos1qay8eoxqrhe0cvf05m4yh", "rstat1");
	}
	TaskResult* PlayerApp::StopStream()
	{
		Log("PlayerApp", "stopstream");
		Player::Get()->Stop();

		return nullptr;
	}
	void PlayerApp::ChatUIEvent(void* args)
	{
		ThreadedEventHandlerArgs* eventArgs = (ThreadedEventHandlerArgs*)args;
		eventArgs->eventHandler.handler(eventArgs->args);
	}
	void PlayerApp::ChatMessageEvent(std::shared_ptr<void> handler)
	{
		ThreadedEventHandlerArgs* eventArgs = (ThreadedEventHandlerArgs*)handler.get();

	}
}}