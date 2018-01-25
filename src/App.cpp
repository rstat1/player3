#define EGL_API_FB
#include <SDL_egl.h>

#include <App.h>
#include <BuildInfo.h>
#include <player/Player.h>
#include <ui/native/EventHub.h>
#include <player/chat/ChatService.h>
#include <platform/PlatformManager.h>
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
		EVENT(ConnectToChat)
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
	void App::EmberEventHandlers()
	{
		HANDLE_EVENT(EmberAuthenticated, true, "UI", HANDLER {
			writeToLog("show home");
			this->ShowHomeScreen();
			if (ChatService::Get()->GetIsConnected() == false)
			{
				writeToLog("Connecting to chat...");
				ChatService::Get()->ConnectToTwitchIRC(EmberService::Get()->GetEmberTwitchToken().c_str(),
					EmberService::Get()->GetEmberTwitchUsername().c_str());
			}
		})
		HANDLE_EVENT(EmberConnecting, true, "UI", HANDLER {
			if (EmberService::Get()->GetEmberIsPlaying()) { /* TODO: Show a smaller notif style thing saying the connection dropped. */ }
			else
			{
				NativeUIHost::Get()->RenderScreen("Connecting", std::map<std::string, boost::any>{}, false);
			}
		})
		HANDLE_EVENT(EmberConnectFailed, true, "UI", HANDLER {
			this->EmberConnectFailedEvent((EmberConnectingEventArgs*)args);
			if (EmberService::Get()->GetEmberIsPlaying() == false)
			{
				sleep(5);
				exit(0);
			}
			else { /* TODO: Something to prevent interupting a playing stream... Or something. */ }
		})
		HANDLE_EVENT(EmberNeedsActivation, true, "UI", HANDLER {
			writeToLog("activation needed");
			this->ShowActivateScreen((EmberAuthenticatedEventArgs*)args);
			if (ChatService::Get()->GetIsConnected()) { ChatService::Get()->DisconnectFromTwitchIRC(); }
		})
		HANDLE_EVENT(EmberStateChange, true, "UI", HANDLER {
			EmberStateChangeEventArgs* newState = (EmberStateChangeEventArgs*)args;
			Log("app", "EmberStateChange, playing = %s, muted = %s", newState->GetFirstArgument().c_str(), newState->GetSecondArgument().c_str());
			if (newState->GetFirstArgument() == "playing") {
				Log("app", "start stream");
				NativeUIHost::Get()->ClearScreen();
			} else if (newState->GetFirstArgument() == "notplaying") {
				NativeUIHost::Get()->RenderScreen("Home", std::map<std::string, boost::any>{}, false);
				ChatService::Get()->LeaveCurrentChannel();
			}
			EmberService::Get()->SendStateChange(newState);
		})
		HANDLE_EVENT(EmberChatAction, true, "UI", HANDLER {
			EmberChatActionEventArgs* eventArgs = (EmberChatActionEventArgs*)args;
			this->ChatAction(eventArgs);
		})
		EmberService::Get()->ConnectToEmber();
	}
	void App::ShowActivateScreen(EmberAuthenticatedEventArgs* eventArgs)
	{
		std::map<std::string, boost::any> bindings;
		bindings["DeviceName"] = eventArgs->GetValue();
		NativeUIHost::Get()->RenderScreen("Activation", bindings, false);
	}
	void App::EmberConnectFailedEvent(EmberConnectingEventArgs* event)
	{
		int attempts = 0;
		std::map<std::string, boost::any> bindings;

		if (event != nullptr)
		{
			attempts = event->GetValue();
			std::string attemptLabel("");
			attemptLabel.append("Failed to connect after ");
			attemptLabel.append(std::to_string(event->GetValue()));
			attemptLabel.append(" attempts. Exiting in 5 seconds.");
			bindings["Attempt"] = attemptLabel;
			NativeUIHost::Get()->RenderScreen("Connecting", bindings, false);
		};
	}
	void App::ShowHomeScreen()
	{
		std::map<std::string, boost::any> bindings;
		std::string versionLabel("");
		versionLabel.append("Build: ");
		versionLabel.append(BUILDNUMBER);
		versionLabel.append(".");
		versionLabel.append(BRANCH);
		bindings["Version"] = versionLabel;
		NativeUIHost::Get()->RenderScreen("Home", bindings, false);
	}
	void App::ChatAction(EmberChatActionEventArgs* eventArgs)
	{
		if (eventArgs->GetFirstArgument() == "join")
		{
			ChatService::Get()->JoinChannel(eventArgs->GetSecondArgument().c_str());
		}
	}
}