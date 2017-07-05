/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <ui/native/elements/containers/ListBlockElement.h>

namespace player3 { namespace ui
{
	ELEMENT_CTOR(ListBlockElement)
	{

	}
	int ListBlockElement::GetChildWidth() { return 0; }
	int ListBlockElement::GetChildHeight() { return 0; }
	Box ListBlockElement::GetBoundingBox() { return Box(0,0,0,0); }
}}