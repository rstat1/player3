#ifndef APP
#define APP

#include <memory>
#include <base/common.h>
#include <ui/native/NativeUIHost.h>
#include <base/threading/common/TaskRunner.h>
#include <base/threading/dispatcher/DispatcherTypes.h>

using namespace base::threading;

namespace app
{
	class App
	{
		public:
			void StartApp();
			void OnInitComplete();
			void SwitchChats(void* args);
			void ChatUIEvent(void* args);
		private:
			TaskRunner* taskRunner;
			void ShowActivateScreenIfNeeded();

		SINGLETON(App);
	};
}

#endif