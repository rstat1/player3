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

		HANDLE_EVENT(UIWorkerThreadStarted, true, "UI", HANDLER {
			this->EmberEventHandlers();
		});
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
		HANDLE_EVENT(EmberAuthenticated, true, "UI", HANDLER {
			writeToLog("show home");
			NativeUIHost::Get()->RenderScreen("Home", std::map<std::string, boost::any>{}, false);
		})
		HANDLE_EVENT(EmberConnecting, true, "UI", HANDLER {
			this->EmberConnectingEvent((EmberConnectingEventArgs*)args);
		})
		HANDLE_EVENT(EmberDisconnected, true, "UI", HANDLER {
			writeToLog("disconnected");
			this->ShowActivateScreen((EmberAuthenticatedEventArgs*)args);
		})
		HANDLE_EVENT(EmberNeedsActivation, true, "UI", HANDLER {
			writeToLog("activation needed");
			this->ShowActivateScreen((EmberAuthenticatedEventArgs*)args);
		})
		HANDLE_EVENT(EmberStateChange, true, "UI", HANDLER {
			EmberStateChangeEventArgs* newState = (EmberStateChangeEventArgs*)args;
			Log("app", "EmberStateChange, playing = %s, muted = %s", newState->GetFirstArgument().c_str(), newState->GetSecondArgument().c_str());
			if (newState->GetFirstArgument() == "playing") {
				Log("app", "start stream");
				NativeUIHost::Get()->ClearScreen();
			} else if (newState->GetFirstArgument() == "notplaying") {
				NativeUIHost::Get()->RenderScreen("Home", std::map<std::string, boost::any>{}, false);
			}
			EmberService::Get()->SendStateChange(newState);
		})
		EmberService::Get()->ConnectToEmber();
	}
	void App::ShowActivateScreen(EmberAuthenticatedEventArgs* eventArgs)
	{
		std::map<std::string, boost::any> bindings;
		bindings["DeviceName"] = eventArgs->GetValue();
		NativeUIHost::Get()->RenderScreen("Activation", bindings, false);
	}
	void App::EmberConnectingEvent(EmberConnectingEventArgs* event)
	{
		int attempts = event->GetValue();
		std::map<std::string, boost::any> bindings;
		Log("app", "connection attempt %i", attempts);
		if (attempts != 0)
		{
			if (attempts < 4)
			{
				std::string attemptLabel("");
				attemptLabel.append("Attempt ");
				attemptLabel.append(std::to_string(event->GetValue()));
				attemptLabel.append(" of 3");
				bindings["Attempt"] = attemptLabel;
			}
			else if (attempts == 4)
			{
				bindings["Attempt"] = std::string("Failed to connect to ember. Exiting...");
			}
			else { exit(0); }
		}
		NativeUIHost::Get()->RenderScreen("Connecting", bindings, false);
	}
}