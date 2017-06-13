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
#include <cstring>
#include <SDL_ttf.h>
#include <platform/Platforms.h>

using namespace std;

namespace player3 { namespace overlay
{
	struct OverlayItem
	{
		public:
			int width;
			int height;
			SDL_Surface* pixels;
			std::string itemInfo;
			OverlayItem() {}
			OverlayItem(std::string itemText, int h, int w) :
				//itemInfo(std::move(itemText)),
				height(h),
				width(w)
				//pixels(std::move(surfaceContent))
			{
				//itemInfo.copy
			}
			void SetPixels(SDL_Surface* surface) { pixels = std::move(surface); }
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
			void Reset(int overlayPitch, SDL_Surface* actualOverlay, int w, int h)
			{
				pitch = surfaceW = surfaceH = 0;
				//SDL_FreeSurface(overlay);
				overlay = nullptr;
				pitch = std::move(overlayPitch);
				overlay = std::move(actualOverlay);
				surfaceW = w;
				surfaceH = h;
			}
	};
	class InfoOverlay
	{
		public:
			void InitOverlay();
			Overlay* UpdateOverlay();
			void AddIntValue(const char* label, int value);
			void AddDoubleValue(const char* label, double value);
			void AddStringValue(const char* label, const char* value);
			
			void UpdateIntValue(const char* label, int newValue);
			void UpdateDoubleValue(const char* label, double newValue);
			void UpdateStringValue(const char* label, const char* newValue);
		private:
			SDL_Surface* GetTextSurface(const char* text);
			void CreateOverlayItem(const char* label, const char* value);
			void UpdateOverlayItem(const char* label, const char* newValue);

			Overlay* overlayData;
			TTF_Font* font = nullptr;
			SDL_Surface* overlaySurface;
			int overlaySurfaceW, overlaySurfaceH;
			std::map<const char*, OverlayItem> overlayItems;
	};
}}

#endif