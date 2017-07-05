/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef BLKELEM
#define BLKELEM

#include <ui/native/elements/ElementBase.h>

namespace player3 { namespace ui
{
	class BlockElement : public ContainerElementBase
	{
		public:
			ELEMENT_BASE(BlockElement, ElementType::Block)

			int GetChildWidth() override { return 0; }
			int GetChildHeight() override { return 0; }
			bool AllowChildrenToSetWidth() override { return true; }
			bool AllowChildrenToSetHeight() override { return true; }
	};
}}

#endif