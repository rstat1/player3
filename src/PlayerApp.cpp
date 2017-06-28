/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <PlayerApp.h>
#include <player/Player.h>
#include <ui/web/UIServer.h>
#include <ui/native/NativeUIHost.h>
#include <platform/PlatformManager.h>

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
	TaskResult* PlayerApp::OnInitComplete()
	{
		Log("PlayerApp", "init complete");
		SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO);
		
		UIServer* server = new UIServer();
		PlatformManager::Get()->InitPlatformInterface();
		Player::Get()->InitPlayer();
		NativeUIHost::Get()->InitUIHost();

		return nullptr;
	}
	TaskResult* PlayerApp::StopStream()
	{
		Log("PlayerApp", "stopstream");
		Player::Get()->Stop();

		return nullptr;
	}
}}