/*
* Copyright (c) 2016 An Incredibly Big Red Robot
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
	struct Box
	{
		public:
			int X;
			int Y;
			int Width;
			int Height;
			Box()
			{
				X = 0;
				Y = 0;
				Width = 0;
				Height = 0;
			}
			Box(int x, int y, int width, int height)
				: X(x), Y(y), Width(width), Height(height)
			{}
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
			Box* position;
			const char* text;
			const char* foreground;
			TextLayout(const char* str, const char* fg, Box* pos)
				: text(str), position(pos), foreground(fg)
			{}
	};
	struct TextMeasurement
	{
		public:
			int width;
			int height;
	};
}}

#endif