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
#include <ui/packaging/Archive.h>
#include <seasocks/PageHandler.h>

using namespace seasocks;
using namespace base::utils;

namespace player3 { namespace ui
{
	class WebPageHandler : public PageHandler
	{
		public:
			WebPageHandler()
			{
				UIFiles = new Archive(GetAppPath().append("/ui.asar"));
				UIFiles->Init();

				//else { Log("UI", "UIFiles failed init"); }
			}
			virtual std::shared_ptr<Response> handle(const Request& request);
		private:
			Archive* UIFiles;
	};
}}

#endif