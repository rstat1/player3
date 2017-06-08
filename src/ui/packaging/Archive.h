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
#include <vector>
#include <unordered_map>
#include <ui/packaging/File.h>
#include <jsoncpp/json/json.h>

namespace player3 { namespace ui
{
	class Archive
	{
		public:
			Archive(std::string path);
			~Archive() { file.Close(); }
			bool Init();
			std::string GetFileFromRoot(std::string name);
			std::string GetFileFromFolder(std::string folderName, std::string fileName);
		private:
			File file;
			std::map<std::string, std::string> FileCache;
			uint32_t headerSize;
			Json::Value filesList;
	};
}}

#endif