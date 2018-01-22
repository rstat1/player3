/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <ui/native/rendering/NanoVGRenderer.h>
#include <ui/native/elements/containers/BlockElement.h>
#include <ui/native/elements/ImageElement.h>

namespace player3 { namespace ui
{
	ELEMENT_CTOR(BlockElement)
	{
		ElementStyle = style;
	}
	void BlockElement::Render()
	{
		Box* bounds = this->GetBoundingBox();
		NanoVGRenderer::Get()->DrawRectangle(bounds->X, bounds->Y, bounds->Width, bounds->Height, this->ElementStyle.BGColor.c_str());
		if (this->Children.size() > 0)
		{
			for (std::unique_ptr<ElementBase> const& e : this->Children)
			{
				e->Render();
			}
		}
	}
	void BlockElement::ArrangeChildren()
	{
        Box* bounds;
		Style elementStyle;
		if (this->Children.size() > 0)
		{
			for (std::unique_ptr<ElementBase> const& e : this->Children)
			{
				e->SetParentBounds(this->GetBoundingBox());
				elementStyle = e->GetElementStyle();
				e->SetBoundingBox(new Box());
				e->Measure();
				if (e->GetElementType() != ElementType::Image)
				{
					bounds = this->GetElementBox(elementStyle.HorizontalAlignment, elementStyle.VerticalAlignment, e->GetBoundingBox()->Width);
					bounds->Width = this->GetBoundingBox()->Width;
					e->SetBoundingBox(bounds);
					e->Measure();
				}
            }
		}
	}
	void BlockElement::Measure()
	{
		Box* bounds;
		std::string vAlign = this->ElementStyle.VerticalAlignment;
		std::string hAlign = this->ElementStyle.HorizontalAlignment;

		bounds = this->GetElementBox(hAlign, vAlign, this->ElementStyle.Width);

        if (bounds->Height == 0) { bounds->Height = this->ElementStyle.Height; }
		if (bounds->Width == 0) { bounds->Width = this->ElementStyle.Width; }

        this->SetBoundingBox(bounds);
	}
	Box* BlockElement::GetElementBox(std::string halign, std::string valign, int width)
	{
		Box* bounds = new Box();

		if (halign == "Fill") { bounds->Width = this->GetScreenWidth(); }
		else if (halign == "Top") {}
		else if (halign == "Bottom") {}
		else if (halign == "Center") { bounds->X = (this->GetScreenWidth() - width) / 2; }

		if (valign == "Fill") { bounds->Height = this->GetScreenHeight(); }
		else if (valign == "Left") {}
		else if (valign == "Right") {}
		else if (valign == "Center") { bounds->Y = (this->GetScreenHeight() - this->ElementStyle.Height) / 2; }

		return bounds;
	}
	void BlockElement::BindProperties(std::map<std::string, boost::any> bindingValues)
	{
		if (this->Children.size() > 0)
		{
			for (std::unique_ptr<ElementBase> const &e : this->Children)
			{
				e->BindProperties(bindingValues);
			}
		}
	}
}}