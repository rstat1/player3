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
		this->SetElementStyle(style);
	}
	void LabelElement::Measure()
	{
		Box bounds = this->GetBoundingBox();
		TextMeasurement* textMeasure = NanoVGRenderer::Get()->MeasureText(std::string(this->GetText()), bounds.Y);
		bounds.Height = textMeasure->height;
		bounds.X = 0;
		this->SetBoundingBox(bounds);
  	}
	void LabelElement::Render()
	{

	}
}}