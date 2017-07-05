/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef LSTBLKELM
#define LSTBLKELM

#include <ui/native/elements/ElementBase.h>

namespace player3 { namespace ui
{
	class ListBlockElement : public ContainerElementBase
	{
		public:
			ELEMENT_BASE(ListBlockElement, ElementType::ListBlock);

			int GetChildWidth() override;
			int GetChildHeight() override;
			bool AllowChildrenToSetWidth() override { return false; }
			bool AllowChildrenToSetHeight() override { return false; }

	};
}}

#endif