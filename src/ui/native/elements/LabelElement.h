/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef LABELELM
#define LABELELM

#include <ui/native/elements/ElementBase.h>

namespace player3 { namespace ui
{
	class LabelElement : public ElementBase
	{
		public:
			LabelElement() {}
			LabelElement(Style style, std::vector<PropertyBinding> bindings);
			Box GetBoundingBox() override;
			ElementType GetElementType() override;
		private:
			Style style;
			Box boundingBox;
			ElementType elementType;
	};
}}

#endif