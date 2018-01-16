/*
* Copyright (c) 2016 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/Utils.h>
#include <ui/native/EventHub.h>
#include <ui/native/NativeUIHost.h>
#include <ui/native/threading/UIWorkerHost.h>
#include <ui/native/threading/MessagePumpSDL.h>
#include <base/threading/common/IOWorkerThread.h>

using namespace base::threading;
using namespace base::threading::IO;

namespace player3 { namespace ui
{
	std::shared_ptr<UIWorkerHost> UIWorkerHost::ref;
	UIWorkerHost::UIWorkerHost()
	{
	}
	void UIWorkerHost::Init()
	{
		EVENT("UIWorkerThreadStarted");

		NEW_TASK0(UIWorker, UIWorkerHost, this, UIWorkerTaskFunc);
		UIWorkerTask* uiTask = new UIWorkerTask(UIWorker);

		IOWorkerThread* UIWorkerThread = new IOWorkerThread(uiTask);
		UIWorkerThread->Start("UIWorker");
	}
	void UIWorkerHost::UIWorkerTaskFunc()
	{
		Log("UIWorkerHost", "Init UI Worker task.");

		NEW_TASK0(UIWorkerInit, UIWorkerHost, this, UIWorkerInitFunc);

		dmp = new MessagePumpSDL("UI");
		Dispatcher::Get()->AddMessagePump("UI", dmp, base::utils::GetThreadID());
		dmp->MakeMessagePump(UIWorkerInit);
    }
	void UIWorkerHost::UIWorkerInitFunc()
	{
		NativeUIHost::Get()->InitUIHost();
        TRIGGER_EVENT(UIWorkerThreadStarted, nullptr);
	}
}}