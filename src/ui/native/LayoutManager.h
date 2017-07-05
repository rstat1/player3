/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef LAYOUT
#define LAYOUT

#include <map>
#include <vector>
#include <base/common.h>
#include <boost/property_tree/ptree.hpp>
#include <ui/native/elements/ElementBase.h>
#include <ui/native/elements/LabelElement.h>
#include <boost/property_tree/xml_parser.hpp>

using namespace boost::property_tree;

namespace player3 { namespace ui
{
	class LayoutManager
	{
		public:
			void LoadAndCacheLayout(const char* name);
			void CreateLayoutInstance(std::map<const char*, void*> bindings, const char* type, const char* tag);
			void UpdateLayoutInstance(std::map<const char*, void*> bindings, const char* tag);
		private:
			void DoLayout();
			void RenderLayout();
			void CreateElement();
			Style ParseStyleBlob(std::string styleBlob);
			PropertyBinding ParsePropertyBinding(std::string binding, const char* propertyName);

			std::unique_ptr<LabelElement> CreateLabelElement(ptree::value_type const& details);
			//std::unique_ptr<ImageLabelElement> CreateImageLabelElement(ptree::value_type const& details);
			//std::unique_ptr<ImageElement> CreateImageElement(ptree::value_type const& details);
			ContainerElementBase* CreateRootElement(ptree::value_type const& details, const char* type);
			AnchorPoint ConvertAnchorProperty(std::string prop);

			std::map<const char*, Layout> cachedLayouts;
	};
}}

#endif