#include "pch.h"
#include "MatchListing.h"

MatchListing FromLobbyToMatchListing(const rllobby_api::responses::GetLobbyResponse& lobby)
{
	return MatchListing{lobby.id, lobby.name, lobby.ipAddress, lobby.port, lobby.map, lobby.playerCount, lobby.secondsSinceUpdate};
}
