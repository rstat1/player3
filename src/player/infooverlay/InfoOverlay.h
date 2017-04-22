/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef INFOOVER
#define INFOOVER

#include <map>
#include <SDL.h>
#include <SDL_ttf.h>
#include <platform/Platforms.h>

namespace player3 { namespace overlay
{
	struct OverlayItem
	{
		public:
			SDL_Surface* pixels;
			const char* itemInfo;
			OverlayItem() {}
			OverlayItem(const char* itemText, SDL_Surface* surfaceContent) :
				itemInfo(std::move(itemText)),
				pixels(std::move(surfaceContent))
			{}
	};
	struct Overlay
	{
		public:
			SDL_Surface* overlay;
			int pitch, surfaceW, surfaceH;
			Overlay() {}
			Overlay(int overlayPitch, SDL_Surface* actualOverlay, int w, int h) :
				pitch(std::move(overlayPitch)),
				overlay(std::move(actualOverlay)),
				surfaceW(w),
				surfaceH(h)
			{}
	};
	class InfoOverlay
	{
		public:
			void InitOverlay();
			Overlay* UpdateOverlay();
			void UpdateDoubleValue(const char* label, double newValue);
			void AddLabeledDoubleValue(const char* label, double value);
		private:
			SDL_Surface* GetTextSurface(const char* text);

			TTF_Font* font = nullptr;
			SDL_Surface* overlaySurface;
			int overlaySurfaceW, overlaySurfaceH;
			std::map<const char*, OverlayItem> overlayItems;
	};
}}

#endif