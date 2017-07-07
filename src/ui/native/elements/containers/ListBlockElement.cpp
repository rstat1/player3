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
		screenSize = PlatformManager::Get()->GetPlatformInterface()->GetScreenSize();
	}
	void ListBlockElement::BindProperties(std::map<const char*, boost::any> bindingValues)
	{
		defaultLabelStyle.FGColor = "#FFFFFFFF";
		defaultLabelStyle.FontSize = 12;
		const char* anchorPropertyBinding = this->propertyBindings["anchor"];
		const char* listItemsPropertyBinding = this->propertyBindings["listitems"];

		if (anchorPropertyBinding != "")
		{
			if (bindingValues.find(anchorPropertyBinding) != bindingValues.end())
			{
				if (bindingValues[anchorPropertyBinding].type() == typeid(AnchorPoint))
				{
					this->anchor = boost::any_cast<AnchorPoint>(bindingValues[anchorPropertyBinding]);
				}
			}
		}
		if (bindingValues.find(listItemsPropertyBinding) != bindingValues.end())
		{
			if (bindingValues[listItemsPropertyBinding].type() == typeid(std::vector<const char*>))
			{
				std::vector<const char*> items = boost::any_cast<std::vector<const char*>>(bindingValues[listItemsPropertyBinding]);
				for (const char* listItem: items)
				{
					this->Children.push_back(this->CreateChildElement(listItem));
				}
			}
		}
	}
	void ListBlockElement::Measure()
	{
		Box bounds;
		switch (anchor)
		{
			case AnchorPoint::BottomLeft:
				bounds.X = 0;
				bounds.Y = screenSize[1] - this->ElementStyle.Height;
				break;
			case AnchorPoint::BottomRight:
				bounds.X = screenSize[0] - this->ElementStyle.Width;
				bounds.Y = screenSize[1] - this->ElementStyle.Height;
				break;
			case AnchorPoint::TopLeft:
				bounds.Y = screenSize[1] - this->ElementStyle.Height;
				break;
			case AnchorPoint::TopRight:
				bounds.X = screenSize[0] - this->ElementStyle.Width;
				bounds.Y = 0;
				break;
			case AnchorPoint::None:
				break;
		}
		bounds.Height = this->ElementStyle.Height;
		bounds.Width = this->ElementStyle.Width;
		this->SetBoundingBox(bounds);
	}
	void ListBlockElement::ArrangeChildren()
	{
		// Note: X-coord on children is not set here because when children are
		// rendered, they're all rendered into a viewport set to the bounds of it's parent.
		Box elementBounds;
		int previousHeight = 0;
		for (std::unique_ptr<ElementBase> const& e : this->Children)
		{
			elementBounds = e->GetBoundingBox();
			elementBounds.Width = this->ElementStyle.Width;
			elementBounds.Y = previousHeight;
			e->SetBoundingBox(elementBounds);
			e->Measure();
			previousHeight += elementBounds.Height + 5;
		}
	}
	void ListBlockElement::Render()
	{

	}
	int ListBlockElement::GetChildWidth() { return this->ElementStyle.Width; }
	int ListBlockElement::GetChildHeight() { return 0; }
	UPTR(LabelElement) ListBlockElement::CreateChildElement(const char* elementValue)
	{
		UPTRVAR(Label, LabelElement) = std::make_unique<LabelElement>(defaultLabelStyle, std::map<const char*, const char*>());
		Label->SetText(elementValue);
		return Label;
	}
}}