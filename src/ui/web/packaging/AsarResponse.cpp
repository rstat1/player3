// /*
// * Copyright (c) 2017 The Incredibly Big Red Robot
// *
// * Use of this source code is governed by a "BSD-style" license that can be
// * found in the included LICENSE file.
// */

// #include <base/logging.h>
// #include <ui/packaging/Archive.h>
// #include <ui/packaging/AsarResponse.h>

// namespace player3 { namespace ui
// {
// 	AsarResponse::AsarResponse(std::string name, std::string folder, const char* type)
// 	{
// 		name_ = std::move(name);
// 		folder_ = std::move(folder);
// 		type_ = std::move(type);
// 		Log("UI", "init asarr");
// 	}
// 	void AsarResponse::handle(std::shared_ptr<ResponseWriter> writer)
// 	{

// 		headers = AsarResponse::Headers({
// 			{"Content-Type", type_},
// 			{"Content-Length", std::to_string(resp.size())}
// 		});
// 		Log("UI", "type = %s", type_);

// 		writer->begin(ResponseCode::Ok, TransferEncoding::Chunked);
// 		for (auto it = headers.begin(); it != headers.end(); ++it) {
//         	writer->header(it->first, it->second);
//     	}
// 		writer->payload(resp.data(), resp.size() - 1, true);
// 		writer->finish(true);
// 	}
// }}