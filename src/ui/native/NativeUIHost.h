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
#include <ui/web/packaging/Archive.h>
#include <ui/native/elements/ElementBase.h>
#include <ui/native/rendering/NanoVGRenderer.h>

namespace player3 { namespace ui
{
	class NativeUIHost
	{
		public:
			NativeUIHost();
			void InitUIHost();
			void LoadInstanceOfLayout(const char* layoutName); //TODO: Property bindings.
		private:
			void LoadLayoutFile();

			Archive* files;
			SDL_Window* win;
			std::vector<int> screenSize;

			SINGLETON(NativeUIHost)

	};
}}

#endif