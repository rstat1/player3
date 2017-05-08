/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef PAGEHANDLE
#define PAGEHANDLE

#include <memory>
#include <seasocks/PageHandler.h>

using namespace seasocks;

namespace player3 { namespace ui
{
	class AngularPageHandler : public PageHandler
	{
		public:
			virtual std::shared_ptr<Response> handle(const Request& request);
	};
}}

#endif