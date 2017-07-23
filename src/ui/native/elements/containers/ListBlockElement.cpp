/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <map>
#include <typeinfo>
#include <base/common.h>
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
		screenSize = { 1280, 720 }; //PlatformManager::Get()->GetPlatformInterface()->GetScreenSize();
		this->SetNeedsRender(true);
	}
	void ListBlockElement::BindProperties(std::map<std::string, boost::any> bindingValues)
	{
		PROFILE_CPU(BindProps, RMTSF_Aggregate);
		if (bindingValues.find(anchorPropertyBinding.c_str()) != bindingValues.end())
		{
			if (bindingValues[anchorPropertyBinding.c_str()].type() == typeid(AnchorPoint))
			{
				this->SetAnchor(boost::any_cast<AnchorPoint>(bindingValues[anchorPropertyBinding.c_str()]));
			}
		}
		if (bindingValues.find(listItemsPropertyBinding.c_str()) != bindingValues.end())
		{
			boost::any listItems = bindingValues[listItemsPropertyBinding.c_str()];
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
		MICROPROFILE_SCOPEI("CHAT", "ListBoxArrange", MP_WHITE);

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
				elementHeight = elementBounds->Height;
				elementBounds->X = x;
				elementBounds->Y = previousHeight;
				elementBounds->Width = width;
				elementBounds->Height = elementHeight;
				e->SetBoundingBox(elementBounds);
				previousHeight += elementBounds->Height + 5;
			}
		}
		MicroProfileFlip(0);
	}
	void ListBlockElement::Render()
	{
		MICROPROFILE_SCOPEI("CHAT", "ListBoxRender", MP_WHITE);

		Box* bounds = this->GetBoundingBox();
		NanoVGRenderer::Get()->DrawRectangle(bounds->X, bounds->Y, bounds->Width, bounds->Height, this->ElementStyle.BGColor.c_str());
		if (this->Children.size() > 0)
		{
			for (std::unique_ptr<ElementBase> const& e : this->Children)
			{
				e->Render();
			}
		}
		MicroProfileFlip(0);
	}
	UPTR(LabelElement) ListBlockElement::CreateChildElement(std::string elementValue)
	{
		UPTRVAR(Label, LabelElement) = boost::make_unique<LabelElement>(defaultLabelStyle, std::vector<PropertyBinding>());
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
			MICROPROFILE_COUNTER_SET("LBERequiredHeight", totalRequiredHeight);
			for (std::unique_ptr<ElementBase> const& e : this->Children)
			{
				totalRequiredHeight -= this->Children.front()->GetBoundingBox()->Height;
				this->Children.pop_front();
				if (totalRequiredHeight < maxHeight ) { break; }
			}
		}
		MICROPROFILE_COUNTER_SET("ActiveLabelElements", this->Children.size());
	}
	void ListBlockElement::Clear()
	{
		this->Children.clear();
		totalRequiredHeight = 0;
	}
	int ListBlockElement::GetChildWidth() { return this->ElementStyle.Width; }
	int ListBlockElement::GetChildHeight() { return 0; }
}}