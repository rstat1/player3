#define EGL_API_FB
#include <SDL_egl.h>

#include <App.h>
#include <player/Player.h>
#include <ui/native/EventHub.h>
#include <player/chat/ChatService.h>
#include <platform/PlatformManager.h>
#include <ui/web/UIServer.h>
#include <ui/native/threading/UIWorkerHost.h>

#include <ui/native/NativeUIHost.h>

using namespace player3::ui;
using namespace player3::chat;
using namespace player3::player;
using namespace player3::platform;

namespace app
{
	std::shared_ptr<App> App::ref;

	void App::StartApp()
	{
		NEW_TASK0(initTask, App, App::Get(), OnInitComplete);
		this->taskRunner = new TaskRunner();
		this->taskRunner->Init("PlayerApp", initTask);
		this->taskRunner->Start();
	}
	void App::OnInitComplete()
	{
		//SDL_SetHint("SDL_PE_GFX_RESOLUTION", "1920x1080");
		SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO);

		//InfluxDBClient::Get()->InitIFDBConnection("http://ingest.telemetry.m/", "player3_metrics", "Player3");

		PlatformManager::Get()->InitPlatformInterface();
		UIServer* uiserve = new UIServer();
		UIWorkerHost::Get()->Init();
		ChatService::Get()->InitChatService();
		Player::Get()->InitPlayer();
		ChatService::Get()->ConnectToTwitchIRC("jqokdnx4u23c80xslwyfflnkcidy5i", "rstat1");
	}
	void App::ChatUIEvent(void* args)
	{
		ThreadedEventHandlerArgs* eventArgs = (ThreadedEventHandlerArgs*)args;
		eventArgs->eventHandler.handler(eventArgs->args);
	}
	void App::SwitchChats(void* args)
	{
		const char* newChat = (const char*)args;
		ChatService::Get()->JoinChannel(newChat);
	}
}