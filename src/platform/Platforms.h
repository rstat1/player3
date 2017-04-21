/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef DECODERS
#define DECODERS

#include <platform/PlatformInterface.h>

#if defined(OS_STEAMLINK)
	#include <platform/steamlink/SteamLinkPlatform.h>
	#define VIDEO_DECODE_INIT platformInterface = new SteamLinkPlatform();
#elif defined(OS_LINUX) && !defined(OS_STEAMLINK)
	#include <platform/desktop/DesktopPlatform.h>
	#define VIDEO_DECODE_INIT platformInterface = new DesktopPlatform();
#endif
#endif