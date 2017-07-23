/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <ui/native/elements/LabelElement.h>
#include <ui/native/rendering/NanoVGRenderer.h>

namespace player3 { namespace ui
{
	ELEMENT_CTOR(LabelElement)
	{
		//this->SetBoundingBox(new Box(100, 100, 100, 100));
		this->SetElementStyle(style);
		this->SetNeedsRender(true);
	}
	void LabelElement::Measure()
	{
		Box* bounds = this->GetBoundingBox();
		std::vector<int> measure = NanoVGRenderer::Get()->MeasureText(std::string(this->GetText()), bounds->Y, 12, bounds->Width);
		bounds->Height = measure[1];
		this->SetBoundingBox(bounds);
  	}
	void LabelElement::Render()
	{
		Box* bounds = this->GetBoundingBox();
		Style style = this->GetElementStyle();
		NanoVGRenderer::Get()->DrawString(this->GetText().c_str(), this->GetElementStyle().FGColor.c_str(), bounds->X,
										  bounds->Y, 12, bounds->Width);
		//this->SetNeedsRender(false);
	}
}}