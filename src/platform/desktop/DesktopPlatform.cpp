/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/common.h>
#include <platform/desktop/DesktopPlatform.h>

namespace player3 { namespace platform
{
	DesktopPlatform::DesktopPlatform() {}
	std::vector<int> DesktopPlatform::GetScreenSize()
	{
		std::vector<int> size { 1024, 768 };
		return size;
	}
	void DesktopPlatform::CreateOverlay(int w, int h)
	{
		surfW = w;
		surfH = h;
		// this->win = SDL_CreateWindow("skylight", SDL_WINDOWPOS_UNDEFINED,
		// 										 SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
		// this->render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
		// Log("DesktopPlatform", "%s", SDL_GetError());
		// SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		// SDL_RenderClear(this->render);
        // SDL_RenderPresent(render);
	}
	void DesktopPlatform::ShowOverlay(void* pixels, int pitch)
	{
		// SDL_Rect dest;
		// dest.x = 0;
		// dest.y = 0;
		// dest.w =  this->surfW;
		// dest.h = this->surfH;
		// void* mPixels;
        // int mPitch;
		// SDL_Surface* overlaySurf = SDL_CreateRGBSurfaceFrom(pixels,  this->surfW, this->surfH, 32, pitch,
		// 													0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
		// if (overlayTexture == nullptr)
		// {
		// 	overlayTexture = SDL_CreateTexture(this->render, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, this->surfW, this->surfH);
		// }
		// SDL_LockTexture(overlayTexture, NULL, &mPixels, &mPitch);
		// memcpy(mPixels, overlaySurf->pixels, overlaySurf->pitch * overlaySurf->h);
		// SDL_UnlockTexture(overlayTexture);
        // mPixels = nullptr;

		// SDL_FreeSurface(overlaySurf);
		// SDL_RenderCopy(render, overlayTexture, NULL, &dest);
		// SDL_RenderPresent(render);
	}
	void DesktopPlatform::DecoderReset() {}
	void DesktopPlatform::DecoderShutdown() {}
	bool DesktopPlatform::DecodeVideoFrame(uint8_t* data, int size) { return true; }
	int DesktopPlatform::GetAudioSampleCount() { return 2048; }
}}