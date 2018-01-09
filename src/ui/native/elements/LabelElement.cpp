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
		for (const PropertyBinding p : bindings)
		{
			if (p.PropertyName == "text") { textPropertyBinding.assign(p.BindingName); }
		}
	}
	void LabelElement::Measure()
	{
		int fontSize = 12;
		if (this->GetElementStyle().FontSize != 0) { fontSize = this->GetElementStyle().FontSize; }
		Box* bounds = this->GetBoundingBox();
		std::vector<int> measure = NanoVGRenderer::Get()->MeasureText(std::string(this->GetText()), fontSize);

        if (bounds->Width == NULL) { bounds->Width = measure[0]; }
		bounds->Height = measure[1];
        Log("labe", "%i", bounds->Height);
        this->SetBoundingBox(bounds);
  	}
	void LabelElement::Render()
	{
		Box* bounds = this->GetBoundingBox();
		Style style = this->GetElementStyle();
		ElementMargin margin = style.Margin;

		int fontSize = 12;
		if (style.FontSize != 0) { fontSize = style.FontSize; }

		NanoVGRenderer::Get()->DrawString(this->GetText().c_str(), this->GetElementStyle().FGColor.c_str(), bounds->X + margin.Left,
										  bounds->Y + bounds->Height + margin.Top, fontSize);//, bounds->Width);
	}
	void LabelElement::BindProperties(std::map<std::string, boost::any> bindingValues)
	{
		if (bindingValues.find(textPropertyBinding) != bindingValues.end())
		{
			this->SetText(boost::any_cast<std::string>(bindingValues[textPropertyBinding]));
		}
	}
}}