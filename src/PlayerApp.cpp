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

	PlayerApp::PlayerApp(const char* url)
	{
		NEW_TASK1(initTask, PlayerApp, PlayerApp::Get(), OnInitComplete, (void*)url);
		this->taskRunner = new TaskRunner();
		this->taskRunner->Init("PlayerApp", initTask);
		this->taskRunner->Start();
	}
	TaskResult* PlayerApp::OnInitComplete(void* url)
	{
		const char* URL = (const char*)url;
		Log("PlayerApp", "init complete");

		UIServer* server = new UIServer();

		if (URL != "")
		{
			std::string streamURL(URL);
			Player::Get()->InitPlayer();
			Player::Get()->StartStream(streamURL);
		}

		return nullptr;
	}
}}