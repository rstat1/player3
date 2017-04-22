/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <base/platform/linux/memtrack.h>
namespace base { namespace platform
{
	double MemTrack::GetPeakMemoryUse()
	{
		struct rusage use;
		getrusage(RUSAGE_SELF, &use);
		return use.ru_maxrss / 1024;
	}
	double MemTrack::GetCurrentMemoryUse()
	{
		int tSize = 0, resident = 0, share = 0;
		ifstream buffer("/proc/self/statm");
		buffer >> tSize >> resident >> share;
		buffer.close();

		long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    	double rss = resident * page_size_kb;
		double shared_mem = share * page_size_kb;
		return (rss - shared_mem) / 1024;
	}
}}
// /**
//  * Returns the peak (maximum so far) resident set size (physical
//  * memory use) measured in bytes, or zero if the value cannot be
//  * determined on this OS.
//  */
// size_t getPeakRSS( )
// {
// 	struct rusage rusage;
// 	getrusage( RUSAGE_SELF, &rusage );
// 	return (size_t)(rusage.ru_maxrss * 1024L);
// }
// size_t getCurrentRSS( )
// {
// 	/* Linux ---------------------------------------------------- */
// 	long rss = 0L;
// 	FILE* fp = NULL;
// 	if ( (fp = fopen( "/proc/self/statm", "r" )) == NULL )
// 		return (size_t)0L;		/* Can't open? */
// 	if ( fscanf( fp, "%*s%ld", &rss ) != 1 )
// 	{
// 		fclose( fp );
// 		return (size_t)0L;		/* Can't read? */
// 	}
// 	fclose( fp );
// 	return (size_t)rss * (size_t)sysconf( _SC_PAGESIZE);
// }