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
		this->SetBoundingBox(new Box(100, 100, 100, 100));
		this->SetElementStyle(style);
	}
	void LabelElement::Measure()
	{
		Box* bounds = this->GetBoundingBox();
		TextMeasurement* textMeasure = NanoVGRenderer::Get()->MeasureText(std::string(this->GetText()), bounds->Y);
		bounds->Height = textMeasure->height;
		this->SetBoundingBox(bounds);
  	}
	void LabelElement::Render()
	{
		Box* bounds = this->GetBoundingBox();
		Style style = this->GetElementStyle();
		//const char* color = this->GetElementStyle().FGColor.c_str();
		Log("i fucking hate this shit", "%s", this->GetElementStyle().FGColor.c_str());

	//	Log("LABEL_RENDER", "%s, %i, %i, %s", this->GetText().c_str(), bounds->X, bounds->Y, color);

		NanoVGRenderer::Get()->DrawString(this->GetText().c_str(), this->GetElementStyle().FGColor.c_str(), bounds->X, bounds->Y, 12);
	}
}}