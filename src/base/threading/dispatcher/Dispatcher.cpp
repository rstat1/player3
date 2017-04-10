/*
* Copyright (c) 2014 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#define INCLUDE_DEFINTIONS 1

#include <assert.h>
#include <base/Utils.h>
#include <base/threading/dispatcher/DispatcherTypes.h>
#if defined(OS_WIN)
#include <windows.h>
#else
#include <sys/syscall.h>
#endif

namespace base { namespace threading
{
	std::shared_ptr<Dispatcher> Dispatcher::ref;

	Dispatcher::Dispatcher()  {}
	Dispatcher* Dispatcher::Get()
	{
		if (!Dispatcher::ref) { ref = make_shared<Dispatcher>(); }
		return ref.get();
	}
	void Dispatcher::AddMessagePump(const char* name, DispatcherMessagePump* dmp, int parentThreadID)
	{
		if (this->knownDMPs.find(name) == knownDMPs.end())
		{
			this->knownDMPs[name] = dmp;
			this->threadIDs[parentThreadID] = name;
		}
		else { Log("base", "Not re-adding existing DMP named: %s", name); }
	}
	void Dispatcher::AddTaskRunner(const char* name, TaskRunner* dmpRef)
	{
		if (this->namedDMPs.find(name) == namedDMPs.end())
		{
			 this->namedDMPs[name] = dmpRef;
			 this->threadIDs[dmpRef->Id] = name;
		}
		else { Log("base", "Not re-adding existing task runner named: %s", name); }
	}
	void Dispatcher::AddNamedThread(const char* name, Thread* threadRef)
	{
		if (this->namedThreads.find(name) == namedThreads.end())
		{
			 this->namedThreads[name] = threadRef;
			 this->threadIDs[threadRef->Id] = name;
		}
		else { Log("base", "Not re-adding existing thread named: %s", name); }
	}
	void Dispatcher::PostTaskToTaskRunner(DispatcherTask* taskToRun, const char* DispatcherName)
	{
		TaskRunner* runner = GetTaskRunner(DispatcherName);
		if (runner != nullptr) { runner->RunTask(taskToRun); }
		else { Log("Dispatcher", "Couldn't find task runner named %s", DispatcherName); }
	}
	void Dispatcher::PostTaskToThread(DispatcherTask* TaskToComplete, const char* threadName)
	{
		Thread* ref = GetThread(threadName);
		if (ref != nullptr) { ref->PostTask(TaskToComplete); }
		else { this->PostTaskToTaskRunner(TaskToComplete, threadName); }
	}
	void Dispatcher::PostTask(const char* receiver, DispatcherTask* task)
	{
		Thread* ref = GetThread(receiver);
		TaskRunner* runner = GetTaskRunner(receiver);
		DispatcherMessagePump* messagePump = GetMessagePump(receiver);

		if (runner != nullptr) { runner->RunTask(task); }
		else if (ref != nullptr) { ref->PostTask(task); }
		else { messagePump->PostMessageToThread(receiver, task, false); }
	}
	TaskRunner* Dispatcher::GetTaskRunner(const char* name)
	{
		auto mapIter = this->namedDMPs.find(name);
		if (mapIter != namedDMPs.end()) { return mapIter->second; }
		else { return nullptr; }
	}
	Thread* Dispatcher::GetThread(const char* name)
    {
		auto mapIter = this->namedThreads.find(name);
		if (mapIter != namedThreads.end()) { return mapIter->second; }
		else { return nullptr; }
	}
	DispatcherMessagePump* Dispatcher::GetMessagePump(const char* name)
	{
		auto mapIter = this->knownDMPs.find(name);
		if (mapIter != knownDMPs.end()) { return mapIter->second; }
		else { return nullptr; }
	}
	int Dispatcher::GetThreadId(const char* name)
	{
		return this->GetThread(name)->Id;
	}
	int Dispatcher::GetCurrentThreadId()
	{
		return base::utils::GetThreadID();
	}
	const char* Dispatcher::GetThreadNameFromId(int id)
	{
		auto mapIter = this->threadIDs.find(id);
		if (mapIter != threadIDs.end()) { return mapIter->second; }
		else { return ""; }
	}
	void Dispatcher::IsCorrectThread(const char* correctThread)
	{
		const char* currentThreadName = this->GetThreadNameFromId(this->GetCurrentThreadId());

		Log("Dispatcher", "thread check -- correctThread %s, currentthread: %s", correctThread, currentThreadName);

		assert(strcmp(currentThreadName, correctThread) == 0);
	}
}}
