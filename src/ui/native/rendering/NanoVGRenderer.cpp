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
			defaultViewport = new Box(0, 0, this->winW, this->winH);
			currentViewport = defaultViewport;
			NANOVG_INIT
			if (context == nullptr)
			{
				Log("NanoVG::Init", "Failed to create NanoVG ctx");
				return;
			}
			NVG_RENDER2_R2(CreateFont, "sans", "SourceSansPro-Regular.otf");
			if (r2 == -1) { Log("NanoVG::Init", "Failed to create font."); }
		}
	}
	void NanoVGRenderer::DrawRectangle(int x, int y, int w, int h, const char* color)
	{
		PROFILE_GPU(DrawRectangle);
		SkylightColor* actualColor = this->ConvertStringToSkColor(std::string(color));

		if (actualColor != nullptr)
		{
			NVGpaint rectPaint;

			NVG_RENDER3(BeginFrame, defaultViewport->Width, defaultViewport->Height, 1)
			NVG_RENDER0(BeginPath)
			NVG_RENDER4(Rect, x, y, w, h)
			NVG_RENDER1(FillColor, nvgRGBA(actualColor->r, actualColor->g, actualColor->b, actualColor->a))
			NVG_RENDER0(Fill)
			NVG_RENDER0(EndFrame)
		}
	}
	void NanoVGRenderer::DrawMultiLineString(const char* text, const char* color, int x, int y, int textSize, int boxWidth)
	{
		PROFILE_GPU(DrawString);

		SkylightColor* actualColor = this->ConvertStringToSkColor(std::string(color));

		NVG_RENDER3(BeginFrame, defaultViewport->Width, defaultViewport->Height, 1)
	//	NVG_RENDER4(Scissor, x, y, x + (float)currentViewport->Width, (float)currentViewport->Height);

		NVG_RENDER1(FontSize, PointsToPixels(textSize));
		NVG_RENDER1(FontFace, "sans");
		NVG_RENDER1(TextLineHeight, 1.2f);
		NVG_RENDER1(FillColor, nvgRGBA(actualColor->r, actualColor->g, actualColor->b, actualColor->a));
		NVG_RENDER1(TextAlign, NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE)
		NVG_RENDER5(TextBox, x, y, boxWidth, text, NULL)
		NVG_RENDER0(EndFrame)

		delete actualColor;
	}
	void NanoVGRenderer::DrawString(const char *text, const char *color, int x, int y, int textSize)
	{
		SkylightColor* actualColor = this->ConvertStringToSkColor(std::string(color));

		NVG_RENDER3(BeginFrame, defaultViewport->Width, defaultViewport->Height, 1)
		//	NVG_RENDER4(Scissor, x, y, x + (float)currentViewport->Width, (float)currentViewport->Height);

		NVG_RENDER1(FontSize, PointsToPixels(textSize));
		NVG_RENDER1(FontFace, "sans");
		NVG_RENDER1(TextLineHeight, 1.2f);
		NVG_RENDER1(FillColor, nvgRGBA(actualColor->r, actualColor->g, actualColor->b, actualColor->a));
		NVG_RENDER1(TextAlign, NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE)
		NVG_RENDER4(Text, x, y, text, NULL)
		NVG_RENDER0(EndFrame)

		delete actualColor;
	}
	void NanoVGRenderer::DrawImage(std::string imagePath, Box* dst)
	{
		float ix,iy,iw,ih, tx, ty, v, a;
		int imgw, imgh;

		Log("NanoVG", "NanoVGRenderer::DrawImage(%s, (%i, %i, %i, %i))", imagePath.c_str(),
			dst->Width, dst->Height, dst->X, dst->Y);
		NVG_RENDER3(BeginFrame, defaultViewport->Width, defaultViewport->Height, 1)
		NVG_RENDER4(Scissor, currentViewport->X,currentViewport->Y, currentViewport->Width, currentViewport->Height);

		NVG_RENDER0(BeginPath)
		NVG_RENDER2_R2(CreateImage, imagePath.c_str(), 0);
		NVG_RENDER3(ImageSize, r2, &imgw, &imgh);
		tx = dst->X;
		ty = dst->Y;
		if (imgw < imgh)
		{
			iw = (float)dst->Width;
			ih = iw * (float)dst->Height / (float)dst->Width;
			ix = 0;
			iy = -(ih - (float)dst->Width);
		}
		else
		{
			ih = (float)dst->Height;
			iw = ih * (float)dst->Width / (float)dst->Height;
			ix = -(iw - (float)dst->Height);
			iy = 0;
		}
		NVG_RENDER7_R7(ImagePattern, tx + ix, ty + iy, iw, ih, 0.0f, r2, 1)
		NVG_RENDER4(Rect, dst->X, dst->Y, dst->Width, dst->Height)
		NVG_RENDER1(FillPaint, r7);
		NVG_RENDER0(Fill)

		NVG_RENDER0(EndFrame)

		NVG_RENDER1(DeleteImage, r2)
	}
	void NanoVGRenderer::DrawSDLSurface(void* pixels, int w, int h)
	{
		NVG_RENDER3(BeginFrame, this->winW, this->winH, 1);
		if (overlayID == 0)
		{
			NVG_RENDER3_R4(CreateImageRGBA, w, h, 0, static_cast<uint8_t*>(pixels));
			overlayID = r4;
		}
		else { NVG_RENDER2(UpdateImage, overlayID, static_cast<uint8_t*>(pixels)); }

		NVG_RENDER7_R7(ImagePattern, 0, 0, w, h, 0, overlayID, 1.0f);
		NVG_RENDER0(BeginPath)
		NVG_RENDER4(Rect, 100, 100, w, h);
		NVG_RENDER1(FillPaint, r7);
		NVG_RENDER0(Fill);
		NVG_RENDER0(EndFrame);
	}
	Box* NanoVGRenderer::GetViewport()
	{
		return currentViewport;
	}
	void NanoVGRenderer::SetViewport(Box* newViewport)
	{
		currentViewport = newViewport;
		// //NVG_RENDER3(BeginFrame, newViewport->Width, newViewport->Height, 1)
		// NVG_RENDER4(Scissor, (float)newViewport->X, (float)newViewport->Y, (float)newViewport->Width, (float)newViewport->Height);
		// //NVG_RENDER0(EndFrame)
	}
	void NanoVGRenderer::ResetViewport()
	{
		currentViewport = defaultViewport;
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
	std::vector<int> NanoVGRenderer::MeasureText(std::string text, int textSize)
	{
		std::vector<int> textMeasure;
		float bounds[4] = {0};
		float ascend, descend, lh;

		NVG_RENDER1(FontSize, PointsToPixels(textSize));
		NVG_RENDER1(FontFace, "sans");
		NVG_RENDER1(TextAlign, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		NVG_RENDER5_R5(TextBounds, 0, 0, text.c_str(), nullptr, bounds)

		if (bounds != nullptr)
		{
			textMeasure.push_back(bounds[2] - bounds[0]); //width
			textMeasure.push_back(bounds[3] - bounds[1]); //height
			return textMeasure;
		}
        else {
            textMeasure.push_back(r5);
        }
		return {};
	}
	std::vector<int> NanoVGRenderer::MeasureTextBox(std::string text, int textSize, int boxWidth)
	{
		std::vector<int> textMeasure;
		float bounds[4] = {0};

		NVG_RENDER1(FontSize, PointsToPixels(textSize));
		NVG_RENDER1(FontFace, "sans");
		NVG_RENDER1(TextAlign, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		NVG_RENDER6(TextBoxBounds, 0, 0, boxWidth, text.c_str(), nullptr, bounds)

		if (bounds != nullptr)
		{
			textMeasure.push_back(bounds[2] - bounds[0]); //width
			textMeasure.push_back(bounds[3] - bounds[1]); //height
			return textMeasure;
		}
		return {};

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