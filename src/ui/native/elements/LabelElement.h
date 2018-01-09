/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef LABELELM
#define LABELELM

#include <ui/native/elements/LayoutManagerBase.h>

namespace player3 { namespace ui
{
	class LabelElement : public ElementBase
	{
		public:
			ELEMENT_BASE(LabelElement, ElementType::Label);
			void Render() override;
			void Measure() override;
			void BindProperties(std::map<std::string, boost::any> bindingValues) override;

			PROPERTY(Text, std::string)
		private:
			std::string textPropertyBinding;
	};
}}

#endif