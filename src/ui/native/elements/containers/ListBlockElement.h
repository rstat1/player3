/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef LSTBLKELM
#define LSTBLKELM

#include <ui/native/elements/LayoutManagerBase.h>

namespace player3 { namespace ui
{
	typedef std::unique_ptr<ElementBase> ElementBasePtr;
	class ListBlockElement : public ContainerElementBase
	{
		public:
			ELEMENT_BASE(ListBlockElement, ElementType::ListBlock);
			void Render() override;
			void Measure() override;
			int GetChildWidth() override;
			int GetChildHeight() override;
			void ArrangeChildren() override;
			bool AllowChildrenToSetWidth() override { return true; }
			bool AllowChildrenToSetHeight() override { return false; }
			void BindProperties(std::map<const char*, boost::any> bindingValues) override;
		private:
			Style defaultLabelStyle;
			std::unique_ptr<LabelElement> CreateChildElement(const char* elementValue);
			std::vector<int> screenSize;
			std::map<const char*, const char*> propertyBindings;

	};
}}

#endif