#define EGL_API_FB
#include <SDL_egl.h>

#include <App.h>
#include <player/Player.h>
#include <ui/native/EventHub.h>
#include <player/chat/ChatService.h>
#include <platform/PlatformManager.h>
#include <ui/web/UIServer.h>
#include <ui/native/threading/UIWorkerHost.h>
#include <ui/ember/EmberService.h>

#include <ui/native/NativeUIHost.h>

using namespace player3::ui;
using namespace player3::chat;
using namespace player3::ember;
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

		EmberService::Get()->SetEmberWebSocketURL("http://192.168.1.12:1999/ws");
		EmberService::Get()->SetEmberServiceURL("http://192.168.1.12:1999/api/ember/client/connect");
		EmberService::Get()->Init();

		PlatformManager::Get()->InitPlatformInterface();
        UIServer::Get()->Init();
		UIWorkerHost::Get()->Init();
		ChatService::Get()->InitChatService();
		Player::Get()->InitPlayer();
//		ChatService::Get()->ConnectToTwitchIRC("mue8x854f0ehqob9df2uxn4vh205x3", "rstat1");

		EventHandler UIInitComplete(true, "UI", [&](void* args) {
			NativeUIHost::Get()->RenderScreen("Activation", std::map<string, boost::any>{}, false);
			EventHandler EmberAuthEvent(true, "UI", [&](void* args) {
				EmberAuthenticatedEventArgs* eventArgs = (EmberAuthenticatedEventArgs*)args;
				std::map<std::string, boost::any> bindings;
				bindings["DeviceName"] = eventArgs->DeviceName;
				NativeUIHost::Get()->RenderScreen("Activation", bindings, false);
			});
			EventHub::Get()->RegisterEventHandler("EmberAuthenticated", EmberAuthEvent);
			EmberService::Get()->ConnectToEmber();
		});
		EventHub::Get()->RegisterEventHandler("UIWorkerThreadStarted", UIInitComplete);

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