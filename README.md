# Player3

This is a custom made video player built specifically for standalone Steam Link box produced by Valve for a few years (its no longer in production).

As such it needs:
* Steam Link SDK
* FFMPEG
* The code in this repository
* Also Python2 because it is old and hasn't been updated in a while.
* A Linux machine (physical or virtual)

Before building please define an environment variable "STEAMLINK_SDK_PATH" that points to the location of an instance of the Steam Link SDK (which can be found at http://github.com/ValveSoftware/steamlink-sdk. 

Beyond that, you just run .\build.py from the root of your checkout (after building FFMPEG)

Disclaimer: This won't really work as is because it depends on a private webservice that isn't open source yet. But it maybe useful in someway as an example of how to do streaming video playback with FFMPEG.
