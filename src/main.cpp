#include <App.h>
#include <base/common.h>

using namespace app;
int main(int argc, char* argv[])
{
	writeToLog("INit", true);
	App* app = new App();
	app->StartApp();
}