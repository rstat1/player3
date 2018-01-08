/*
* Copyright (c) 2018 The Incredibly Big Red Robot
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
		Box* bounds = new Box();

		if (this->GetElementStyle().HorizontalAlignment == "Center")
		{
			bounds->X = (this->GetParentBounds()->Width - this->GetElementStyle().Width) / 2;
		}
		if (this->GetElementStyle().VerticalAlignment == "Center")
		{
			bounds->Y = (this->GetScreenHeight() - this->GetParentBounds()->Height) / 2;
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