/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <map>
#include <typeinfo>
#include <base/common.h>
#include <platform/PlatformManager.h>
#include <ui/native/elements/TextBlockElement.h>
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
		defaultLabelStyle.Width = this->ElementStyle.Width - 10;
		defaultLabelStyle.FGColor.assign("rgba(255, 255, 255, 255)");
		for (const PropertyBinding p : bindings)
		{
			if (p.PropertyName == "anchor") { anchorPropertyBinding.assign(p.BindingName); }
			else if (p.PropertyName == "items") { listItemsPropertyBinding.assign(p.BindingName); }
		}
	}
	void ListBlockElement::BindProperties(std::map<std::string, boost::any> bindingValues)
	{
		if (bindingValues.find(anchorPropertyBinding) != bindingValues.end())
		{
			if (bindingValues[anchorPropertyBinding].type() == typeid(AnchorPoint))
			{
				this->SetAnchor(boost::any_cast<AnchorPoint>(bindingValues[anchorPropertyBinding]));
			}
		}
		if (bindingValues.find(listItemsPropertyBinding) != bindingValues.end())
		{
			boost::any listItems = bindingValues[listItemsPropertyBinding];
			if (listItems.type() == typeid(std::string))
			{
				std::string item = boost::any_cast<std::string>(listItems);
				this->AddChildItems(item);
			}
			else if (listItems.type() == typeid(std::vector<ListBlockItem>))
			{
				std::vector<ListBlockItem> items = boost::any_cast<std::vector<ListBlockItem>>(listItems);
				for (ListBlockItem item : items)
				{
					this->AddChildItems(item.content);
				}
			}
		}
	}
	void ListBlockElement::Measure()
	{
		Box* bounds = new Box();
		switch (this->GetAnchor())
		{
			case AnchorPoint::BottomLeft:
				bounds->X = 0;
				bounds->Y = this->GetScreenHeight() - this->ElementStyle.Height;
				break;
			case AnchorPoint::BottomRight:
				bounds->X = this->GetScreenWidth() - this->ElementStyle.Width;
				bounds->Y = this->GetScreenHeight() - this->ElementStyle.Height;
				break;
			case AnchorPoint::TopLeft:
				bounds->X = 0;
				bounds->Y = 0;
				break;
			case AnchorPoint::TopRight:
				bounds->X = this->GetScreenWidth() - this->ElementStyle.Width;
				bounds->Y = 0;
				break;
			case AnchorPoint::Not:
				break;
		}
		bounds->Height = this->ElementStyle.Height;
		bounds->Width = this->ElementStyle.Width;
		this->SetBoundingBox(bounds);
	}
	void ListBlockElement::ArrangeChildren()
	{
		Box* elementBounds;
		int x, y, width, elementHeight;
		if (this->Children.size() > 0)
		{
			previousHeight = this->GetBoundingBox()->Y + 25;
			for (std::unique_ptr<ElementBase> const& e : this->Children)
			{
				x = this->GetBoundingBox()->X + 20;
				width = this->ElementStyle.Width - 20;
				elementBounds = e->GetBoundingBox();
				elementBounds->X = x;
				elementBounds->Y = previousHeight;
				elementBounds->Width = width;
				e->SetBoundingBox(elementBounds);
				previousHeight += elementBounds->Height + 5;
			}
		}
	}
	void ListBlockElement::Render()
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
	UPTR(TextBlockElement) ListBlockElement::CreateChildElement(std::string elementValue)
	{
		NEWUPTR(Label, TextBlockElement, defaultLabelStyle, std::vector<PropertyBinding>());
		Label->SetText(elementValue);
		Label->SetBoundingBox(new Box(0, 0, this->ElementStyle.Width - 20, 0));
		Label->Measure();
		totalRequiredHeight += Label->GetBoundingBox()->Height;
		return Label;
	}
	void ListBlockElement::AddChildItems(std::string itemValue)
	{
		int maxHeight = this->ElementStyle.Height - 80;
		this->Children.push_back(this->CreateChildElement(itemValue));

		if (totalRequiredHeight >= maxHeight)
		{
			for (std::unique_ptr<ElementBase> const& e : this->Children)
			{
				totalRequiredHeight -= this->Children.front()->GetBoundingBox()->Height;
				this->Children.pop_front();
				if (totalRequiredHeight < maxHeight ) { break; }
			}
		}
	}
	void ListBlockElement::Clear()
	{
		this->Children.clear();
		totalRequiredHeight = 0;
	}
	int ListBlockElement::GetChildWidth() { return this->ElementStyle.Width; }
	int ListBlockElement::GetChildHeight() { return 0; }
}}