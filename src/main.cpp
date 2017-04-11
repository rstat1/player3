/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <iostream>
#include <base/common.h>
#include <player/PlayerApp.h>

using namespace streamlink::player;

int main(int argc, char* argv[])
{
	writeToLog("Init streamlink", true);

	if (argc == 1)
	{
		std::cout << "Player3 UI/Manager server not implemented." << std::endl;
		return 0;
	}
	else if (argc > 1)
	{
		if (argv[1] == "--player")
		{
			if (argc < 3)
			{
				Log("main", "missing url to play.");
				return 0;
			}
			else { PlayerApp* player = new PlayerApp((const char*)argv[2]); }
		}
	}
	return 0;
}