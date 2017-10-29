/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <string>
#include <base/Utils.h>
#include <base/common.h>
#include <boost/foreach.hpp>
#include <ui/native/LayoutManager.h>
#include <boost/algorithm/string/trim.hpp>
#include <boost/smart_ptr/make_unique.hpp>

#include <ui/native/elements/containers/BlockElement.h>
#include <ui/native/elements/containers/ListBlockElement.h>

using namespace base::utils;

namespace player3 { namespace ui
{
	void LayoutManager::LoadAndCacheLayout(const char* name)
	{
		Style s;
		Layout l;
		ptree layout;
#if !defined(OS_STEAMLINK)
		std::string layoutsFile("/home/rstat1/Apps/streamlink/player/src/");
#else
		std::string layoutsFile("");
#endif
		layoutsFile.append(name);
		layoutsFile.append(".uism");

		read_xml(layoutsFile, layout);
		l.name = layout.get<std::string>("layout.<xmlattr>.name").c_str();
		for (ptree::value_type const& v: layout.get_child("layout"))
		{
			if (v.first == "block") { l.rootElement = this->CreateRootElement(v, "block"); }
			else if (v.first == "listblock") { l.rootElement = this->CreateRootElement(v, "listblock"); }
		}
		if (strncmp(l.name, "", 1) == 0) { Log("LM", "layout has no name"); }
		else
		{
			this->cachedLayouts[name] = l;
		}
	}
	void LayoutManager::RenderLayout(std::map<std::string, boost::any> bindings, const char* type, bool clearRoot = false)
	{
		if (this->cachedLayouts.find(type) != this->cachedLayouts.end())
		{
			PROFILE_CPU(RenderLayout, RMTSF_Aggregate)
			Layout toInstance = this->cachedLayouts[type];

			if (clearRoot) { toInstance.rootElement->Clear(); }

			toInstance.rootElement->BindProperties(bindings);
			toInstance.rootElement->Measure();
			toInstance.rootElement->ArrangeChildren();
			toInstance.rootElement->Render();

			toInstance.currentBindings = bindings;
			this->layoutInstances[type] = toInstance;
		}
		if (layoutInstances.size() > 0)
		{
			for (auto& l : layoutInstances)
			{
				if (strncmp(l.first, type, 1) != 0)
				{
					//l.second.rootElement->BindProperties(l.second.currentBindings);
					l.second.rootElement->Measure();
					l.second.rootElement->ArrangeChildren();
					l.second.rootElement->Render();
				}
			}
		}
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
				//TODO: Implement padding. Maybe.
				else if (elementDetails[0].find("padding") != std::string::npos) {}
				//TODO: Implement margin. Maybe.
				else if (elementDetails[0].find("margin") != std::string::npos) {}
				else if (elementDetails[0].find("bgcolor") != std::string::npos)
				{
					elementStyle.BGColor.assign(elementDetails[1]);
				}
				else if (elementDetails[0].find("fgcolor") != std::string::npos)
				{
					elementStyle.FGColor.assign(elementDetails[1]);
				}
				else if (elementDetails[0] == "halign")
				{
					elementStyle.HorizontalAlignment.assign(this->VerifyHorizontalAlignment(elementDetails[1]));
				}
				else if (elementDetails[0] == "valign")
				{
					elementStyle.VerticalAlignment.assign(this->VerifyVerticalAlignment(elementDetails[1]));
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
		bind.BindingName.assign(parts[1].substr(1, parts[1].size() - 2));
		return bind;
	}
	std::unique_ptr<LabelElement> LayoutManager::CreateLabelElement(ptree::value_type const& details)
	{
		std::unique_ptr<LabelElement> label;
		std::vector<PropertyBinding> bindings;

		std::string textProperty = details.second.get<std::string>("<xmlattr>.text");
		Style style = ParseStyleBlob(details.second.get<std::string>("<xmlattr>.style").c_str());
		if (textProperty.find("{Binding:") != std::string::npos)
		{
			PropertyBinding binding = ParsePropertyBinding(textProperty, "text");
			bindings.push_back(binding);
			label = boost::make_unique<LabelElement>(style, bindings);
		}
		else
		{
			label = boost::make_unique<LabelElement>(style, bindings);
			label->SetText(textProperty.c_str());
		}
		return label;
	}
	ContainerElementBase* LayoutManager::CreateRootElement(ptree::value_type const& details, const char* type)
	{
		Style s;
		AnchorPoint anchor;
		bool anchorIsBound = false;
		ContainerElementBase* root;
		std::vector<PropertyBinding> bindings;
		std::string anchorProperty = details.second.get<std::string>("<xmlattr>.anchor");

		if (strncmp(type, "block", 5) == 0)
		{
			s = ParseStyleBlob(details.second.get("<xmlattr>.style", ""));
			root = new BlockElement(s, std::vector<PropertyBinding>());
			
			if (anchorProperty != "") { root->SetAnchor(ConvertAnchorProperty(anchorProperty)); }
		}
		else if (strncmp(type, "listblock", 9) == 0)
		{
			s = ParseStyleBlob(details.second.get("<xmlattr>.style", ""));
			std::string itemsProperty = details.second.get<std::string>("<xmlattr>.items");
			std::string itemTypeProperty = details.second.get<std::string>("<xmlattr>.itemType");

			if (itemsProperty.find("{Binding:") != std::string::npos)
			{
				PropertyBinding binding = ParsePropertyBinding(itemsProperty, "items");
				bindings.push_back(binding);
			}
			if (anchorProperty.find("{Binding:") != std::string::npos)
			{
				PropertyBinding binding = ParsePropertyBinding(anchorProperty, "anchor");
				bindings.push_back(binding);
				anchorIsBound = true;
			}

			root = new ListBlockElement(s, bindings);
			if (anchorIsBound == false) { root->SetAnchor(ConvertAnchorProperty(details.second.get("<xmlattr>.anchor", "bottom-right"))); }
			root->SetItemType(ConvertItemTypeProperty(itemTypeProperty));
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
		Log("LM", "anchor = %s", property.c_str());

		boost::trim(property);

		if (property == "bottom-left") { return AnchorPoint::BottomLeft; }
		else if (property == "bottom-right") { return AnchorPoint::BottomRight; }
		else if (property == "top-left") { return AnchorPoint::TopLeft; }
		else if (property == "top-right") { return AnchorPoint::TopRight; }
		else { return AnchorPoint::Not; }
	}
	ElementType LayoutManager::ConvertItemTypeProperty(std::string property)
	{
		if (property == "label") { return ElementType::Label; }
		else if (property == "imglabel") { return ElementType::ImageLabel; }
		else if (property == "img") { return ElementType::Image; }
		else { return ElementType::Invalid; }
	}
	std::string LayoutManager::VerifyVerticalAlignment(std::string prop)
	{
		if (prop == "Fill" || prop == "Top" || prop == "Bottom" || prop == "Center") { return prop; }
		else { return ""; }
	}
	std::string LayoutManager::VerifyHorizontalAlignment(std::string prop)
	{
		if (prop == "Fill" || prop == "Left" || prop == "Right" || prop == "Center") { return prop; }
		else { return ""; }
	}
}}