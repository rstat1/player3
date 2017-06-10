/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <thread>
#include <memory>
#include <uWS/src/uWS.h>
#include <ui/UIServer.h>
#include <base/logging.h>
#include <ui/handlers/WebPageHandler.h>
#include <ui/handlers/WebSocketHandler.h>

using namespace std;
using namespace uWS;

namespace player3 { namespace ui
{
	UIServer::UIServer()
	{
		std::thread ui([&] {
			int port;
			uWS::Hub h;
			WebPageHandler* handler = new WebPageHandler();
			WebSocketHandler* wsHandler = new WebSocketHandler();
#if defined(OS_STEAMLINK)
			port = 80;
#else
			port = 8080;
#endif
			h.onHttpRequest([&](HttpResponse* res, HttpRequest req, char* data, size_t len, size_t rem) {
				handler->handle(res,req, data, len, rem);
			});
			h.onMessage([&](WebSocket<SERVER>* ws, char* msg, size_t len, OpCode code) {
				wsHandler->onData(ws, msg, len);
			});
			h.onDisconnection([&](WebSocket<SERVER> *ws, int code, char *message, size_t length) {
				wsHandler->onDisconnect(ws);
			});
			bool listenSuccess = h.listen(port);
			if (listenSuccess) { Log("UI", "Listening on port whatever."); }
			else { Log("UI", "Not listening on port whatever"); }
			h.run();
//
		});
		ui.detach();
	}
}}