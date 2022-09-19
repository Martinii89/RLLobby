#pragma once

#include <string>
#include <mutex>

class GuiState
{
public:

	std::mutex gui_lock;

	bool hosting = false;
	std::string token;
	std::string id;
	char hostname[256];

	bool filter_exact = false;
	char namefilter[256];
	char mapfilter[256];
	char password[256];
	int selectedIndex = -1;
	GuiState();

	void SetHostingState(int id, std::string token, bool hosting);
};