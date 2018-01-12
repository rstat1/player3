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

		EmberService::Get()->SetEmberWebSocketURL("ws://192.168.1.12:1999/ws");
		EmberService::Get()->SetEmberConnectURL("http://192.168.1.12:1999/api/ember/client/connect");
		EmberService::Get()->Init();

		PlatformManager::Get()->InitPlatformInterface();
		UIWorkerHost::Get()->Init();
		ChatService::Get()->InitChatService();
		Player::Get()->InitPlayer();
//		ChatService::Get()->ConnectToTwitchIRC("mue8x854f0ehqob9df2uxn4vh205x3", "rstat1");

		EventHandler UIInitComplete(true, "UI", [&](void* args) {
			this->EmberEventHandlers();
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
	void App::EmberEventHandlers()
	{
		EventHandler EmberAuthEvent(true, "UI", [&](void* args) {
			//TODO: Home screen
			writeToLog("show home");
			NativeUIHost::Get()->RenderScreen("Home", std::map<std::string, boost::any>{}, false);
		});
		EventHandler EmberActivationRequired(true, "UI", [&](void* args) {
			writeToLog("activation needed");
			this->ShowActivateScreen((EmberAuthenticatedEventArgs*)args);
		});
		EventHandler EmberDisconnected(true, "UI", [&](void* args) {
			writeToLog("disconnected");
			this->ShowActivateScreen((EmberAuthenticatedEventArgs*)args);
		});
		EventHub::Get()->RegisterEventHandler("EmberAuthenticated", EmberAuthEvent);
		EventHub::Get()->RegisterEventHandler("EmberDisconnected", EmberDisconnected);
		EventHub::Get()->RegisterEventHandler("EmberNeedsActivation", EmberActivationRequired);
		EmberService::Get()->ConnectToEmber();
	}
	void App::ShowActivateScreen(EmberAuthenticatedEventArgs* eventArgs)
	{
		std::map<std::string, boost::any> bindings;
		bindings["DeviceName"] = eventArgs->DeviceName;
		NativeUIHost::Get()->RenderScreen("Activation", bindings, false);
	}

}