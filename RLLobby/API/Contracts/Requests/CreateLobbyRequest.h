#pragma once

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

struct CreateLobbyRequest
{
	std::string name;
	std::string map;
	std::string description;
	int playerCount;
	std::vector<std::string> players;
	bool hasPassword = false;
	std::string ipAddress;
	int port;
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateLobbyRequest, name, map, description, playerCount, players, hasPassword, ipAddress, port)