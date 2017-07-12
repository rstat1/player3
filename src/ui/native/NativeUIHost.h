/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef NATIVEUI
#define NATIVEUI

#include <vector>
#include <memory>
#include <ui/native/LayoutManager.h>
#include <ui/native/rendering/NanoVGRenderer.h>

namespace player3 { namespace ui
{
	class NativeUIHost
	{
		public:
			NativeUIHost();
			void InitUIHost();
			void RenderLayout(const char* name, std::map<std::string, boost::any> propertyBindings, bool clearRoot);
		private:
			SDL_Window* win;
			std::vector<int> screenSize;
			LayoutManager* layoutManager;

		SINGLETON(NativeUIHost)

	};
}}

#endif