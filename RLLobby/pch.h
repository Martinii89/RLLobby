#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include "bakkesmod/plugin/bakkesmodplugin.h"

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <source_location>

#include "imgui/imgui.h"
#include "IMGUI/imgui_stdlib.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

//#define NO_SIGNALRCLIENT_EXPORTS
//#include "signalrclient/hub_connection.h"
//#include "signalrclient/hub_connection_builder.h"
//#include "signalrclient/signalr_value.h"


extern std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

struct format_string
{
	std::string_view str;
	std::source_location loc;

	format_string(const char* str, const std::source_location& loc = std::source_location::current()) : str(str), loc(loc)
	{
	}

	[[nodiscard]] std::string GetLocation() const
	{
		return std::format("[{} {}:{}]", loc.function_name(), loc.file_name(), loc.line());
	}
};

template <typename S, typename... Args>
void LOG(const S& format_str, Args&&... args)
{
	_globalCvarManager->log(std::vformat(format_str, std::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
void DEBUGLOG(const format_string& format_str, Args&&... args)
{
	auto text = std::vformat(format_str, std::make_format_args(std::forward<Args>(args)...));
	auto location = format_str.GetLocation();
	_globalCvarManager->log(std::format("{} {}", text, location));
}

