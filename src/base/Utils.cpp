/*
* Copyright (c) 2013 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/Utils.h>
#include <base/common.h>
#include <boost/filesystem.hpp>
#if defined(OS_WIN)
#include <windows.h>
#else
#include <sys/syscall.h>
#endif

using namespace boost::filesystem;

namespace base { namespace utils
{
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
	{
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim))
		{
			elems.push_back(item);
		}
		return elems;
	}
	std::vector<std::string> split(const std::string &s, char delim)
	{
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}
	std::string GetAppPath()
	{
		path currentPath = initial_path();
		return currentPath.string();
	}
	std::string GetPlatformPathPrefix()
	{
#if defined(OS_WIN)
		return "";
#elif defined(OS_ANDROID)
		return "/data/data/us.rdro.xpappfx/";
#else
		return "";
#endif
	}
	int GetThreadID()
	{
#if defined(OS_WIN)
		return (int)GetCurrentThreadId();
#else
		return (int)syscall(__NR_gettid);
#endif
	}
#if defined(OS_LINUX) || defined(OS_ANDROID)
	pthread_t GetPthreadID()
	{
		return pthread_self();
	}
#endif
	//https://stackoverflow.com/questions/20446201/how-to-check-if-string-ends-with-txt
	bool EndsWith(const std::string &str, const std::string &suffix)
	{
		return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
	}
#if defined(OS_WIN)
	std::string GetLastErrorStdStr()
	{
		DWORD error = GetLastError();
		if (error)
		{
			LPVOID lpMsgBuf;
			DWORD bufLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
			if (bufLen)
			{
				LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
				std::string result(lpMsgStr, lpMsgStr + bufLen);

				LocalFree(lpMsgBuf);

				return result;
			}
		}
		return std::string();
	}
#endif
	std::string str_tolower(std::string s)
	{
    	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
			return std::tolower(c);
		});
    	return s;
	}
#if defined(OS_LINUX) || defined(OS_STEAMLINK)
	void OutputBacktrace()
	{
		int j, nptrs;
        void *buffer[100];
        char **strings;

        nptrs = backtrace(buffer, 100);
		strings = backtrace_symbols(buffer, nptrs);
        if (strings == NULL)
		{
			writeToLog("Failed to get backtrace");
			return;
        }

        for (j = 0; j < nptrs; j++) { Log("BTPRINT", "%s\n", strings[j]); }

        free(strings);
	}
#endif
}}