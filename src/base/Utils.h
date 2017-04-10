/*
* Copyright (c) 2013 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef UTILFUNCS
#define UTILFUNCS

#include <vector>
#include <sstream>
#include <algorithm>
#include <base/base_exports.h>

using namespace std;

namespace base { namespace utils
{
    BASEAPI std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	BASEAPI std::vector<std::string> split(const std::string &s, char delim);
	BASEAPI std::string GetPlatformPathPrefix();
	BASEAPI std::string GetAppPath();
	BASEAPI int GetThreadID();
#if defined(OS_WIN)
	std::string GetLastErrorStdStr();
#endif
}}

#endif