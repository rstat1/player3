/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <string>
#include <base/Utils.h>
#include <boost/foreach.hpp>
#include <ui/native/LayoutManager.h>

#include <ui/native/elements/containers/BlockElement.h>
#include <ui/native/elements/containers/ListBlockElement.h>

using namespace base::utils;

namespace player3 { namespace ui
{
	void LayoutManager::LoadAndCacheLayout(const char* name)
	{
		Layout l;
		ptree layout;
		std::string layoutsFile("/home/rstat1/Apps/streamlink/player/src/ChatUI.uism");
		read_xml(layoutsFile, layout);
		l.name = layout.get<std::string>("layout.<xmlattr>.name").c_str();
		for (ptree::value_type const& v: layout.get_child("layout"))
		{
			Log("UI", "v.first == %s", v.first.c_str());
			if (v.first == "block") { l.rootElement = this->CreateRootElement(v, "block"); }
			else if (v.first == "listblock") { l.rootElement = this->CreateRootElement(v, "listblock");  }
		}
		if (l.name != nullptr) { cachedLayouts[l.name] = l; }
	}
	void LayoutManager::CreateLayoutInstance(std::map<const char*, boost::any> bindings, const char* type, const char* tag)
	{
		if (this->cachedLayouts.find(type) != this->cachedLayouts.end())
		{
			Layout toInstance = this->cachedLayouts[type];
			toInstance.rootElement->BindProperties(bindings);
			toInstance.rootElement->Measure();
			toInstance.rootElement->ArrangeChildren();
			toInstance.rootElement->Render();
		}
	}
	void LayoutManager::UpdateLayoutInstance(std::map<const char*, boost::any> bindings, const char* tag)
	{

	}
	void LayoutManager::RenderLayout()
	{
		//TODO: Draw all the things.
	}
	Style LayoutManager::ParseStyleBlob(std::string styleBlob)
	{
		Style elementStyle;
		std::vector<std::string> elementDetails;
		std::vector<std::string> styleElements = split(styleBlob, ';');
		if (styleBlob != "")
		{
			for (const std::string element : styleElements)
			{
				elementDetails = split(element, ':');
				if (elementDetails[0] == "width") { elementStyle.Width = stoi(elementDetails[1]); }
				else if (elementDetails[0] == "height") { elementStyle.Height = stoi(elementDetails[1]); }
				else if (elementDetails[0] == "font-size") { elementStyle.FontSize = stoi(elementDetails[1]); }
				else if (elementDetails[0].find("padding") != std::string::npos)
				{
					//TODO: Implement padding. Maybe.
					//elementStyle.width = stoi(elementDetails[1]);
				}
				else if (elementDetails[0].find("margin") != std::string::npos)
				{
					//TODO: Implement margin. Maybe.
				}
			}
		}
		return elementStyle;
	}
	PropertyBinding LayoutManager::ParsePropertyBinding(std::string binding, const char* property)
	{
		PropertyBinding bind;
		std::vector<std::string> parts = split(binding, ':');
		bind.PropertyName = property;
		bind.BindingName = parts[1].substr(1, parts[1].size() - 2).c_str();

		Log("LM", "property = %s, bindingName = %s", bind.PropertyName, bind.BindingName);

		return bind;
	}
	std::unique_ptr<LabelElement> LayoutManager::CreateLabelElement(ptree::value_type const& details)
	{
		std::map<const char*, const char*> bindings;
		std::string textProperty = details.second.get<std::string>("<xmlattr>.text");
		if (textProperty.find("{Binding:") != std::string::npos)
		{
			PropertyBinding binding = ParsePropertyBinding(textProperty, "text");
			bindings["text"] = binding.BindingName;
		}
		Style style = ParseStyleBlob(details.second.get<std::string>("<xmlattr>.style").c_str());
		return std::make_unique<LabelElement>(style, bindings);
	}
	ContainerElementBase* LayoutManager::CreateRootElement(ptree::value_type const& details, const char* type)
	{
		Style s;
		AnchorPoint anchor;
		ContainerElementBase* root;
		std::map<const char*, const char*> bindings;

		if (strncmp(type, "block", 5) == 0)
		{
			s = ParseStyleBlob(details.second.get("block.<xmlattr>.style", ""));
			root = new BlockElement(s, std::map<const char*, const char*>());
			root->anchor = ConvertAnchorProperty(details.second.get("block.<xmlattr>.anchor", "bottom-left"));
		}
		else if (strncmp(type, "listblock", 9) == 0)
		{
			s = ParseStyleBlob(details.second.get("listblock.<xmlattr>.style", ""));
			std::string itemsProperty = details.second.get<std::string>("<xmlattr>.items");
			std::string anchorProperty = details.second.get<std::string>("<xmlattr>.anchor");
			std::string itemTypeProperty = details.second.get<std::string>("<xmlattr>.itemType");
			if (itemsProperty.find("{Binding:") != std::string::npos)
			{
				PropertyBinding binding = ParsePropertyBinding(itemsProperty, "items");
				bindings["items"] = binding.BindingName;
			}
			if (anchorProperty.find("{Binding:") != std::string::npos)
			{
				PropertyBinding binding = ParsePropertyBinding(anchorProperty, "anchor");
				bindings["anchor"] = binding.BindingName;
			}
			root = new ListBlockElement(s, bindings);
			root->SetItemType(ConvertItemTypeProperty(itemTypeProperty));
			root->anchor = ConvertAnchorProperty(details.second.get("listblock.<xmlattr>.anchor", "bottom-left"));
		}

		for (ptree::value_type const& w: details.second)
		{
			if (w.first == "label") { root->Children.push_back(CreateLabelElement(w)); }
			else if (w.first == "img") {  }
			else if (w.first == "imglabel") { }
		}
		return root;
	}
	AnchorPoint LayoutManager::ConvertAnchorProperty(std::string property)
	{
		if (property == "bottom-left") { return AnchorPoint::BottomLeft; }
		else if (property == "bottom-right") { return AnchorPoint::BottomRight; }
		else if (property == "top-left") { return AnchorPoint::TopLeft; }
		else if (property == "top-right") { return AnchorPoint::TopRight; }
		else { return AnchorPoint::None; }
	}
	ElementType LayoutManager::ConvertItemTypeProperty(std::string property)
	{
		if (property == "label") { return ElementType::Label; }
		else if (property == "imglabel") { return ElementType::ImageLabel; }
		else if (property == "img") { return ElementType::Image; }
		else { return ElementType::Invalid; }
	}
}}