#include "pch.h"
#include "GuiState.h"

GuiState::GuiState()
{
	memset(hostname, 0, sizeof(hostname));
	memset(namefilter, 0, sizeof(namefilter));
	memset(mapfilter, 0, sizeof(mapfilter));
	memset(password, 0, sizeof(password));
}

void GuiState::SetHostingState(int new_id, std::string new_token, bool new_hosting)
{
	std::lock_guard<std::mutex> guard(gui_lock);
	id = new_id;
	token = new_token;
	hosting = new_hosting;
}