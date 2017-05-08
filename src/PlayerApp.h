/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef PLAYAPP
#define PLAYAPP

#include <memory>
#include <base/threading/common/TaskRunner.h>
#include <base/threading/dispatcher/DispatcherTypes.h>

using namespace std;
using namespace base::threading;

namespace player3 { namespace player
{

	class PlayerApp
	{
		public:
			PlayerApp();
			void Start();
			TaskResult* StopStream();
			TaskResult* OnInitComplete();
			static PlayerApp* Get()
			{
				if (!PlayerApp::ref) { ref = make_shared<PlayerApp>(); }
				return ref.get();
			}
		private:
			TaskRunner* taskRunner;
			static std::shared_ptr<PlayerApp> ref;
	};
}}

#endif