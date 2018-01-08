/*
* Copyright (c) 2016 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef RENDERER
#define RENDERER

#include <memory>
#include <base/threading/common/Thread.h>
#include <base/threading/common/IOTasks.h>
#include <base/threading/common/ConditionVariable.h>
#include <base/threading/dispatcher/DispatcherTypes.h>

using namespace base::threading;
using namespace base::threading::IO;

namespace player3 { namespace ui
{
	class UIWorkerTask : public IOTaskBase
	{
		public:
			UIWorkerTask(DispatcherTask* uiWorkerTask)
			{
				this->task = uiWorkerTask;
			}
			void ExecuteTask() override
			{
				this->task->Invoke(false);
			}
		private:
			UIWorkerTask() {}
	};
	class UIWorkerHost
	{
		public:
			UIWorkerHost();
			~UIWorkerHost() {}
			void Init();
			void UIWorkerTaskFunc();
			void UIWorkerInitFunc();
		private:
			DispatcherMessagePump* dmp;

		SINGLETON(UIWorkerHost)
	};
}}

#endif