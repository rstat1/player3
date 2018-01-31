#ifndef APP
#define APP

#include <memory>
#include <base/common.h>
#include <ui/ember/EmberService.h>
#include <ui/native/NativeUIHost.h>
#include <base/threading/common/TaskRunner.h>
#include <base/threading/dispatcher/DispatcherTypes.h>

using namespace player3::ember;
using namespace base::threading;

namespace app
{
	class App
	{
		public:
			void StartApp();
			void OnInitComplete();
			void ChatUIEvent(void* args);
			void ChatAction(EmberChatActionEventArgs* eventArgs);
			PROPERTY(CurrentChannelName, std::string);
		private:
			TaskRunner* taskRunner;
			void ShowHomeScreen();
			void EmberEventHandlers();
			void EmberConnectFailedEvent(EmberConnectingEventArgs* event);
			void ShowActivateScreen(EmberAuthenticatedEventArgs* eventArgs);

		SINGLETON(App);
	};
}

#endif