/*name
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
	#define ELEMENT_CTOR(name) name::name(Style style, std::vector<PropertyBinding> bindings)
	#define ELEMENT_BASE(name, type) name(Style style, std::vector<PropertyBinding> bindings); \
							   		 Box GetBoundingBox() override; \
									 ElementType GetElementType() override { return type; }

	enum ElementType
	{
		Image,
		Label,
		Block,
		ListBlock,
		Invalid
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
			const char* bgColor;
			const char* fgColor;
	};
	struct PropertyBinding
	{
		public:
			const char* PropertyName;
			const char* BindingName;
	};
	class ElementBase
	{
		public:
			virtual Box GetBoundingBox() = 0;
			virtual ElementType GetElementType() = 0;
	};
	class ContainerElementBase : public ElementBase
	{
		public:
			virtual bool AllowChildrenToSetWidth() = 0;
			virtual bool AllowChildrenToSetHeight() = 0;
			virtual int GetChildWidth() = 0;
			virtual int GetChildHeight() = 0;
			virtual Box GetBoundingBox() { return Box(0, 0, 0, 0); }
			virtual ElementType GetElementType() { return ElementType::Invalid; }

			AnchorPoint anchor;
			Style ElementStyle;
			std::vector<std::unique_ptr<ElementBase>> Children;
	};
	struct Layout
	{
		public:
			Style style;
			const char* name;
			ContainerElementBase* rootElement;
	};
}}

#endif