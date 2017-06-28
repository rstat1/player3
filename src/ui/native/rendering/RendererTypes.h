/*
* Copyright (c) 2016 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/


#ifndef RNDERTYPS
#define RNDERTYPS

#include <stdint.h>

namespace player3 { namespace ui
{
	struct SkylightColor
	{
		public:
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
	};
	struct SkylightRectangle
	{
		public:
			int x;
			int y;
			int w;
			int h;
			bool filledBG;
			SkylightColor background;
	};
	enum TextStyle {
		TextStyle_Normal = 0,
		TextStyle_Bold = 1,
		TextStyle_Italic = 2,
		TextStyle_Underline = 3,
		TextStyle_Strikethrough = 4,
		TextStyle_MIN = TextStyle_Normal,
		TextStyle_MAX = TextStyle_Strikethrough
	};
	enum TextDrawingMode {
		TextDrawingMode_DrawString = 0,
		TextDrawingMode_DrawShadedString = 1,
		TextDrawingMode_DrawBlendedString = 2,
		TextDrawingMode_MIN = TextDrawingMode_DrawString,
		TextDrawingMode_MAX = TextDrawingMode_DrawBlendedString
	};
	struct TextLayout
	{
		public:
			const char* text;
			int fontSize;
			SkylightRectangle* position;
			TextStyle textStyle;
			SkylightColor background;
			SkylightColor foreground;
			TextDrawingMode drawingStyle;
	};
	struct TextMeasurement
	{
		public:
			int width;
			int height;
	};
}}

#endif