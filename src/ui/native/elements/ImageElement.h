/*
* Copyright (c) 2018 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef IMGELEM
#define IMGELEM

#include <ui/native/elements/LayoutManagerBase.h>

namespace player3 { namespace ui
  {
	class ImageElement : public ElementBase
	{
		public:
			ELEMENT_BASE(ImageElement, ElementType::Image);
			void Render() override;
			void Measure() override;

			PROPERTY(Source, std::string)
	};
  }}

#endif