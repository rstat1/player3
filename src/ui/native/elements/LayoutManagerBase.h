/*
* Copyright (c) 2017 The Incredibly Big Red Robot
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
	#define UPTRVAR(name, type) std::unique_ptr<type> name
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
	struct Style
	{
		public:
			int Width;
			int Height;
			Box Margin;
			Box Padding;
			int FontSize;
			std::string BGColor;
			std::string FGColor;
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

			PROPERTY(BoundingBox, Box*)
			PROPERTY(NeedsRender, bool);
			PROPERTY(ElementStyle, Style)
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
			ContainerElementBase* rootElement;
			std::map<std::string, boost::any> currentBindings;
	};
}}

#endif