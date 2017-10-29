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
		ElementStyle = style;
		//TODO: Don't hard code this here
		screenSize = {1280, 720};
	}
	void BlockElement::Render()
	{

	}
	void BlockElement::Measure()
	{
		Box* bounds = this->GetBoundingBox();
		std::string vAlign = this->GetVerticalAlignment();
		std::string hAlign = this->GetHorizontalAlignment();

		if (hAlign == "Fill") { bounds->Width = screenSize[0]; }
		else if (hAlign == "Top") {}
		else if (hAlign == "Bottom") {}
		else if (hAlign == "Center") { bounds->X = (screenSize[0] - this->ElementStyle.Width) / 2; }

		if (vAlign == "Fill") { bounds->Height = screenSize[1]; }
		else if (vAlign == "Left") {}
		else if (vAlign == "Right") {}
		else if (vAlign == "Center") { bounds->Y = (screenSize[1] - this->ElementStyle.Height) / 2; }
	}
}}