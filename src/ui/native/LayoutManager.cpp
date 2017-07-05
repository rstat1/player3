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
		std::string layoutsFile("/home/rstat1/Apps/streamlink/player/src/StreamIsLive.uism");

		read_xml(layoutsFile, layout);

		l.name = layout.get<std::string>("layout.<xmlattr>.name").c_str();

		for (ptree::value_type const& v: layout.get_child("layout"))
		{
			Log("UI", "v.first == %s", v.first.c_str());
			if (v.first == "block") { l.rootElement = this->CreateRootElement(v, "block"); }
			else if (v.first == "listblock") { l.rootElement = nullptr;  }
		}
		if (l.name != nullptr) { cachedLayouts[l.name] = l; }
	}
	void LayoutManager::RenderLayout()
	{
		//TODO: Do property binding.
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
				if (elementDetails[0] == "width") { elementStyle.width = stoi(elementDetails[1]); }
				else if (elementDetails[0] == "height") { elementStyle.height = stoi(elementDetails[1]); }
				else if (elementDetails[0] == "font-size") { elementStyle.fontSize = stoi(elementDetails[1]); }
				else if (elementDetails[0].find("padding") != std::string::npos)
				{
					//TODO: Implement padding.
					//elementStyle.width = stoi(elementDetails[1]);
				}
				else if (elementDetails[0].find("margin") != std::string::npos)
				{
					//TODO: Implement margin.
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
		bind.BindingName = parts[1].substr(0, parts[1].size() -1).c_str();
		return bind;
	}
	void LayoutManager::CreateLayoutInstance(std::map<const char*, void*> bindings, const char* type, const char* tag)
	{

	}
	void LayoutManager::UpdateLayoutInstance(std::map<const char*, void*> bindings, const char* tag)
	{

	}
	std::unique_ptr<LabelElement> LayoutManager::CreateLabelElement(ptree::value_type const& details)
	{
		std::vector<PropertyBinding> bindings;
		std::string textProperty = details.second.get<std::string>("<xmlattr>.text");
		if (textProperty.find("{Binding") != std::string::npos)
		{
			PropertyBinding binding = ParsePropertyBinding(textProperty, "text");
			bindings.push_back(binding);
		}
		Style style = ParseStyleBlob(details.second.get<std::string>("<xmlattr>.style").c_str());
		return std::make_unique<LabelElement>(style, bindings);
	}
	ContainerElementBase* LayoutManager::CreateRootElement(ptree::value_type const& details, const char* type)
	{
		Style s;
		ContainerElementBase* root;
		std::vector<PropertyBinding> bindings;

		if (type == "block")
		{
			s = ParseStyleBlob(details.second.get("block.<xmlattr>.style", ""));
			root = new BlockElement(s, std::vector<PropertyBinding>());
			root->anchor = ConvertAnchorProperty(details.second.get("block.<xmlattr>.anchor", "bottom-left"));
		}
		else if (type == "listblock")
		{
			s = ParseStyleBlob(details.second.get("listblock.<xmlattr>.style", ""));
			std::string itemsProperty = details.second.get<std::string>("<xmlattr>.items");
			if (itemsProperty.find("{Binding") != std::string::npos)
			{
				PropertyBinding binding = ParsePropertyBinding(itemsProperty, "items");
				bindings.push_back(binding);
			}
			root = new ListBlockElement(s, bindings);
			root->anchor = ConvertAnchorProperty(details.second.get("listblock.<xmlattr>.anchor", "bottom-left"));
		}

		for (ptree::value_type const& w: details.second)
		{
			if (w.first == "label") { root->Children.push_back(CreateLabelElement(w)); }
			if (w.first == "img") {  }
			if (w.first == "imglabel") { }
		}
		return root;
	}
	AnchorPoint LayoutManager::ConvertAnchorProperty(std::string property)
	{
		if (property == "bottom-left") { return AnchorPoint::BottomLeft; }
		if (property == "bottom-right") { return AnchorPoint::BottomRight; }
		if (property == "top-left") { return AnchorPoint::TopLeft; }
		if (property == "top-right") { return AnchorPoint::TopRight; }
	}
}}