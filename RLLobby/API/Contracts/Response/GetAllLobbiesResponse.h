#pragma once
#include <string>
#include <vector>
#include "nlohmann/json.hpp"


namespace rllobby_api::responses
{
	struct GetAllLobbiesResponse
	{
		struct Lobby
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
			std::string created;
			std::string updated;
		};

		std::vector<Lobby> lobbies;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetAllLobbiesResponse::Lobby, id, name, map, description, playerCount, players, hasPassword, ipAddress, port, created, updated)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetAllLobbiesResponse, lobbies)
}
