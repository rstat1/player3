/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef LAYOUT
#define LAYOUT

#include <map>
#include <base/common.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <ui/native/elements/LabelElement.h>

using namespace boost::property_tree;

namespace player3 { namespace ui
{
	class LayoutManager
	{
		public:
			void LoadAndCacheLayout(const char* name);
			void RenderLayout(std::map<std::string, boost::any> bindings, const char* type, bool clearRoot);
		private:
			Style ParseStyleBlob(std::string styleBlob);
			AnchorPoint ConvertAnchorProperty(std::string prop);
			ElementType ConvertItemTypeProperty(std::string prop);
			PropertyBinding ParsePropertyBinding(std::string binding, const char* propertyName);

			ContainerElementBase* CreateRootElement(ptree::value_type const& details, const char* type);
			std::unique_ptr<LabelElement> CreateLabelElement(ptree::value_type const& details);

			std::map<const char*, Layout> cachedLayouts;
			std::map<const char*, Layout> layoutInstances;
	};
}}

#endif
			//std::unique_ptr<ImageLabelElement> CreateImageLabelElement(ptree::value_type const& details);
			//std::unique_ptr<ImageElement> CreateImageElement(ptree::value_type const& details);