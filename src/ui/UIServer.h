/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef UISERVE
#define UISERVE

#include <uws/src/uWS.h>

namespace player3 { namespace ui
{
	struct RouteResponse
	{
		public:
			int length;
			const char* response;
	};
	class UIServer
	{
		public:
			UIServer();
		private:
			RouteResponse* ServeRoute(std::string route);
	};
}}

#endif