#pragma once
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

namespace rllobby_api::responses
{
	struct CreateLobbyResponse
	{
		std::string id;
		std::string name;
		std::string map;
		std::string description;
		int64_t playerCount;
		std::vector<std::string> players;
		bool hasPassword;
		std::string ipAddress;
		int64_t port;
		std::string token;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateLobbyResponse, id, name, map, description, playerCount, players, hasPassword, ipAddress, port, token)
}
