#pragma once
#include "MatchListing.h"

class MatchesContainer
{
public:
	//Thread-safe access methods
	void AddMatch(MatchListing);
	void AddMatches(std::vector<MatchListing> matches);
	std::vector<MatchListing> GetMatches();
	void ClearMatches();

	std::mutex matches_lock;
	std::vector<MatchListing> _data;
private:
};
