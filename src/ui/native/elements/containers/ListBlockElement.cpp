/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <map>
#include <typeinfo>
#include <platform/PlatformManager.h>
#include <ui/native/elements/LabelElement.h>
#include <ui/native/rendering/NanoVGRenderer.h>
#include <ui/native/elements/containers/ListBlockElement.h>

using namespace player3::ui;
using namespace player3::platform;

namespace player3 { namespace ui
{
	ELEMENT_CTOR(ListBlockElement)
	{
		ElementStyle = style;
		defaultLabelStyle.FontSize = 12;
		defaultLabelStyle.FGColor.assign("rgba(255, 255, 255, 255)");
		for (const PropertyBinding p : bindings)
		{
			if (p.PropertyName == "anchor") { anchorPropertyBinding.assign(p.BindingName); }
			else if (p.PropertyName == "items") { listItemsPropertyBinding.assign(p.BindingName); }
		}
		screenSize = PlatformManager::Get()->GetPlatformInterface()->GetScreenSize();
	}
	void ListBlockElement::BindProperties(std::map<std::string, boost::any> bindingValues)
	{
		if (bindingValues.find(anchorPropertyBinding.c_str()) != bindingValues.end())
		{
			if (bindingValues[anchorPropertyBinding.c_str()].type() == typeid(AnchorPoint))
			{
				this->anchor = boost::any_cast<AnchorPoint>(bindingValues[anchorPropertyBinding.c_str()]);
			}
		}
		if (bindingValues.find(listItemsPropertyBinding.c_str()) != bindingValues.end())
		{
			if (bindingValues[listItemsPropertyBinding.c_str()].type() == typeid(std::string))
			{
				if (this->Children.size() <= this->GetMaxItems())
				{
					this->AddChildItems(bindingValues[listItemsPropertyBinding.c_str()]);
				}
				else {  }
			}
		}
	}
	void ListBlockElement::Measure()
	{
		Box* bounds = new Box();
		switch (anchor)
		{
			case AnchorPoint::BottomLeft:
				bounds->X = 0;
				bounds->Y = screenSize[1] - this->ElementStyle.Height;
				break;
			case AnchorPoint::BottomRight:
				bounds->X = screenSize[0] - this->ElementStyle.Width;
				bounds->Y = screenSize[1] - this->ElementStyle.Height;
				break;
			case AnchorPoint::TopLeft:
				bounds->Y = screenSize[1] - this->ElementStyle.Height;
				break;
			case AnchorPoint::TopRight:
				bounds->X = screenSize[0] - this->ElementStyle.Width;
				bounds->Y = 0;
				break;
			case AnchorPoint::None:
				break;
		}
		bounds->Height = this->ElementStyle.Height;
		bounds->Width = this->ElementStyle.Width;
		this->SetBoundingBox(bounds);
	}
	void ListBlockElement::ArrangeChildren()
	{
		// Note: X-coord on children is not set here because when children are
		// rendered, they're all rendered into a viewport set to the bounds of it's parent,
		// meaning
		Box* elementBounds;
		int previousHeight = 0;
		for (std::unique_ptr<ElementBase> const& e : this->Children)
		{
			e->Measure();
			elementBounds = e->GetBoundingBox();
			elementBounds->X = 0;
			elementBounds->Width = this->ElementStyle.Width;
			elementBounds->Y = previousHeight;
			e->SetBoundingBox(elementBounds);
			previousHeight += elementBounds->Height + 5;
		}
	}
	void ListBlockElement::Render()
	{
		Box* bounds = this->GetBoundingBox();
		NanoVGRenderer::Get()->DrawRectangle(bounds->X, bounds->Y, bounds->Width, bounds->Height, this->ElementStyle.BGColor.c_str());
		NanoVGRenderer::Get()->SetViewport(bounds);
		for (std::unique_ptr<ElementBase> const& e : this->Children)
		{
			e->Render();
		}
		NanoVGRenderer::Get()->Present();

	}
	int ListBlockElement::GetChildWidth() { return this->ElementStyle.Width; }
	int ListBlockElement::GetChildHeight() { return 0; }
	UPTR(LabelElement) ListBlockElement::CreateChildElement(std::string elementValue)
	{
		UPTRVAR(Label, LabelElement) = std::make_unique<LabelElement>(defaultLabelStyle, std::vector<PropertyBinding>());
		Label->SetText(elementValue);
		return Label;
	}
	void ListBlockElement::AddChildItems(boost::any itemValue)
	{
		std::string item = boost::any_cast<std::string>(itemValue);
		this->Children.push_back(this->CreateChildElement(item));
	}
}}