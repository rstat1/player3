#ifndef MPDEFS
#define MPDEFS

#if defined(ENABLE_PROFILER)
	#define MICROPROFILE_ENABLED 1
#else
	#define MICROPROFILE_ENABLED 0
#endif

#include <microprofile/microprofile.h>

#if defined(PROFILE_INIT)

#endif

#endif