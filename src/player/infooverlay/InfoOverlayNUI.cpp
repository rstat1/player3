/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <string>
#include <ui/native/NativeUIHost.h>
#include <player/infooverlay/InfoOverlayNUI.h>
#include <ui/native/elements/containers/ListBlockElement.h>

#include <base/metrics/InfluxDBClient.h>

using namespace player3::ui;
using namespace base::metrics;

namespace player3 { namespace overlay
{
	void InfoOverlayNUI::AddDoubleValue(const char* label, double value)
	{
		this->CreateOverlayItem(label, std::to_string(value).c_str());
	}
	void InfoOverlayNUI::AddIntValue(const char* label, int value)
	{
		this->CreateOverlayItem(label, to_string(value).c_str());
	}
	void InfoOverlayNUI::AddStringValue(const char* label, const char* value)
	{
		this->CreateOverlayItem(label, value);
	}
	void InfoOverlayNUI::UpdateDoubleValue(const char* label, double newValue)
	{
		//InfluxDBClient::Get()->UpdateDoubleMetric(std::string(label), newValue);

		this->UpdateOverlayItem(label, to_string(newValue).c_str());
	}
	void InfoOverlayNUI::UpdateIntValue(const char* label, int newValue)
	{
		//InfluxDBClient::Get()->UpdateIntMetric(std::string(label), newValue);
		this->UpdateOverlayItem(label, to_string(newValue).c_str());
	}
	void InfoOverlayNUI::UpdateStringValue(const char* label, const char* newValue)
	{
		this->UpdateOverlayItem(label, newValue);
	}
	void InfoOverlayNUI::CreateOverlayItem(const char* label, const char* value)
	{
		string item("");
		item.append(label);
		item.append(": ");
		item.append(value);
		overlayItems.emplace(std::make_pair(label, item));

	}
	void InfoOverlayNUI::UpdateOverlayItem(const char* label, const char* newValue)
	{
		const char* overlayItem;
		if (overlayItems.find(label) != overlayItems.end())
		{
			string item("");
			item.append(label);
			item.append(": ");
			item.append(newValue);
			overlayItems[label] = item;
		}
	}
	void InfoOverlayNUI::UpdateOverlay()
	{
		std::vector<ListBlockItem> items;
		std::map<std::string, boost::any> bindings;
		for (const auto& pair : overlayItems)
		{
			items.emplace_back(pair.second);
		}
		bindings["Items"] = items;
		bindings["AnchorPoint"] = AnchorPoint::BottomLeft;
		NativeUIHost::Get()->RenderScreen("DebugOverlay", bindings, true);
	}
}}