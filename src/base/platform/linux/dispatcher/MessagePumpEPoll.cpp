/*
* Copyright (c) 2014-2016 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <string>
#include <sys/syscall.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <base/common.h>
#include <base/threading/dispatcher/MessagePump.h>
#include <base/threading/common/TaskRunner.h>
#include <base/threading/dispatcher/DispatcherTypes.h>
#include <iostream>
#include <string.h>
#include <errno.h>

using namespace platform;

namespace base { namespace threading
{
	std::map<int, MessageReceiver*> MessagePumpEPoll::handlers;
	bool DispatcherMessagePump::msgPumpWinInit;

	void MessagePumpEPoll::MakeMessagePump(bool isTaskRunner)
	{
		Log("MakeMessagePump", "Current Thread ID: %u, name = %s", syscall(__NR_gettid), this->wndIdExtenstion);

        struct epoll_event eventInfo;
		int epollFD, result;

		Log("Dispatcher", this->wndIdExtenstion);
		SharedThreadState* sts = this->GetSharedState(isTaskRunner);

		if (sts == nullptr) { writeToLog("Why is the null?"); }
		else
		{
			int result = pipe(sts->fds);
			if (result == -1)
            {
                perror(this->wndIdExtenstion);
                Log(this->wndIdExtenstion, "Pipe could create not. Result: %i", errno);
            }
            else
            {
                if (fcntl(sts->fds[0], F_SETFL, O_NONBLOCK) != 0) { writeToLog("fcntl couldn't. (1)"); }
			    if (fcntl(sts->fds[1], F_SETFL, O_NONBLOCK) != 0) { writeToLog("fcntl couldn't. (2)"); }

			    sts->epollFD = epoll_create(1);
			    if (sts->epollFD == -1) { writeToLog("Failed to create epoll FD"); }

			    memset(&eventInfo, 0, sizeof(epoll_event));
			    eventInfo.events = EPOLLIN | EPOLLET;
			    eventInfo.data.fd = sts->fds[0];

			    result = epoll_ctl(sts->epollFD, EPOLL_CTL_ADD, sts->fds[0], &eventInfo);
			    if (result != 0) { writeToLog("epoll_ctl failed"); }
            }
		}
		this->StartMessageLoop(isTaskRunner);
	}
	void MessagePumpEPoll::MakeMessagePump(DispatcherTask* InitTask)
	{
		this->initTask = InitTask;
		this->MakeMessagePump(true);
	}
	void MessagePumpEPoll::StartMessageLoop(bool isTaskRunner)
	{
		Log("StartMessageLoop", "Current Thread ID: %u, name = %s", syscall(__NR_gettid), this->wndIdExtenstion);

		bool runInitTask = true;
		DispatcherTask* task;
		MessageReceiver* recv;
        SharedThreadState* sts = this->GetSharedState(isTaskRunner);

		if (sts != nullptr)
		{
			struct epoll_event events[4];
			this->startComplete = true;
			this->postBlocker->Signal();

			while (1)
			{
				//TODO: Properly implement the InitTask stuff on not-TaskRunners
				if (isTaskRunner && runInitTask)
				{
					runInitTask = false;
					if (this->initTask != nullptr)
					{
						Log("StartMessageLoop", "Running init task.");
						write(sts->fds[1], &initTask, sizeof(initTask));
					}
				}
				epoll_wait(sts->epollFD, events, 1, -1);

				if (events[0].data.fd > 0)
				{
					//This will BLOCK the thread for duration of hook function.
					auto it = handlers.find(events[0].data.fd);
					if (it != handlers.end())
					{
						recv = it->second;
						recv->recvFunc();
					}
				}
				ssize_t bytesRead = read(sts->fds[0], &task, sizeof(task));
				if (bytesRead == sizeof(task))
				{
					if (task->HasCallback()) { task->InvokeWithCallback(!task->HasArguments()); }
					else { task->Invoke(!task->HasArguments()); }
				}
			}
		}
		else { writeToLog("STS doesn't real"); }
	}
	void MessagePumpEPoll::PostMessageToThread(const char* thread, DispatcherTask *task, bool isTaskRunner)
	{
		if (this->startComplete == false) { this->postBlocker->Wait(); }

		Thread* AppHostThread = Dispatcher::Get()->GetThread(thread);
		SharedThreadState* sts = this->GetSharedState(isTaskRunner);

		write(sts->fds[1], &task, sizeof(task));
	}
	void MessagePumpEPoll::RegisterMessageHandler(MessageReceiver* recv)
	{
		NOTIMPLEMENTED("DispatcherMessagePump::RegisterMessageHandler() - Linux");
	}
    SharedThreadState* MessagePumpEPoll::GetSharedState(bool isTaskRunner)
    {
        SharedThreadState* sts;
        if (isTaskRunner)
        {
            TaskRunner* taskRunner = Dispatcher::Get()->GetTaskRunner(this->wndIdExtenstion);
            sts = (SharedThreadState*)taskRunner->extra;
        }
        else
        {
            Thread* AppHostThread = Dispatcher::Get()->GetThread(this->wndIdExtenstion);
            sts = (SharedThreadState*)AppHostThread->extra;
        }
        if (sts == nullptr) { return nullptr; }
        else { return sts; }
    }
}}