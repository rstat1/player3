/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <iostream>
#include <vector>
#include <base/pickle/Pickle.h>
#include <ui/packaging/Archive.h>

using namespace std;
using namespace base;

namespace player3 { namespace ui
{
	Archive::Archive(std::string path)
		: file(path, File::Flags::FLAG_OPEN | File::Flags::FLAG_READ)
	{
		Log("UI", "ASAR Path: %s", path.c_str());
	}
	bool Archive::Init()
	{
		if (file.IsValid() == false) { return false; }

		int len;
		uint32_t headerSize;
		std::string header;
		std::vector<char> buf;

		buf.resize(8);
		len = file.ReadAtCurrentPos(buf.data(), buf.size());
		if (len != static_cast<int>(buf.size()))
		{
			Log("UI", "Failed to read header from file");
			return false;
		}
		if (!PickleIterator(Pickle(buf.data(), buf.size())).ReadUInt32(&headerSize))
		{
			Log("UI", "Failed to parse header size");
			return false;
		}
		buf.resize(headerSize);
		len = file.ReadAtCurrentPos(buf.data(), buf.size());
		if (!PickleIterator(Pickle(buf.data(), buf.size())).ReadString(&header))
		{
			Log("UI", "Failed to parse header from file");
			return false;
		}

		Log("UI", header.c_str());

		Json::Value headerFromJSON;
		Json::Reader reader;
		reader.parse(header, headerFromJSON, false);
		this->filesList = headerFromJSON["files"];
		this->headerSize = 8 + headerSize;
		return true;
	}
	std::string Archive::GetFileFromRoot(std::string name)
	{
		char* end;
		std::vector<char> buf;
		std::string fileContent("");
		Json::Value fileInfo = this->filesList[name];
		int size = fileInfo.get("size", -1).asInt();
		const char* offsetAsStr = fileInfo.get("offset", 0).asString().c_str();
		uint64_t offset = std::strtoull(offsetAsStr, &end, 10);

		buf.resize(size);
		int bytesRead = file.Read(offset + this->headerSize, buf.data(), size - 1);
		fileContent = buf.data();
		buf.clear();

		return fileContent;
	}
	std::string Archive::GetFileFromFolder(std::string folderName, std::string fileName)
	{
		//TODO: Files that can't represented as strings :/
		return "";
	}
}}