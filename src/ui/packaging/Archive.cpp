/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <memory>
#include <vector>
#include <base/pickle/Pickle.h>
#include <ui/packaging/Archive.h>

using namespace std;
using namespace base;

namespace player3 { namespace ui
{
	std::shared_ptr<Archive> Archive::ref;

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
	std::vector<char> Archive::GetFile(std::string name, std::string folder)
	{
		char* end;
		std::vector<char> buf;
		Json::Value fileInfo;
		if (folder != "/")
		{
			fileInfo = this->filesList[folder]["files"];
			fileInfo = fileInfo[name];
		}
		else { fileInfo = this->filesList[name]; }

		int size = fileInfo.get("size", -1).asInt();
		const char* offsetAsStr = fileInfo.get("offset", 0).asString().c_str();
		uint64_t offset = std::strtoull(offsetAsStr, &end, 10);

		Log("UI", "folder name = %s, filename = %s, size = %i", folder.c_str(), name.c_str(), size);

		buf.resize(size + 1);
		int bytesRead = file.Read(offset + this->headerSize, buf.data(), size);
		return buf;
		// buf.clear();
		// buf.resize(0);
	}
	std::string Archive::GetFileFromRoot(std::string name)
	{
		auto search = this->FileCache.find(name);
		if (search != this->FileCache.end())
		{
			Log("UI", "returning %s from cache.", name.c_str());
			return search->second;
		}
		else
		{
			Log("UI", "not returning %s from cache.", name.c_str());

			char* end;
			std::vector<char> buf;
			std::string fileContent("");
			Json::Value fileInfo = this->filesList[name];
			int size = fileInfo.get("size", -1).asInt();
			const char* offsetAsStr = fileInfo.get("offset", 0).asString().c_str();
			uint64_t offset = std::strtoull(offsetAsStr, &end, 10);

			buf.resize(size + 1);
			int bytesRead = file.Read(offset + this->headerSize, buf.data(), size);

			fileContent.assign(buf.data());
			//return fileContent;
			this->FileCache.insert(std::make_pair(std::move(name), std::move(fileContent)));
			return this->FileCache[name];
		}
	}
	std::string Archive::GetFileFromFolder(std::string folderName, std::string fileName)
	{
		//TODO: Files that can't represented as strings :/
		return "";
	}
}}