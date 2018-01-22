/*
* Copyright (c) 2018 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <ui/native/elements/ImageElement.h>
#include <ui/native/rendering/NanoVGRenderer.h>

namespace player3 { namespace ui
{
	ELEMENT_CTOR(ImageElement)
	{
		this->SetElementStyle(style);
	}
	void ImageElement::Measure()
	{
		int topMargin = 0;
		Box* bounds = new Box();

		if (this->GetElementStyle().Margin.Top != 0) { topMargin = this->GetElementStyle().Margin.Top; }

		if (this->GetElementStyle().HorizontalAlignment == "Center")
		{
			//bounds->X = (this->GetParentBounds()->Width - this->GetElementStyle().Width) / 2;
			bounds->X = (this->GetScreenWidth() - this->GetElementStyle().Width) / 2;
			Log("imgelem::measure", "screenwidth = %i, elementwidth = %i", this->GetScreenWidth(), this->GetElementStyle().Width);
		}
		if (this->GetElementStyle().VerticalAlignment == "Center")
		{
			bounds->Y = topMargin + (this->GetScreenHeight() - this->GetParentBounds()->Height) / 2;
		}

		bounds->Width = this->GetElementStyle().Width;
		bounds->Height = this->GetElementStyle().Height;
		this->SetBoundingBox(bounds);
	}
	void ImageElement::Render()
	{
		NanoVGRenderer::Get()->DrawImage(this->GetSource(), this->GetBoundingBox());
	}
}}