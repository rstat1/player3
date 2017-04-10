/*
* Copyright (c) 2012-2016 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef CONFIG
	#define CONFIG
	#define BASE_DEBUG 1
	#if defined(WITH_SDL)
		#define USING_SDL 1
	#endif
	#if defined(WITH_UI)
		#define INCLUDE_UI 1
	#endif

	#if defined(WINDOWS)
		#include <direct.h>
		#include <Windows.h>
		#define _Platform "Windows";
		#define OS_WIN 1
		#define Instance HINSTANCE
		#define WindowHandle HWND
		#define GetCurrentDir _getcwd
	#elif defined(LINUX)
		#include <unistd.h>
		#if defined(__arm__)
			#define _Platform "steamlink"
			#define OS_STEAMLINK 1
			#define OS_LINUX 1
		#else
			#define _Platform "Linux";
			#define OS_LINUX 1
		#endif
		#define Instance long unsigned int
		#define WindowHandle long unsigned int
		#define GetCurrentDir getcwd
	#elif defined(__ANDROID__)
		#define OS_ANDROID 1
		#define _Platform "Android"
	#else
		#error This platform is not supported.
	#endif

	#if defined(__GNUC__)
		#define COMPILER_GCC 1
	#elif defined(_MSC_VER)
		#define COMPILER_MSVC 1
	#else
		#error Please add support for your compiler in base/common.h
	#endif
	#if defined(COMPILER_MSVC)
		#define OVERRIDE override
	#elif defined(__clang__)
		#define OVERRIDE override
	#else
		#define OVERRIDE override
	#endif
	#define DISALLOW_COPY_AND_ASSIGN(TypeName) TypeName(const TypeName&); void operator=(const TypeName&)
#endif
#ifdef __cplusplus
#include <base/logging.h>
#define NOTIMPLEMENTED(name) Log("All", "%s is not currently implemented.",  name);
#endif
