#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include "bakkesmod/plugin/bakkesmodplugin.h"

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include "imgui/imgui.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

extern std::shared_ptr<CVarManagerWrapper> _globalCvarManager;


template<typename S, typename... Args>
void LOG(const S& format_str, Args&&... args)
{
	_globalCvarManager->log(std::vformat(format_str, std::make_format_args(std::forward<Args>(args)...)));
}