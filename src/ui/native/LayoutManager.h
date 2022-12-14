/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef LAYOUT
#define LAYOUT

#include <map>
#include <base/Utils.h>
#include <base/common.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <ui/native/elements/LabelElement.h>
#include <ui/native/elements/ImageElement.h>
#include <ui/native/elements/TextBlockElement.h>

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
			ElementMargin ParseMarginBlob(std::string marginBlob);
			ElementType ConvertItemTypeProperty(std::string prop);
			std::string VerifyVerticalAlignment(std::string prop);
			std::string VerifyHorizontalAlignment(std::string prop);
			PropertyBinding ParsePropertyBinding(std::string binding, const char* propertyName);

			UPTR(LabelElement) CreateLabelElement(ptree::value_type const& details);
			UPTR(ImageElement) CreateImageElement(ptree::value_type const& details);
			UPTR(TextBlockElement) CreateTextBlockElement(ptree::value_type const& details);
			ContainerElementBase* CreateRootElement(ptree::value_type const& details, const char* type);

			std::map<const char*, Layout, base::utils::CStringComparator> cachedLayouts;
			std::map<const char*, Layout, base::utils::CStringComparator> layoutInstances;
	};
}}

#endif
			//std::unique_ptr<ImageLabelElement> CreateImageLabelElement(ptree::value_type const& details);
			//std::unique_ptr<ImageElement> CreateImageElement(ptree::value_type const& details);