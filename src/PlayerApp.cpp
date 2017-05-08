/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <PlayerApp.h>
#include <player/Player.h>
#include <ui/UIServer.h>

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

		UIServer* server = new UIServer();
		Player::Get()->InitPlayer();

		return nullptr;
	}
	TaskResult* PlayerApp::StopStream()
	{
		Log("PlayerApp", "stopstream");
		Player::Get()->Stop();

		return nullptr;
	}
}}