/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef PAGEHANDLE
#define PAGEHANDLE

#include <memory>
#include <base/Utils.h>
#include <uWS/src/uWS.h>
#include <ui/web/packaging/Archive.h>

using namespace uWS;
using namespace base::utils;

namespace player3 { namespace ui
{
	class WebPageHandler
	{
		public:
			WebPageHandler() {}
			void handle(HttpResponse* resp, HttpRequest req, char* data, size_t len, size_t remaining);
		private:
			Archive* UIFiles;
	};
}}

#endif