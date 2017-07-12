#include <App.h>
#include <player/Player.h>
#include <ui/native/EventHub.h>
#include <ui/native/NativeUIHost.h>
#include <player/chat/ChatService.h>
#include <platform/PlatformManager.h>
#include <ui/web/UIServer.h>

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
		SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO);
		UIServer* uiserve = new UIServer();
		PlatformManager::Get()->InitPlatformInterface();
		NativeUIHost::Get()->InitUIHost();
		ChatService::Get()->InitChatService();
		Player::Get()->InitPlayer();
		ChatService::Get()->ConnectToTwitchIRC("jqokdnx4u23c80xslwyfflnkcidy5i", "rstat1");
		//csx62qos1qay8eoxqrhe0cvf05m4yh
	}
	void App::ChatUIEvent(void* args)
	{
		ThreadedEventHandlerArgs* eventArgs = (ThreadedEventHandlerArgs*)args;
		eventArgs->eventHandler.handler(eventArgs->args);
	}
}