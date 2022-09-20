#pragma once

namespace rllobby_api::requests
{
	struct DeleteLobbyRequest
	{
		std::string id;
		std::string token;
	};
}
