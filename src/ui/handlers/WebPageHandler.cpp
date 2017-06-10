/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/Utils.h>
#include <seasocks/Request.h>
#include <seasocks/SynchronousResponse.h>
#include <ui/handlers/WebPageHandler.h>
#include <ui/packaging/AsarResponse.h>

using namespace base::utils;

namespace player3 { namespace ui
{
	std::shared_ptr<Response> WebPageHandler::handle(const seasocks::Request& request)
	{
		std::string contentType("");
		std::string folder("/");
		Log("UI", "handle request");
		//ResponseBuilder resp(ResponseCode::Ok);
		std::string requestURL = request.getRequestUri();
		if (requestURL == "/ws") { return nullptr; }
		if (requestURL == "/" || requestURL == "/login" || requestURL == "/home")
		{
			requestURL = "index.html";
			contentType = "text/html";
		}
		else
		{
			requestURL = requestURL.replace(0, 1, "");
			if (requestURL.find("assets/") != std::string::npos)
			{
				requestURL = requestURL.replace(0, 7, "");
				folder = "assets";
			}

		}
		return make_shared<AsarResponse>(std::move(requestURL), folder, std::move(contentType.c_str()));
		//return make_shared<SynchronousResponse>()

	}
	std::shared_ptr<Response> WebPageHandler::HandleImageRequest(std::string fileName)
	{


		return nullptr;
	}
}}