/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <iostream>
#include <base/Utils.h>
#include <base/common.h>
#include <PlayerApp.h>
#include <client/linux/handler/exception_handler.h>

using namespace base::utils;
using namespace player3::player;
using namespace google_breakpad;

static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded)
{
  	printf("Dump path: %s\n", descriptor.path());
 	return succeeded;
}

int main(int argc, char* argv[])
{
	writeToLog("Init player3", true);
	MinidumpDescriptor descriptor(GetAppPath());
	ExceptionHandler eh(descriptor, NULL, dumpCallback, NULL, true, -1);

	PlayerApp::Get()->Start();
	return 0;
}