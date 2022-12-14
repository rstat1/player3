/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef LSTBLKELM
#define LSTBLKELM

#include <ui/native/elements/LayoutManagerBase.h>

namespace player3 { namespace ui
{
	struct ListBlockItem
	{
		public:
			std::string content;
			ListBlockItem(std::string itemContent)
				: content(std::move(itemContent))
			{}
	};
	typedef std::unique_ptr<ElementBase> ElementBasePtr;
	class ListBlockElement : public ContainerElementBase
	{
		public:
			ELEMENT_BASE(ListBlockElement, ElementType::ListBlock);
			void Clear() override;
			void Render() override;
			void Measure() override;
			int GetChildWidth() override;
			int GetChildHeight() override;
			void ArrangeChildren() override;
			bool AllowChildrenToSetWidth() override { return true; }
			bool AllowChildrenToSetHeight() override { return false; }
			void BindProperties(std::map<std::string, boost::any> bindingValues) override;
		private:
			void AddChildItems(std::string itemValue);
			UPTR(TextBlockElement) CreateChildElement(std::string elementValue);

			bool containerDrawn = false;
			Style defaultLabelStyle;
//			std::vector<int> screenSize;
			std::string anchorPropertyBinding;
			std::string listItemsPropertyBinding;
			std::vector<PropertyBinding> propertyBindings;
			int previousHeight = 0, totalRequiredHeight = 0;

	};
}}

#endif