/*
* Copyright (c) 2016 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef NANOVGDEFS
#define NANOVGDEFS

#include <base/common.h>

#if defined(OS_STEAMLINK)
	#include <GLES2/gl2.h>
	#define NANOVG_GLES2_IMPLEMENTATION
	#define NANOVG_INIT context = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#else
	#include <GL/glew.h>
	#define NANOVG_GL2_IMPLEMENTATION
	#define NANOVG_INIT context = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif
#include <nanovg/nanovg.h>
#include <nanovg/nanovg_gl.h>
#include <nanovg/nanovg_gl_utils.h>
//#include <ui/graphics/rendering/nanovg/NanoVGPerfGraph.h>

#endif