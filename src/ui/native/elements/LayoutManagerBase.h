/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef ELEMBASE
#define ELEMBASE

#include <map>
#include <vector>
#include <memory>
#include <boost/any.hpp>
#include <ui/native/PropertyMacros.h>

namespace player3 { namespace ui
{
	#define UPTR(type) std::unique_ptr<type>
	#define UPTRVAR(name, type) std::unique_ptr<type> name
	#define MAKEUPTR(type, arg1, arg2) std::make_unique<type>(arg1, arg2);
	#define ELEMENT_CTOR(name) name::name(Style style, std::map<const char*, const char*> bindings)
	#define ELEMENT_BASE(name, type) name(Style style, std::map<const char*, const char*> bindings); \
									 ElementType GetElementType() override { return type; }

	enum ElementType
	{
		Image,
		Label,
		Block,
		ListBlock,
		ImageLabel,
		Invalid
	};
	enum AnchorPoint
	{
		None,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
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
	struct Style
	{
		public:
			int Width;
			int Height;
			Box Margin;
			Box Padding;
			int FontSize;
			const char* BGColor;
			const char* FGColor;
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
			virtual void Measure() {};
			virtual void Render() = 0;
			virtual ElementType GetElementType() = 0;
			virtual void BindProperties(std::map<const char*, boost::any> bindingValues) {};

			PROPERTY(BoundingBox, Box)
			PROPERTY(ElementStyle, Style)
	};
	class ContainerElementBase : public ElementBase
	{
		public:
			virtual int GetChildWidth() = 0;
			virtual int GetChildHeight() = 0;
			virtual void ArrangeChildren() = 0;
			virtual ElementType GetElementType() = 0;
			virtual bool AllowChildrenToSetWidth() = 0;
			virtual bool AllowChildrenToSetHeight() = 0;

			AnchorPoint anchor;
			Style ElementStyle;
			std::vector<std::unique_ptr<ElementBase>> Children;

			PROPERTY(MaxItems, int)
			PROPERTY(BoundingBox, Box)
			PROPERTY(ItemType, ElementType)
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