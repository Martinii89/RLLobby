#pragma once

#include <string>
#include <mutex>

class GuiData
{
public:

	bool hosting = false;
	std::string token;
	std::string id;
	std::string hostname;

	bool filter_exact = false;
	std::string namefilter;
	std::string mapfilter;
	std::string password;
	std::string selected_id;

	void SetHostingState(const std::string& new_id, const std::string& new_token, bool new_hosting);
private:
	std::mutex m_lock;
};
