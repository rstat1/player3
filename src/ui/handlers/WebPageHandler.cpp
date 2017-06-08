/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/Utils.h>
#include <seasocks/Request.h>
#include <seasocks/ResponseBuilder.h>
#include <ui/handlers/WebPageHandler.h>

using namespace base::utils;

namespace player3 { namespace ui
{
	std::shared_ptr<Response> WebPageHandler::handle(const seasocks::Request& request)
	{
		std::string content("");
		ResponseBuilder resp(ResponseCode::Ok);
		std::string requestURL = request.getRequestUri();
		if (requestURL == "/" || requestURL == "/login" || requestURL == "/home")
		{
			return Response::htmlResponse(UIFiles->GetFileFromRoot("index.html"));
		}
		else if (requestURL != "/ws")
		{
			requestURL = requestURL.replace(0, 1, "");
			if (requestURL.find("assets/") != std::string::npos)
			{
				requestURL = requestURL.replace(0, 7, "");
				content = UIFiles->GetFileFromFolder("assets/", requestURL);
			}
			else
			{
				content = UIFiles->GetFileFromRoot(requestURL);
			}
			if (EndsWith(requestURL, ".css") == true) { resp.withContentType("text/css"); }
			else { resp.withContentType("text/plain"); }
			resp << content;
			content.clear();
			return resp.build();
		}
		else { return nullptr; }
		// Log("UI", "requested url = %s", requestURL.c_str());
		// return nullptr;
	}
}}