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
	std::string host_address;

	bool filter_exact = false;
	std::string namefilter;
	std::string mapfilter;
	std::string password;
	std::string selected_id;
	int port = 7777;

	void SetHostingState(const std::string& new_id, const std::string& new_token, bool new_hosting);
private:
	std::mutex m_lock;
};
