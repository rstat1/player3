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
	void InfoOverlay::AddDoubleValue(const char* label, double value)
	{
		this->CreateOverlayItem(label, to_string(value).c_str());
	}
	void InfoOverlay::AddIntValue(const char* label, int value)
	{
		this->CreateOverlayItem(label, to_string(value).c_str());
	}
	void InfoOverlay::UpdateDoubleValue(const char* label, double newValue)
	{
		this->UpdateOverlayItem(label, to_string(newValue).c_str());
	}
	void InfoOverlay::UpdateIntValue(const char* label, int newValue)
	{
		this->UpdateOverlayItem(label, to_string(newValue).c_str());
	}
	void InfoOverlay::UpdateOverlayItem(const char* label, const char* newValue)
	{
		OverlayItem overlayItem;
		if (overlayItems.find(label) != overlayItems.end())
		{
			overlayItem = overlayItems[label];
			string item("");
			item.append(label);
			item.append(": ");
			item.append(newValue);

			overlayItem.itemInfo.swap(item);
			overlayItems[label] = overlayItem;
		}
	}
	void InfoOverlay::CreateOverlayItem(const char* label, const char* value)
	{
		int w, h;
		string item("");
		item.append(label);
		item.append(": ");
		item.append(value);
		TTF_SizeText(font, item.c_str(), &w, &h);
		this->overlaySurfaceH += h;
		overlayItems.emplace(std::make_pair(label, OverlayItem(item, h, w)));
	}
	Overlay* InfoOverlay::UpdateOverlay()
	{
		if (overlaySurface == nullptr)
		{
#if defined(OS_STEAMLINK)
			overlaySurface = SDL_CreateRGBSurfaceWithFormat(0, this->overlaySurfaceW, this->overlaySurfaceH, 32, SDL_PIXELFORMAT_ARGB8888);
#else
			overlaySurface = SDL_CreateRGBSurface(0, this->overlaySurfaceW, this->overlaySurfaceH, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#endif
		}
		else { SDL_FillRect(overlaySurface, nullptr, SDL_MapRGB(overlaySurface->format, 0, 0, 0)); }
		int i = 0, prevHeight = 0;
		SDL_Rect overlayItemRect;
		SDL_Surface* text;

		overlayItemRect.x = 0;
		for (const auto& pair : overlayItems)
		{
			overlayItemRect.x = 0;
			overlayItemRect.y = prevHeight;
			overlayItemRect.w = pair.second.width;
			overlayItemRect.h = pair.second.height;
			prevHeight += pair.second.height;
			if (i == 0) { overlayItemRect.y = 0; }

			text = GetTextSurface(pair.second.itemInfo.c_str());
			SDL_BlitSurface(text, NULL, overlaySurface, &overlayItemRect);
			SDL_FreeSurface(text);
			i++;
		}
		if (overlayData != nullptr)
		{
			overlayData->Reset(overlaySurface->pitch, overlaySurface, overlaySurfaceW, overlaySurfaceH);
		}
		else
		{
			overlayData = new Overlay(overlaySurface->pitch, overlaySurface, overlaySurfaceW, overlaySurfaceH);
		}
		return overlayData;
	}
	SDL_Surface* InfoOverlay::GetTextSurface(const char* text)
	{
		SDL_Color fg = { 255, 255, 255 };
		SDL_Color bg = { 0, 0, 0 };
		return TTF_RenderText_Shaded(font, text, fg, bg);
	}
}}
