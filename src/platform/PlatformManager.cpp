/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <platform/PlatformManager.h>

namespace player3 { namespace platform
{
	std::shared_ptr<PlatformManager> PlatformManager::ref;

	void PlatformManager::InitPlatformInterface() { PLATFORM_INIT; }
}}