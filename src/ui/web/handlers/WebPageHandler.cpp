/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <ui/web/handlers/WebPageHandler.h>

namespace player3 { namespace ui
{
	void WebPageHandler::handle(HttpResponse* resp, HttpRequest req, char* data, size_t len, size_t remaining)
	{
		std::string contentType("");
		std::string folder("/");
		Log("UI", "handle request");
		std::string requestURL = req.getUrl().toString();
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
		if (EndsWith(requestURL, ".css") == true) { contentType = "text/css"; }
		else if (EndsWith(requestURL, ".js") == true) { contentType = "application/javascript"; }
		else if (EndsWith(requestURL, ".html") == true) { contentType = "text/html"; }
		else { contentType = "text/plain"; }

		auto response = Archive::Get()->GetFile(requestURL, folder);
		if (response.size() > 0) { resp->end(response.data(), response.size()); }
		else { resp->end(); }
	}
}}