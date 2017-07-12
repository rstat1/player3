/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <vector>
#include <base/Utils.h>
#include <base/common.h>
#include <ui/native/NativeUIHost.h>
#include <platform/PlatformManager.h>

using namespace base::utils;
using namespace player3::platform;

namespace player3 { namespace ui
{
	std::shared_ptr<NativeUIHost> NativeUIHost::ref;
	NativeUIHost::NativeUIHost() {}
	void NativeUIHost::InitUIHost()
	{
		screenSize = PlatformManager::Get()->GetPlatformInterface()->GetScreenSize();
		layoutManager = new LayoutManager();
		layoutManager->LoadAndCacheLayout("ChatUI");
		layoutManager->LoadAndCacheLayout("StreamIsLive");
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
		win = SDL_CreateWindow("Player3", SDL_WINDOWPOS_UNDEFINED,
									  	  SDL_WINDOWPOS_UNDEFINED, screenSize[0], screenSize[1],
									  	  SDL_FLAGS);
		Log("SteamLinkPlatform", "%s", SDL_GetError());
		if (win != nullptr)
		{
			Log("UI", "Init renderer");
			NanoVGRenderer::Get()->RendererInit(win);
			NanoVGRenderer::Get()->Clear();
			NanoVGRenderer::Get()->Present();
		}
		else { Log("SteamLinkPlatform", "%s", SDL_GetError()); }
		SDL_ShowWindow(win);
		INIT_GPU_PROFILE
	}
	void NativeUIHost::RenderLayout(const char* name, std::map<std::string, boost::any> propertyBindings, bool clearRoot = false)
	{
		layoutManager->RenderLayout(propertyBindings, name, clearRoot);
	}
}}