/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <player/Player.h>
#include <player/PlayerApp.h>

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

		std::string streamURL(URL);
		Player* player = new Player();
		player->StartStream(streamURL);

		return nullptr;
	}
}}