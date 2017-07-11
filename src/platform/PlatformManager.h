/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef PLATMAN
#define PLATMAN

#include <memory>
#include <base/common.h>
#include <platform/Platforms.h>

namespace player3 { namespace platform
{
	class PlatformManager
	{
		public:
			void InitPlatformInterface();
			PlatformInterface* GetPlatformInterface() { return platformInterface; }
			static PlatformManager* Get()
			{
				if (!PlatformManager::ref) { ref = std::make_shared<PlatformManager>(); }
				return ref.get();
			}
		private:
			PlatformInterface* platformInterface;
			static std::shared_ptr<PlatformManager> ref;
	};
}}

#endif