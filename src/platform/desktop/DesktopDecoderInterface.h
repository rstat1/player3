/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef DESKDEC
#define DESKDEC

#include <platform/DecoderInterface.h>

namespace player3 { namespace platform
{
	class DesktopDecoderInterface : public DecoderInterface
	{
		public:
			void Reset() override {}
			void Shutdown() override {}
			void DecodeVideoFrame(uint8_t* data, int size) override {}
	};
}}

#endif