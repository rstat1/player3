/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <ui/native/elements/LabelElement.h>

namespace player3 { namespace ui
{
	LabelElement::LabelElement(Style style, std::vector<PropertyBinding> bindings)
	{
		this->style = style;
	}
	Box LabelElement::GetBoundingBox() { return this->boundingBox; }
	ElementType LabelElement::GetElementType() { return ElementType::Label; }
}}