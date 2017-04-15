/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef DECODERS
#define DECODERS

#include <platform/DecoderInterface.h>

#if defined(OS_STEAMLINK)
	#include <platform/steamlink/SteamLinkDecoder.h>
	#define VIDEO_DECODE_INIT decoder = new SteamLinkDecoder();
#elif defined(OS_LINUX) && !defined(OS_STEAMLINK)
	#include <platform/desktop/DesktopDecoderInterface.h>
	#define VIDEO_DECODE_INIT decoder = new DesktopDecoderInterface();
#endif
#endif