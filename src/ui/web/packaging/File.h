/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef BASEFILE
#define BASEFILE

#include <string>

namespace player3 { namespace ui
{
	class File
	{
		public:
			enum Flags
			{
				  FLAG_OPEN = 1 << 0, // Opens a file, only if it exists.
				  FLAG_READ = 1 << 5,
			};
			File() {}
			File(std::string path, uint32_t flags);
			void Open(std::string path);
			void Close();
			bool Exists();
			bool IsValid();
			int Read(uint64_t offset, char* buffer, int size);
			int ReadAtCurrentPos(char* data, int size);
		private:
			int fd = 0;
			FILE* file_;
			std::string filePath;
	};
}}

#endif