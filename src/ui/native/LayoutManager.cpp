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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <ui/native/elements/LabelElement.h>

using namespace base::utils;
namespace proptree = boost::property_tree;

namespace player3 { namespace ui
{
	void LayoutManager::LoadUISM(const char* name)
	{
		proptree::ptree layout;
		std::string layoutsFile("/home/rstat1/Apps/streamlink/player/src/StreamIsLive.uism");
		proptree::read_xml(layoutsFile, layout);
		Layout l;
		l.name = layout.get<std::string>("layout.<xmlattr>.name").c_str();
		l.style = ParseStyleBlob(layout.get<std::string>("layout.<xmlattr>.style"));

		for (proptree::ptree::value_type const& v: layout.get_child("layout"))
		{
			Log("UI", "v.first == %s", v.first.c_str());
			if (v.first == "label")
			{
				std::vector<PropertyBinding> bindings;
				std::string textProperty = v.second.get<std::string>("<xmlattr>.text");
				if (textProperty.find("{Binding") != std::string::npos)
				{
					PropertyBinding binding = ParsePropertyBinding(textProperty);
					bindings.push_back(binding);
				}
				Style style = ParseStyleBlob(v.second.get<std::string>("<xmlattr>.style").c_str());
				l.childElements.push_back(std::make_unique<LabelElement>(style, bindings));
			}
			if (v.first == "image") {}
		}
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
	PropertyBinding LayoutManager::ParsePropertyBinding(std::string binding)
	{

	}
}}