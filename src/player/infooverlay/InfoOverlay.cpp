/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <utility>
#include <string>
#include <base/common.h>
#include <player/infooverlay/InfoOverlay.h>

using namespace std;

namespace player3 { namespace overlay
{
	void InfoOverlay::InitOverlay()
	{
		this->overlaySurfaceW = 210;
		if (TTF_Init() == -1) { Log("InfoOverlay", "failed to init ttf"); }
		else { font = TTF_OpenFont("NotoSans-Regular.ttf", 12); }
	}
	void InfoOverlay::AddLabeledDoubleValue(const char* label, double value)
	{
		SDL_Surface* text;
		OverlayItem overlayItem;

		string item("");
		item.append(label);
		item.append(": ");
		item.append(to_string(value).c_str());
		text = this->GetTextSurface(item.c_str());
		this->overlaySurfaceH += text->h;
		overlayItems.emplace(std::make_pair(label, OverlayItem(item.c_str(), text)));
	}
	void InfoOverlay::UpdateDoubleValue(const char* label, double newValue)
	{
		SDL_Surface* text;
		OverlayItem overlayItem;
		if (overlayItems.find(label) != overlayItems.end())
		{
			overlayItem = overlayItems[label];
			string item("");
			item.append(label);
			item.append(": ");
			item.append(to_string(newValue).c_str());

			SDL_FreeSurface(overlayItem.pixels);
			text = this->GetTextSurface(item.c_str());
			overlayItem.pixels = text;
			overlayItem.itemInfo = item.c_str();

			overlayItems[label] = overlayItem;
		}
	}
	Overlay* InfoOverlay::UpdateOverlay()
	{
		if (overlaySurface == nullptr)
		{
#if defined(OS_STEAMLINK)
		overlaySurface = SDL_CreateRGBSurfaceWithFormat(0, this->overlaySurfaceW, this->overlaySurfaceH, 32, SDL_PIXELFORMAT_ARGB32);
#else
		overlaySurface = SDL_CreateRGBSurface(0, this->overlaySurfaceW, this->overlaySurfaceH, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#endif
		}
		else { SDL_FillRect(overlaySurface, nullptr, SDL_MapRGB(overlaySurface->format, 0, 0, 0)); }
		int i = 0, prevHeight;
		SDL_Rect overlayItemRect;

		overlayItemRect.x = 0;
		for (const auto& pair : overlayItems)
		{
			overlayItemRect.x = 0;
			overlayItemRect.y = prevHeight;
			overlayItemRect.w = pair.second.pixels->w;
			overlayItemRect.h = pair.second.pixels->h;
			prevHeight = pair.second.pixels->h;
			if (i == 0) { overlayItemRect.y = 0; }

			SDL_BlitSurface(pair.second.pixels, NULL, overlaySurface, &overlayItemRect);
			i++;
		}
		return new Overlay(overlaySurface->pitch, overlaySurface, overlaySurfaceW, overlaySurfaceH);
	}
	SDL_Surface* InfoOverlay::GetTextSurface(const char* text)
	{
		SDL_Color fg = { 255, 255, 255 };
		SDL_Color bg = { 0, 0, 0 };
		return TTF_RenderText_Shaded(font, text, fg, bg);
	}
}}
