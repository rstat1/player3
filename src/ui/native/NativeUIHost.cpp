/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <vector>
#include <base/Utils.h>
#include <base/common.h>
#include <ui/native/EventHub.h>
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
		layoutManager->LoadAndCacheLayout("Home");
		layoutManager->LoadAndCacheLayout("ChatUI");
		layoutManager->LoadAndCacheLayout("Activation");
		layoutManager->LoadAndCacheLayout("DebugOverlay");
		// layoutManager->LoadAndCacheLayout("StreamIsLive");

		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
		win = SDL_CreateWindow("Player3", SDL_WINDOWPOS_UNDEFINED,
									  	  SDL_WINDOWPOS_UNDEFINED, 1280, 720,
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
#if defined(OS_STEAMLINK)
		SDL_ShowCursor(0);
#endif
		SDL_ShowWindow(win);
	}
	void NativeUIHost::RenderScreen(const char* name, std::map<std::string, boost::any> propertyBindings, bool clearRoot = false)
	{
		NanoVGRenderer::Get()->Clear();
		layoutManager->RenderLayout(propertyBindings, name, clearRoot);
		NanoVGRenderer::Get()->Present();
	}
	void NativeUIHost::SDLEvent(SDL_Event event)
	{

	}
	void NativeUIHost::ClearScreen()
	{
		NanoVGRenderer::Get()->Clear();
		NanoVGRenderer::Get()->Present();
	}
}}