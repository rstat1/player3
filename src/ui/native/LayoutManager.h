/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef LAYOUT
#define LAYOUT

#include <vector>
#include <base/common.h>
#include <ui/native/elements/ElementBase.h>

namespace player3 { namespace ui
{
	class LayoutManager
	{
		public:
			void LoadUISM(const char* name);
		private:
			void DoLayout();
			void RenderLayout();
			void CreateElement();
			Style ParseStyleBlob(std::string styleBlob);
	};
}}

#endif