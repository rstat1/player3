/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

//Implements a reader for the ASAR format, created/used by the Electron project.

#ifndef ASARREAD
#define ASARREAD

#include <map>
#include <string>
#include <memory>
#include <base/Utils.h>
#include <ui/web/packaging/File.h>
#include <jsoncpp/json/json.h>
#include <ui/web/packaging/AsarResponse.h>

using namespace base::utils;

namespace player3 { namespace ui
{
	class Archive
	{
		public:
			Archive()
			{
				Log("UI", "archive init");
				file.Open(GetAppPath().append("/ui.asar"));
				Init();
			}
			Archive(std::string path);
			~Archive() { file.Close(); }
			bool Init();
			std::vector<char> GetFile(std::string name, std::string folder);
			std::string GetFileFromRoot(std::string name);
			std::string GetFileFromFolder(std::string folderName, std::string fileName);
			static Archive* Get()
			{
				if (!Archive::ref) { ref = std::make_shared<Archive>(); }
				return ref.get();
			}
		private:
			File file;
			static std::shared_ptr<Archive> ref;
			std::map<std::string, std::string> FileCache;
			uint32_t headerSize;
			Json::Value filesList;
	};
}}

#endif