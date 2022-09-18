#pragma once
#include <string>

namespace rllobby_api::requests
{
	struct KeepAliveRequest
	{
		std::string id;
		std::string token;
	};
}
