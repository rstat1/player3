/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <thread>
#include <memory>
#include <ui/UIServer.h>
#include <seasocks/Server.h>
#include <seasocks/PrintfLogger.h>
#include <ui/handlers/WebSocketHandler.h>
#include <ui/handlers/WebPageHandler.h>

using namespace std;
using namespace seasocks;

namespace player3 { namespace ui
{
	UIServer::UIServer()
	{
		std::thread ui([&] {
			int port;

#if defined(OS_STEAMLINK)
			port = 80;
#else
			port = 8080;
#endif
			std::shared_ptr<PrintfLogger> logger = std::make_shared<PrintfLogger>(Logger::Level::_DEBUG);
//"/home/rstat1/Apps/streamlink/player/src/ui/ui/dist"
			Server web(logger);
			web.addWebSocketHandler("/ws", std::make_shared<WebSocketHandler>());
			web.addPageHandler(std::make_shared<WebPageHandler>());
			web.serve("", port);
		});
		ui.detach();
	}
}}