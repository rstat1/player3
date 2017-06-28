/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/Utils.h>
#include <base/common.h>
#include <ui/native/NativeUIHost.h>
#include <platform/PlatformManager.h>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace base::utils;
using namespace player3::platform;

namespace player3 { namespace ui
{
	std::shared_ptr<NativeUIHost> NativeUIHost::ref;
	NativeUIHost::NativeUIHost() {}
	void NativeUIHost::InitUIHost()
	{
		screenSize = PlatformManager::Get()->GetPlatformInterface()->GetScreenSize();

		Log("UI", "new window %i, %i, %s", screenSize[0], screenSize[1], "Player3");

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 5);

		win = SDL_CreateWindow("Player3", SDL_WINDOWPOS_UNDEFINED,
									  SDL_WINDOWPOS_UNDEFINED, screenSize[0], screenSize[1],
									  SDL_WINDOW_OPENGL);

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
		this->LoadLayoutFile();
	}
	void NativeUIHost::LoadLayoutFile()
	{

	}
}}