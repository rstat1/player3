/*
* Copyright (c) 2018 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef EMBRCOM
#define EMBRCOM

#include <ui/native/EventHub.h>

namespace player3 { namespace ember
{
	enum MessageType
	{
		MUTE,
		STOP,
		EXIT,
		START,
		INIT,
		UNMUTE,
		DISCONNECT,
		CHATUISTATE,
		QUALITYCHANGE,
	};
	EVENTARGS(EmberConnecting, int)
	EVENTARGS(EmberStream, std::string)
	EVENTARGS(EmberAuthenticated, std::string)
	EVENTARGS2(EmberStateChange, std::string, std::string)
}}

#endif