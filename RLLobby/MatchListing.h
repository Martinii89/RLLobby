#pragma once

#include <string>

#include "API/Contracts/Response/GetAllLobbiesResponse.h"

struct MatchListing
{
	std::string id;
	std::string name;
	std::string host;
	int port;
	std::string map_name;
	int players;
	int time_updated; // Time of last KeepAlive from the host
};



MatchListing FromLobbyToMatchListing(const rllobby_api::responses::GetLobbyResponse& lobby);
