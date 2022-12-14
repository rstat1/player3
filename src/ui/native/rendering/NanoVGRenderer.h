/*
* Copyright (c) 2016 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef NANOVGRENDER
#define NANOVGRENDER

#include <SDL.h>
#include <string>
#include <memory>
#include <vector>
#include <base/common.h>
#include <ui/native/rendering/RendererTypes.h>

#define NVG_RENDER0(func) nvg##func(this->context);
#define NVG_RENDER1(func, arg1) nvg##func(this->context, arg1);
#define NVG_RENDER2(func, arg1, arg2) nvg##func(this->context, arg1, arg2);
#define NVG_RENDER3(func, arg1, arg2, arg3) nvg##func(this->context, arg1, arg2, arg3);
#define NVG_RENDER4(func, arg1, arg2, arg3, arg4) nvg##func(this->context, arg1, arg2, arg3, arg4);
#define NVG_RENDER5(func, arg1, arg2, arg3, arg4, arg5) nvg##func(this->context, arg1, arg2, arg3, arg4, arg5);
#define NVG_RENDER6(func, arg1, arg2, arg3, arg4, arg5, arg6) nvg##func(this->context, arg1, arg2, arg3, arg4, arg5, arg6);
#define NVG_RENDER7(func, arg1, arg2, arg3, arg4, arg5, arg6, arg7) nvg##func(this->context, arg1, arg2, arg3, arg4, arg5, arg6, arg7);

#define NVG_RENDER1_R1(func, arg1) auto r1 = nvg##func(this->context, arg1);
#define NVG_RENDER2_R2(func, arg1, arg2) auto r2 = nvg##func(this->context, arg1, arg2);
#define NVG_RENDER3_R4(func, arg1, arg2, arg3, arg4) auto r4 = nvg##func(this->context, arg1, arg2, arg3, arg4);
#define NVG_RENDER5_R5(func, arg1, arg2, arg3, arg4, arg5) auto r5 = nvg##func(this->context, arg1, arg2, arg3, arg4, arg5);
#define NVG_RENDER6_R6(func, arg1, arg2, arg3, arg4, arg5, arg6) auto r6 = nvg##func(this->context, arg1, arg2, arg3, arg4, arg5, arg6);
#define NVG_RENDER7_R7(func, arg1, arg2, arg3, arg4, arg5, arg6, arg7) auto r7 = nvg##func(this->context, arg1, arg2, arg3, arg4, arg5, arg6, arg7);

#if defined(OS_STEAMLINK)
	#define SDL_FLAGS SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
#elif (defined(OS_LINUX) && !defined(OS_STEAMLINK)) || defined(OS_WIN)
	#define SDL_FLAGS SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
#endif

class NVGcontext;

namespace player3 { namespace ui
{
	class NanoVGRenderer
	{
		public:
			void Clear();
			void Present();
			void ResetViewport();
			Box* GetViewport();
			void RendererInit(SDL_Window* window);
		  	void DrawString(const char* text, const char* color, int x, int y, int textSize);
			void DrawMultiLineString(const char* text, const char* color, int x, int y, int textSize, int boxWidth);
			void DrawRectangle(int x, int y, int w, int h, const char* color);
			std::vector<int> MeasureText(std::string text, int textSize);
			std::vector<int> MeasureTextBox(std::string text, int textSize, int boxWidth);
			void SetViewport(Box* newViewport);
			void SetWindowBackgroundColor(SkylightColor* color);
			void DrawImage(std::string imagePath, Box* dst);
			void DrawSDLSurface(void* pixels, int w, int h);
			void ClearViewport();
		private:
	  		double PointsToPixels(int points);
			SkylightColor* ConvertStringToSkColor(std::string str);

			SDL_Window* win;
			NVGcontext* context;
			SDL_GLContext glContext;
			int winW = 0, winH = 0, overlayID = 0;
			Box* currentViewport, *defaultViewport;

			SINGLETON(NanoVGRenderer)
	};
}}

#endif