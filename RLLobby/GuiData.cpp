#include "pch.h"
#include "GuiData.h"


void GuiData::SetHostingState(const std::string& new_id, const std::string& new_token, const bool new_hosting)
{
	std::lock_guard guard(m_lock);
	id = new_id;
	token = new_token;
	hosting = new_hosting;
}