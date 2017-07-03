/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef ELEMBASE
#define ELEMBASE

#include <vector>
#include <memory>

namespace player3 { namespace ui
{
	enum ElementType
	{
		Image,
		Label
	};
	enum AnchorPoint
	{
		None,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};
	struct Box
	{
		public:
			int Left;
			int Right;
			int Top;
			int Bottom;
			Box()
			{
				Left = 0;
				Right = 0;
				Top = 0;
				Bottom = 0;
			}
			Box(int left, int right, int top, int bottom)
				: Left(left), Right(right), Top(top), Bottom(bottom)
			{}
	};
	struct Style
	{
		public:
			int width;
			int height;
			Box margin;
			Box padding;
			int fontSize;
			AnchorPoint anchor;
			const char* bgColor;
			const char* fgColor;
	};
	struct PropertyBinding
	{
		public:
			const char* propertyName;
			const char* bindingName;
	};
	class ElementBase
	{
		public:
			virtual Box GetBoundingBox() = 0;
			virtual ElementType GetElementType() = 0;
	};
	struct Layout
	{
		public:
			Style style;
			const char* name;
			std::vector<std::unique_ptr<ElementBase>> childElements;
	};
}}

#endif