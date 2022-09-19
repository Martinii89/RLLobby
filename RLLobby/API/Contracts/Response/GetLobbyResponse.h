#pragma once

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

namespace rllobby_api::responses
{
	struct GetLobbyResponse
	{
		std::string id;
		std::string name;
		std::string map;
		std::string description;
		int playerCount;
		std::vector<std::string> players;
		bool hasPassword;
		std::string ipAddress;
		int port;
		int secondsSinceUpdate;
	};
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetLobbyResponse, id, name, map, description, playerCount, players, hasPassword, ipAddress, port, secondsSinceUpdate)
}
