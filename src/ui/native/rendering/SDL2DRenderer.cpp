/*
* Copyright (c) 2016 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/Utils.h>
#include <sdl/include/SDL_image.h>
#include <ui/native/rendering/SDL2DRenderer.h>

using namespace base::utils;

namespace player3 { namespace ui
{
	std::shared_ptr<SDL2DRenderer> SDL2DRenderer::ref;

	void SDL2DRenderer::RendererInit(SDL_Window* window)
	{
		int winW, winH;
		SDL_GetWindowSize(window, &winW, &winH);

		defaultViewport = new Box(0, 0, winW, winH);
		this->rendererRef = SDL_CreateRenderer(window, -1,
											   SDL_RENDERER_ACCELERATED |
											   SDL_RENDERER_PRESENTVSYNC);
		if (this->rendererRef != nullptr)
		{
			if (TTF_Init() != 0) { Log("SDL2D::Init", "Failed to init SDL_TTF: %s", TTF_GetError()); }
			else
			{
				const char* fontPath = "NotoSans-Regular.ttf";
				this->currentFont = TTF_OpenFont(fontPath, 18);
				if (this->currentFont == NULL) { Log("SDL2D::Init", "Font load failed: %s", TTF_GetError()); }
			}
			if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) { Log("SDL2D::Init", "Failed to init image loader"); }
		}
		else
		{
			 Log("SDL2D::Init", "Failed to init renderer.");
			 SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Renderer init failed.", "Unable to init renderer. This is a bug.", nullptr);
		}
	}
	void SDL2DRenderer::Clear()
	{
		writeToLog("SDL2D::Clear()");
		SDL_RENDER_CALL0(Clear);
	}
	void SDL2DRenderer::Present()
	{
		writeToLog("SDL2D::Present()");
		SDL_RENDER_CALL0(Present);
	}
	void SDL2DRenderer::DrawRectangle(Box* rect, const char* color)
	{
		SDL_Color actualColor = this->ConvertStringToSkColor(std::string(color));

	    SDL_SetRenderDrawColor(this->rendererRef, actualColor.r, actualColor.g,
												  actualColor.b, actualColor.a);
		SDL_Rect* rectangle = ConvertRectToSDL(rect);
		SDL_RENDER_CALL1(FillRect, rectangle);
	}
	void SDL2DRenderer::DrawString(TextLayout* textLayout)
	{
		Log("SDL2d", "%s", textLayout->text);

		SDL_Color fg, bg;
		bg.a = 0;
		bg.r = 0;
		bg.g = 0;
		bg.b = 0;
	    SDL_Surface* textSurface;
		if (this->currentFont != nullptr)
		{
			fg = this->ConvertStringToSkColor(textLayout->foreground);
			textSurface = TTF_RenderText_Shaded(currentFont, textLayout->text, fg, bg);
			if (textSurface)
			{
				writeToLog("Look like we got a text surface. This seems really inefficient");
				SDL_Texture* texture = nullptr;
				texture = SDL_CreateTextureFromSurface(this->rendererRef, textSurface);
				SDL_RENDER_CALL3(Copy, texture, NULL, ConvertRectToSDL(textLayout->position));
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(textSurface);
			}
			else { Log("SDL2D::DrawString()", "error: %s", SDL_GetError()); }
		}
		else { Log("SDL2D", "font is nullptr"); }
	}
	void SDL2DRenderer::SetViewport(Box* rect)
	{
		Log("SDL2D::SetViewPort()", "new viewport: x = %i, y = %i, w = %i, h = %i",
									rect->X, rect->Y, rect->Width, rect->Height);

		SDL_RENDER_CALL1(SetViewport, ConvertRectToSDL(rect));
	}
	Box* SDL2DRenderer::GetViewport()
	{
		return currentViewport;
	}
	void SDL2DRenderer::ResetViewport()
	{
		currentViewport = defaultViewport;
		SDL_RENDER_CALL1(SetViewport, ConvertRectToSDL(defaultViewport));
	}
	void SDL2DRenderer::SetWindowBackgroundColor(SkylightColor* color)
	{
		Log("SDL2D::SetBGColor()", "Changing window BG color %i, %i, %i, %i", color->r, color->g, color->b);
		SDL_SetRenderDrawColor(this->rendererRef, color->r, color->g, color->b, color->a);
		SDL_RENDER_CALL0(Clear);
	}
	void SDL2DRenderer::DrawImage(std::string imagePath, Box* dst)
	{
		if (IMG_isPNG(SDL_RWFromFile(imagePath.c_str(), "rb")))
		{
			SDL_Texture* texture = nullptr;
			SDL_Surface* surface = IMG_Load(imagePath.c_str());
			if (surface != nullptr)
			{
				texture = SDL_CreateTextureFromSurface(this->rendererRef, surface);
				SDL_FreeSurface(surface);
				SDL_RENDER_CALL3(Copy, texture, NULL, ConvertRectToSDL(dst));
				SDL_DestroyTexture(texture);
			}
			else { Log("SDLInterface::DrawImage()", "Draw image failed. %s", IMG_GetError()); }
		}
		else { Log("SDLInterface::DrawImage()", "Specified image is not a PNG."); }
	}
	SDL_Color SDL2DRenderer::ConvertColorToSDL(SkylightColor color)
	{
		SDL_Color sdlColor;
		sdlColor.r = color.r;
		sdlColor.g = color.g;
		sdlColor.b = color.b;
		sdlColor.a = color.a;
		return sdlColor;
	}
	SDL_Rect* SDL2DRenderer::ConvertRectToSDL(Box* rect)
	{
		SDL_Rect* rectangle = new SDL_Rect();
		rectangle->x = rect->X;
		rectangle->y = rect->Y;
		rectangle->w = rect->Width;
		rectangle->h = rect->Height;
		return rectangle;
	}
	std::vector<int> SDL2DRenderer::MeasureText(std::string text, int width, int textSize, int boxWidth)
	{
		int w = 0, h = 0;
		if (TTF_SizeText(currentFont, text.c_str(), &w, &h) != -1)
		{
			return std::vector<int> { w, h };
		}
		return std::vector<int>();
	}
	SDL_Color SDL2DRenderer::ConvertStringToSkColor(std::string str)
	{
		SDL_Color actualColor;
		str.replace(0, 5, "");
		str.replace(str.end() - 1, str.end(), "");
		std::vector<std::string> colorParts = split(str, ',');

		if (colorParts.empty() == false && colorParts.size() == 4)
		{
			actualColor.r = std::stoi(colorParts[0]);
			actualColor.g = std::stoi(colorParts[1]);
			actualColor.b = std::stoi(colorParts[2]);
			actualColor.a = 255;//std::stoi(colorParts[3]);
			return actualColor;
		}
		return SDL_Color();
	}
}}