#pragma once
#include <string>
#include <vector>

#include "GetLobbyResponse.h"
#include "nlohmann/json.hpp"


namespace rllobby_api::responses
{
	struct GetAllLobbiesResponse
	{
		std::vector<GetLobbyResponse> lobbies;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetAllLobbiesResponse, lobbies)
}
