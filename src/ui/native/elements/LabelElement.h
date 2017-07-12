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
			void Measure() override;
			void Render() override;

			PROPERTY(Text, std::string)
	};
}}

#endif