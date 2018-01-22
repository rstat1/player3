/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#ifndef INFONUI
#define INFONUI

#include <map>

using namespace std;

namespace player3 { namespace overlay
{
	class InfoOverlayNUI
	{
		public:
			void InitOverlay();
			void UpdateOverlay();
			void AddIntValue(const char* label, int value);
			void AddDoubleValue(const char* label, double value);
			void AddStringValue(const char* label, const char* value);
			void UpdateIntValue(const char* label, int newValue);
			void UpdateDoubleValue(const char* label, double newValue);
			void UpdateStringValue(const char* label, const char* newValue);
		private:
			void CreateOverlayItem(const char* label, const char* value);
			void UpdateOverlayItem(const char* label, const char* newValue);

			std::map<std::string, std::string> overlayItems;

	};
}}

#endif