/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/logging.h>
#include <fcntl.h>
#include <boost/filesystem.hpp>
#include <ui/packaging/File.h>

using namespace boost::filesystem;

namespace player3 { namespace ui
{
	File::File(std::string path, uint32_t flags) : filePath(path)
	{
		// fd = open(filePath.c_str(), O_RDONLY);
		// //file_ = fopen(filePath.c_str(), "r");
		// if (fd != -1) Log("UI", "Open file: %s", path.c_str());
	}
	void File::Open(std::string path)
	{
		fd = open(path.c_str(), O_RDONLY);

		perror("open filed failed ");
		if (fd != -1) Log("UI", "Open file: %s", path.c_str());
	}
	void File::Close()
	{
		if (file_) { fclose(file_); }
	}
	bool File::Exists()
	{
		if (exists(filePath)) { return true; }
		return false;
	}
	bool File::IsValid()
	{
		return fd != -1;
	}
	int File::Read(uint64_t offset, char* buffer, int size)
	{
		int bytesRead = 0, rv;
		if (IsValid() == false)
		{
			Log("File", "fd not valid");
			return -1;
		}
		do
		{
			rv = pread(fd, buffer + bytesRead, size - bytesRead, offset + bytesRead);
			if (rv <= 0) { break; }
			bytesRead += rv;
		}
		while ( bytesRead < size);
		return bytesRead ? bytesRead : rv;
	}
	int File::ReadAtCurrentPos(char* data, int size)
	{
		int bytesRead = 0, rv;
		if (IsValid() == false) { return -1; }
		do
		{
			rv = read(fd, data + bytesRead, size - bytesRead);
			if (rv <= 0) { break; }
			bytesRead += rv;
		}
		while ( bytesRead < size);
		return bytesRead ? bytesRead : rv;
	}
}}