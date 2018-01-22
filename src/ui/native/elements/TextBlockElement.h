/*
* Copyright (c) 2018 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef TXTBLK
#define TXTBLK

#include <ui/native/elements/LayoutManagerBase.h>

namespace player3 { namespace ui
{
	class TextBlockElement: public ElementBase
	{
		public:
			ELEMENT_BASE(TextBlockElement, ElementType::TextBlock);
			void Measure() override;
			void Render() override;

			PROPERTY(MaxWidth, int)
			PROPERTY(Text, std::string)
	};
}}

#endif