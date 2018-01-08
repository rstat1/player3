/*
* Copyright (c) 2018 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <ui/native/rendering/NanoVGRenderer.h>
#include <ui/native/elements/TextBlockElement.h>

namespace player3 { namespace ui
{
	ELEMENT_CTOR(TextBlockElement)
	{
		this->SetElementStyle(style);
	}
    void TextBlockElement::Measure() {
		int fontSize = 12;
		Box *bounds = this->GetBoundingBox();

		if (this->GetElementStyle().FontSize != 0) { fontSize = this->GetElementStyle().FontSize; }
		if (this->GetElementStyle().Width != 0) { bounds->Width = this->GetElementStyle().Width; }

		std::vector<int> measure = NanoVGRenderer::Get()->MeasureTextBox(std::string(this->GetText()), fontSize,
																		 bounds->Width);
		if (bounds->Width == 0) { bounds->Width = measure[0]; }

		bounds->Height = measure[1];
		this->SetBoundingBox(bounds);
	}
	void TextBlockElement::Render()
	{
		Box* bounds = this->GetBoundingBox();
		Style style = this->GetElementStyle();
		ElementMargin margin = style.Margin;

		int fontSize = 12;
		if (style.FontSize != 0) { fontSize = style.FontSize; }

		NanoVGRenderer::Get()->DrawMultiLineString(this->GetText().c_str(), this->GetElementStyle().FGColor.c_str(), bounds->X + margin.Left,
										  bounds->Y +  margin.Top, fontSize, bounds->Width);
	}
  }}