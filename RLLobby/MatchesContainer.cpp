#include "pch.h"
#include "MatchesContainer.h"

void MatchesContainer::AddMatch(MatchListing m)
{
	std::lock_guard<std::mutex> guard(matches_lock);
	_data.push_back(m);
}

void MatchesContainer::AddMatches(std::vector<MatchListing> matches)
{
	std::lock_guard<std::mutex> guard(matches_lock);
	_data = matches;
}


// This is probably stupid and inefficient...
std::vector<MatchListing> MatchesContainer::GetMatches()
{
	std::lock_guard<std::mutex> guard(matches_lock);
	auto copy = std::vector<MatchListing>();
	for (auto i : _data)
	{
		copy.push_back(i);
	}
	return copy;
}

void MatchesContainer::ClearMatches()
{
	std::lock_guard<std::mutex> guard(matches_lock);
	_data.clear();
}
