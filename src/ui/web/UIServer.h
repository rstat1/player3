/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef UISERVE
#define UISERVE

#include <map>
#include <memory>
#include <base/common.h>

namespace player3 { namespace ui
{
	class UIServer
	{
		public:
			UIServer() {}
			void Init();

		SINGLETON(UIServer)
	};
}}

#endif