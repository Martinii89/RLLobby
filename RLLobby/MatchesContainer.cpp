#include "pch.h"
#include "MatchesContainer.h"

void MatchesContainer::AddMatch(const MatchListing m)
{
	std::lock_guard guard(m_lock);
	m_data.push_back(m);
}

void MatchesContainer::AddMatches(const std::vector<MatchListing>& matches)
{
	std::lock_guard guard(m_lock);
	m_data = matches;
}


std::vector<MatchListing> MatchesContainer::GetMatches()
{
	std::lock_guard guard(m_lock);
	auto copy = std::vector<MatchListing>();
	for (const auto& lobby : m_data)
	{
		copy.push_back(lobby);
	}
	return copy;
}

std::optional<MatchListing> MatchesContainer::GetMatch(const std::string& id)
{
	std::lock_guard guard(m_lock);
	for (auto& lobby : m_data)
	{
		if (lobby.id == id)
		{
			return lobby;
		}
	}
	return {};
}

void MatchesContainer::ClearMatches()
{
	std::lock_guard guard(m_lock);
	m_data.clear();
}
