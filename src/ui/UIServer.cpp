/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <thread>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ui/UIServer.h>
#include <base/common.h>

using namespace std;

namespace player3 { namespace ui
{
	UIServer::UIServer()
	{
		std::thread ui([&] {
			uWS::Hub h;
			bool listenSuccess;
			h.onHttpRequest([&](uWS::HttpResponse* res, uWS::HttpRequest req, char* data, size_t, size_t ) {
				RouteResponse* resp;
				resp = this->ServeRoute(req.getUrl().toString().c_str());
				if (resp != nullptr) { res->end(resp->response, resp->length); }
				else { res->end("<p>Unknown route.</p>", 21); }
			});
#if defined(OS_STEAMLINK)
			listenSuccess = h.listen(80);
#else
			listenSuccess = h.listen(8080);
#endif
			if (listenSuccess) { Log("UI", "Listening on port whatever."); }
			else { Log("UI", "Not listening on port whatever"); }
			h.run();
		});
		ui.detach();
	}
	RouteResponse* UIServer::ServeRoute(std::string route)
	{
		std::stringstream homePage;
		Log("UI", "requested URL %s\n", route.c_str());

		return nullptr;
	}
}}