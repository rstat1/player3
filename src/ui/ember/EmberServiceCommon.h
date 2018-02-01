/*
* Copyright (c) 2018 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef EMBRCOM
#define EMBRCOM

#include <ui/native/EventHub.h>

namespace player3 { namespace ember
{
	using namespace player3::ui;
	enum MessageType
	{
		MUTE,
		STOP,
		EXIT,
		START,
		INIT,
		UNMUTE,
		ACTIVATE,
		DEACTIVATE,
		CHATUISTATE,
		QUALITYCHANGE,
		CHATUIPOSITION,
	};
	EVENTARGS(EmberConnecting, int)
	EVENTARGS(EmberStream, std::string)
	EVENTARGS(EmberAuthenticated, std::string)
	EVENTARGS2(EmberChatAction, std::string, std::string)
	EVENTARGS2(EmberStateChange, std::string, std::string)
}}

#endif