/* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef ELEMBASE
#define ELEMBASE

#include <map>
#include <deque>
#include <vector>
#include <memory>
#include <boost/any.hpp>
#include <ui/native/PropertyMacros.h>
#include <boost/smart_ptr/make_unique.hpp>
#include <ui/native/rendering/RendererTypes.h>

namespace player3 { namespace ui
{
	#define UPTR(type) std::unique_ptr<type>
	#define NEWUPTR(name, type, arg1, arg2) std::unique_ptr<type> name = MAKEUPTR(type, arg1, arg2);
	#define MAKEUPTR(type, arg1, arg2) boost::make_unique<type>(arg1, arg2);
	#define ELEMENT_CTOR(name) name::name(Style style, std::vector<PropertyBinding> bindings)
	#define ELEMENT_BASE(name, type) name(Style style, std::vector<PropertyBinding> bindings); \
									 ElementType GetElementType() override { return type; }

	enum ElementType
	{
		Image,
		Label,
		Block,
		ListBlock,
		ImageLabel,
		TextBlock,
		Invalid
	};
	enum AnchorPoint
	{
		Not,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
	};
	struct ElementMargin
	{
		public:
			int Top;
			int Bottom;
			int Left;
			int Right;
			ElementMargin()
			{
				Top = 0;
				Bottom = 0;
				Left = 0;
				Right = 0;
			}
			ElementMargin(int top, int bottom, int left, int right)
			{
				Top = top;
				Bottom = bottom;
				Left = left;
				Right = right;
			}
	};
	struct Style
	{
		public:
			int Width;
			int Height;
			int FontSize;
			std::string BGColor;
			std::string FGColor;
			ElementMargin Margin;
			ElementMargin Padding;
			std::string VerticalAlignment;
			std::string HorizontalAlignment;
	};
	struct PropertyBinding
	{
		public:
			std::string PropertyName;
			std::string BindingName;
	};
	class ElementBase
	{
		public:
			virtual void Measure() {};
			virtual void Render() = 0;
			virtual ElementType GetElementType() = 0;
			virtual void UpdatePropertyBinding(const char* name, boost::any newValue) {};
			virtual void BindProperties(std::map<std::string, boost::any> bindingValues) {};

			int GetScreenHeight() { return 720; }
			int GetScreenWidth() { return 1280; }

			PROPERTY(BoundingBox, Box*)
			PROPERTY(NeedsRender, bool);
			PROPERTY(ElementStyle, Style)
			PROPERTY_PTR(ParentBounds, Box)
			PROPERTY(VerticalAlignment, std::string)
			PROPERTY(HorizontalAlignment, std::string)
	};
	class ContainerElementBase : public ElementBase
	{
		public:
			virtual void Clear() = 0;
			virtual int GetChildWidth() = 0;
			virtual int GetChildHeight() = 0;
			virtual void ArrangeChildren() = 0;
			virtual ElementType GetElementType() = 0;
			virtual bool AllowChildrenToSetWidth() = 0;
			virtual bool AllowChildrenToSetHeight() = 0;

			Style ElementStyle;
			std::deque<std::unique_ptr<ElementBase>> Children;

			PROPERTY(Anchor, AnchorPoint)
			PROPERTY(ItemType, ElementType)
	};
	struct Layout
	{
		public:
			const char* name;
			std::string options;
			ContainerElementBase* rootElement;
			std::map<std::string, boost::any> currentBindings;
	};
}}

#endif