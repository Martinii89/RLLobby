#pragma once


#include "nlohmann/json.hpp"

namespace signalr::json_utils
{
	json CreateJson(const value& v);
}
