/*
* Copyright (c) 2016 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/Utils.h>
#include <ui/native/rendering/NanoVGDefines.h>
#include <ui/native/rendering/NanoVGRenderer.h>

using namespace base::utils;

namespace player3 { namespace ui
{
	std::shared_ptr<NanoVGRenderer> NanoVGRenderer::ref;

	void NanoVGRenderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void NanoVGRenderer::Present()
	{
		SDL_GL_SwapWindow(this->win);
	}
	void NanoVGRenderer::RendererInit(SDL_Window* window)
	{
		this->win = window;
 		glContext = SDL_GL_CreateContext(window);

		if (glContext == NULL) { Log("NanoVG::Init", "gl ctx create failed: %s", SDL_GetError()); }
		else
		{
#if !defined(OS_STEAMLINK)
			GLenum err = glewInit();
			if (err != GLEW_OK)
			{
				Log("NanoVG::Init", "glew init failed. %s", glewGetErrorString(err));
			}
#endif
			int mkCurRet = SDL_GL_MakeCurrent(window, glContext);
			if (mkCurRet < 0)  { Log("NanoVG::Init", "gl mkcur failed: %s", SDL_GetError()); }

			GLint dims[4] = {0};
			glGetIntegerv(GL_VIEWPORT, dims);
			this->winW = dims[2];
			this->winH = dims[3];

			NANOVG_INIT
#if defined(OS_STEAMLINK)
			NVG_RENDER2(CreateFont, "sans", "/usr/share/fonts/noto/NotoSans-Regular.ttf");
#else
			NVG_RENDER2(CreateFont, "sans", "NotoSans-Regular.ttf");
#endif
			if (context == nullptr) { Log("NanoVG::Init", "Failed to create NanoVG ctx"); }
		}
	}
	void NanoVGRenderer::DrawRectangle(int x, int y, int w, int h, const char* color)
	{
		SkylightColor* actualColor = this->ConvertStringToSkColor(std::string(color));

		if (actualColor != nullptr)
		{
			Log("NVGR", "new rect %i, %i, %i %i, color = %i, %i, %i, %i", x, y, w, h,
				actualColor->r, actualColor->g, actualColor->b, actualColor->a);
			NVGpaint rectPaint;

			NVG_RENDER3(BeginFrame, this->winW, this->winH, 1)

			NVG_RENDER0(BeginPath)
			NVG_RENDER4(Rect, x, y, w, h)
			NVG_RENDER1(FillColor, nvgRGBA(actualColor->r, actualColor->g, actualColor->b, actualColor->a))
			NVG_RENDER0(Fill)

			NVG_RENDER0(EndFrame)
		}
	}
	void NanoVGRenderer::DrawString(const char* text, const char* color, int x, int y, int textSize)
	{
		Log("NANOVG_TEXTCOLOR", "%i", currentViewport->Y);

		SkylightColor* actualColor = this->ConvertStringToSkColor(std::string(color));

		NVG_RENDER3(BeginFrame, this->winW, this->winH, 1)
		//NVG_RENDER4(Scissor, (float)currentViewport->X, (float)currentViewport->Y, (float)currentViewport->Width, (float)currentViewport->Height);

		NVG_RENDER1(FontSize, PointsToPixels(textSize));
		NVG_RENDER1(FontFace, "sans");
		NVG_RENDER1(FillColor, nvgRGBA(actualColor->r, actualColor->g, actualColor->b, actualColor->a));
		NVG_RENDER1(TextAlign, NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE)
		NVG_RENDER4(Text, x, y, text, NULL)

		NVG_RENDER0(EndFrame)
	}
	void NanoVGRenderer::DrawImage(std::string imagePath, Box* dst)
	{
		float ix,iy,iw,ih, tx, ty, v, a;
		int imgw, imgh;

		Log("NanoVG", "NanoVGRenderer::DrawImage(%s)", imagePath.c_str());

		NVG_RENDER3(BeginFrame, this->winW, this->winH, 1)

		NVG_RENDER0(BeginPath)
		NVG_RENDER2_R2(CreateImage, imagePath.c_str(), 0);
		NVG_RENDER3(ImageSize, r2, &imgw, &imgh);
		tx = dst->X;
		ty = dst->Y;
		if (imgw < imgh)
		{
			iw = (float)imgw;
			ih = iw * (float)imgh / (float)imgw;
			ix = 0;
			iy = -(ih - (float)imgw);
		}
		else
		{
			ih = (float)imgh;
			iw = ih * (float)imgw / (float)imgh;
			ix = -(iw - (float)imgh);
			iy = 0;
		}
		NVG_RENDER7_R7(ImagePattern, tx + ix, ty + iy, iw, ih, 0.0f, r2, 1)
		NVG_RENDER4(Rect, dst->X, dst->Y, dst->Width, dst->Height)
		NVG_RENDER1(FillPaint, r7);
		NVG_RENDER0(Fill)

		NVG_RENDER0(EndFrame)

		NVG_RENDER1(DeleteImage, r2)
	}
	Box* NanoVGRenderer::GetViewport()
	{
		return nullptr;
	}
	void NanoVGRenderer::SetViewport(Box* newViewport)
	{
		Log("NanoVG", "Setting viewport: (%i, %i, %i, %i)", newViewport->X, newViewport->Y, newViewport->Width, newViewport->Height);
		currentViewport = newViewport;

		NVG_RENDER3(BeginFrame, newViewport->Width, newViewport->Height, 1)
		NVG_RENDER4(Scissor, (float)newViewport->X, (float)newViewport->Y, (float)newViewport->Width, (float)newViewport->Height);
		NVG_RENDER0(EndFrame)
	}
	void NanoVGRenderer::ResetViewport()
	{
		NVG_RENDER0(ResetScissor);
	}
	void NanoVGRenderer::SetWindowBackgroundColor(SkylightColor* color)
	{
		Log("NanoVG::SetBGColor()", "Changing window BG color %i, %i, %i, %i", color->r, color->g, color->b, color->a);

		float R = (float)color->r / 255;
		float G = (float)color->g / 255;
		float B = (float)color->b / 255;
		float A = (float)color->a / 255;
		glClearColor(R, G, B, A);
		Clear();
	}
	TextMeasurement* NanoVGRenderer::MeasureText(std::string text, int width)
	{
		Log("NANOVG_TM", "%s", text.c_str());
		TextMeasurement* textMeasure = new TextMeasurement();
		float bounds[4] = {0};

		NVG_RENDER1(FontSize, PointsToPixels(16));
		NVG_RENDER1(FontFace, "sans");

		NVG_RENDER1(TextAlign, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		NVG_RENDER1(TextLineHeight, 1.2f);
		NVG_RENDER6(TextBoxBounds, 0, 0, 400, text.c_str(), nullptr, bounds)
		if (bounds != nullptr)
		{
			textMeasure->width = bounds[2] - bounds[0];
			textMeasure->height = bounds[3] + bounds[1];

			Log("NanoVG::MeasureText", "%f, %f, %f, %f", bounds[0], bounds[1], bounds[2], bounds[3]);

			return textMeasure;
		}
		return nullptr;
	}
	double NanoVGRenderer::PointsToPixels(int points)
	{
		return points * (96.0 / 72.0);

	}
	SkylightColor* NanoVGRenderer::ConvertStringToSkColor(std::string str)
	{
		SkylightColor* actualColor;
		str.replace(0, 5, "");
		str.replace(str.end() - 1, str.end(), "");
		std::vector<std::string> colorParts = split(str, ',');


		if (colorParts.empty() == false && colorParts.size() == 4)
		{
			Log("UI", "colorParts count = %i", colorParts.size());
			actualColor = new SkylightColor();
			actualColor->r = std::stoi(colorParts[0]);
			actualColor->g = std::stoi(colorParts[1]);
			actualColor->b = std::stoi(colorParts[2]);
			actualColor->a = std::stoi(colorParts[3]);
			return actualColor;
		}
		else return nullptr;
	}
}}