/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <ui/native/elements/containers/BlockElement.h>

namespace player3 { namespace ui
{
	ELEMENT_CTOR(BlockElement)
	{
		this->SetBoundingBox(new Box(0, 0, 0, 0));
	}
	void BlockElement::Render()
	{

	}
}}