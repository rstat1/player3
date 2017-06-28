// /*
// * Copyright (c) 2017 The Incredibly Big Red Robot
// *
// * Use of this source code is governed by a "BSD-style" license that can be
// * found in the included LICENSE file.
// */

// #ifndef ASARRESP
// #define ASARRESP

// #include <memory>
// #include <seasocks/Response.h>
// #include <seasocks/ResponseWriter.h>
// #include <seasocks/TransferEncoding.h>

// using namespace seasocks;

// namespace player3 { namespace ui
// {
// 	class AsarResponse : public Response
// 	{
// 		public:
// 			virtual ~AsarResponse() {}
// 			virtual void cancel() override {}
// 			typedef std::multimap<std::string, std::string> Headers;
// 			AsarResponse(std::string name, std::string folder, const char* type);
// 			virtual void handle(std::shared_ptr<ResponseWriter> writer) override;
// 		private:
// 			std::string name_;
// 			std::string folder_;
// 			Headers headers;
// 			const char* type_;
// 	};
// }}

// #endif