#pragma once
#include "MatchListing.h"

class MatchesContainer
{
public:
	void AddMatch(MatchListing);
	void AddMatches(const std::vector<MatchListing>& matches);
	std::vector<MatchListing> GetMatches();
	std::optional<MatchListing> GetMatch(const std::string& id);
	void ClearMatches();

private:
	std::mutex m_lock;
	std::vector<MatchListing> m_data;
};
