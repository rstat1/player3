/*
* Copyright (c) 2016-2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef SDL2DRENDER
#define SDL2DRENDER

#include <string>
#include <vector>
#include <base/common.h>
#include <sdl/include/SDL_ttf.h>
#include <ui/native/rendering/RendererTypes.h>

#define SDL_RENDER_CALL0(type) SDL_Render##type(this->rendererRef);
#define SDL_RENDER_CALL1(type, arg1) SDL_Render##type(this->rendererRef, arg1);
#define SDL_RENDER_CALL2(type, arg1, arg2) SDL_Render##type(this->rendererRef, arg1, arg2);
#define SDL_RENDER_CALL3(type, arg1, arg2, arg3) SDL_Render##type(this->rendererRef, arg1, arg2, arg3);

#if defined(OS_STEAMLINK)
	#define SDL_FLAGS SDL_WINDOW_FULLSCREEN //| SDL_WINDOW_OPENGL
#elif defined(OS_LINUX) || defined(OS_WIN)
	#define SDL_FLAGS SDL_WINDOW_RESIZABLE //| SDL_WINDOW_OPENGL
#endif

namespace player3 { namespace ui
{
	class SDL2DRenderer
	{
		public:
			void Clear();
			void Present();
			void ResetViewport();
			Box* GetViewport();
			void RendererInit(SDL_Window* window);
		  	void DrawString(TextLayout* textLayout);
			void SetViewport(Box* rect);
			void DrawRectangle(Box* rect, const char* color);
			std::vector<int> MeasureText(std::string text, int width, int textSize, int boxWidth);
			void SetWindowBackgroundColor(SkylightColor* color);
			const char* GetRendererType() { return "SDL 2D Renderer"; }
			void DrawImage(std::string imagePath, Box* dst);
		private:
			SDL_Rect* ConvertRectToSDL(Box* rect);
			SDL_Color ConvertColorToSDL(SkylightColor color);
			SDL_Color ConvertStringToSkColor(std::string str);

			Box* currentViewport, *defaultViewport;
			SDL_Renderer *rendererRef;
			TTF_Font *currentFont;

		SINGLETON(SDL2DRenderer)
	};
}}

#endif
