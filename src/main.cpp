#include <App.h>
#include <base/common.h>

using namespace app;
void InitProfiler()
{
#if defined(ENABLE_PROFILER)
	MicroProfileInit();
	MicroProfileSetEnableAllGroups(true);
	MicroProfileSetForceMetaCounters(true);
#endif
	MICROPROFILE_DEFINE(MAIN, "MAIN", "Main", MP_PURPLE);
	MICROPROFILE_DEFINE(VIDEO, "VIDEO", "Video", MP_BLUE);
	MICROPROFILE_DEFINE(CHAT, "CHAT", "Chat", MP_ALICEBLUE);
	MICROPROFILE_COUNTER_ADD("memory", 1000);
	MICROPROFILE_COUNTER_ADD("ActiveLabelElements", 1000);
	MICROPROFILE_COUNTER_ADD("AVClockDiff", 1000);
	MICROPROFILE_COUNTER_ADD("AVDelayDiff", 1000);
	MICROPROFILE_COUNTER_ADD("FrameTimer", 1000);
	MICROPROFILE_COUNTER_ADD("VideoActualDelay", 1000);
	MICROPROFILE_COUNTER_ADD("AudioClock", 1000);
	MICROPROFILE_COUNTER_ADD("AudioDelay", 1000);
	MICROPROFILE_COUNTER_ADD("LBERequiredHeight", 1000);

	MICROPROFILE_COUNTER_CONFIG("memory", MICROPROFILE_COUNTER_FORMAT_BYTES, 10000, 0);
}
void StopProfiler()
{
#if defined(ENABLE_PROFILER)
	MicroProfileShutdown();
#endif
}
int main(int argc, char* argv[])
{
	InitProfiler();

	writeToLog("INit", true);
	App* app = new App();
	app->StartApp();

	StopProfiler();
}