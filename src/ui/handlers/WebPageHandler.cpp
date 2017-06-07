/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <ui/handlers/WebPageHandler.h>
#include <seasocks/Request.h>

namespace player3 { namespace ui
{
	std::shared_ptr<Response> WebPageHandler::handle(const seasocks::Request& request)
	{
		std::string requestURL = request.getRequestUri();
		if (requestURL == "/" || requestURL == "/login" || requestURL == "/home")
		{
			return Response::htmlResponse(UIFiles->GetFileFromRoot("index.html"));
		}
		else
		{
			requestURL = requestURL.replace(0, 1, "");
			if (requestURL.find("assets/") != std::string::npos)
			{
				requestURL = requestURL.replace(0, 7, "");
				return Response::htmlResponse(UIFiles->GetFileFromFolder("assets", requestURL));
			}
			else
			{
				return Response::textResponse(UIFiles->GetFileFromRoot(requestURL));
			}
		}
		Log("UI", "requested url = %s", requestURL.c_str());
		return nullptr;
	}
}}