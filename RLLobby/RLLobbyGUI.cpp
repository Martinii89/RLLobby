#include "pch.h"
#include "RLLobby.h"
#include "Utils.h"
#include "imgui/imgui_internal.h"
#include "bakkesmod/wrappers/GuiManagerWrapper.h"


// Do ImGui rendering here
void RLLobby::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	// Make style consistent with BM
	auto& style = ImGui::GetStyle();
	if (gameWrapper != nullptr)
	{
		const auto bm_style_ptr = gameWrapper->GetGUIManager().GetImGuiStyle();
		if (bm_style_ptr != nullptr)
		{
			style = *(ImGuiStyle*)bm_style_ptr;
		}
		else
		{
			cvarManager->log("bm style ptr was null!!");
		}
	}
	else
	{
		cvarManager->log("gamewrapper was null!!");
	}
	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
	if (ImGui::BeginTabBar("#"), tab_bar_flags)
	{
		if (ImGui::BeginTabItem("Join"))
		{
			DrawJoinTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Host"))
		{
			DrawHostTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Port forward (experimental)"))
		{
			ImGui::BeginChild("#", ImVec2(-5, -25));
			ImGui::Text("Check the console (F6) to know if the upnp process failed or not");
			if (ImGui::Button("Try automatic upnp port forwarding"))
			{
				PortForward("18000");
			}
			if (ImGui::Button("Remove upnp forwarding"))
			{
				PortForwardRemove();
				//cvarManager->log("Removing upnp forwarding not implemented. The forwarding default to 1 hour of duration for now");
				// TODO: Implement
			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}


	//ImGui::Text(api->GetStatus().c_str());

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

void RLLobby::DrawHostTab()
{
	ImGui::BeginChild("#", ImVec2(-5, -25));

	ImGui::Columns(1, "game", false);
	ImGui::Text("Lobby name: ");
	ImGui::SameLine();
	ImGui::InputText("##lobbyNameInput", m_guistate.hostname, IM_ARRAYSIZE(m_guistate.hostname));

	if (!m_guistate.hosting)
	{
		if (ImGui::Button("Host"))
		{
			std::string hostname_str = std::string(m_guistate.hostname);
			gameWrapper->Execute([this, hostname_str](GameWrapper* gw) { ApiAddMatch({"", hostname_str}); });
			m_guistate.hosting = true;
		}
	}
	else
	{
		if (ImGui::Button("Unlist hosting"))
		{
			gameWrapper->Execute([this](GameWrapper* gw) { APiMatchDelete(); });
		}
	}

	//ImGui::NextColumn();
	ImGui::EndChild();
}

void RLLobby::DrawJoinTab()
{
	ImGui::BeginChild("#", ImVec2(-5, -25));

	// Filter matches
	ImGui::PushItemWidth(50);
	ImGui::InputText("Name filter", m_guistate.namefilter, IM_ARRAYSIZE(m_guistate.namefilter));
	ImGui::PushItemWidth(50);
	ImGui::SameLine();
	ImGui::InputText("map filter", m_guistate.mapfilter, IM_ARRAYSIZE(m_guistate.mapfilter));
	ImGui::SameLine();
	Utils::HelpMarker("Use * for wildcard [add a * at the end of your filter unless you want a exeact match]");

	// Refresh matches button
	ImGui::SameLine(ImGui::GetWindowWidth() - 60);
	if (ImGui::Button("Refresh"))
	{
		m_guistate.selectedIndex = -1;
		ApiGetMatches(true);
	}

	ImGui::Columns(4, 0, false);
	SetMatchColumnWidths();

	ImGui::Text("Name");
	ImGui::NextColumn();
	ImGui::Text("Map");
	ImGui::NextColumn();
	ImGui::Text("Players");
	ImGui::NextColumn();
	ImGui::Text("t");
	ImGui::SameLine();
	Utils::HelpMarker("Seconds since last update from host");
	ImGui::NextColumn();
	ImGui::Separator();
	ImGui::Columns(1, 0, false);
	ImGui::BeginChild("", ImVec2(0, -35));
	ImGui::Columns(4, 0, false);
	SetMatchColumnWidths();

	{
		// scope for mutex. Don't remove
		std::lock_guard<std::mutex> guard(m_matches.matches_lock);
		bool own_listing = false;
		for (size_t i = 0; i < m_matches._data.size(); i++)
		{
			auto& item = m_matches._data[i];
			if (item.id == m_guistate.id) own_listing = true;
			if (own_listing)
			{
				ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
				ImGui::PushStyleColor(ImGuiCol_Text, color * ImVec4(1, 0.7, 0.7, 0.66));
			}
			if (strlen(m_guistate.namefilter) != 0 && !Utils::StringMatch(m_guistate.namefilter, item.name.c_str())) continue;
			if (strlen(m_guistate.mapfilter) != 0 && !Utils::StringMatch(m_guistate.mapfilter, item.map_name.c_str())) continue;

			const std::string& item_name = item.name + (own_listing ? " (you)" : "") + "##" + std::to_string(i);
			if (ImGui::Selectable(item_name.c_str(), m_guistate.selectedIndex == i, ImGuiSelectableFlags_SpanAllColumns))
			{
				m_guistate.selectedIndex = i;
			};
			ImGui::NextColumn();
			ImGui::Text(item.map_name.c_str());
			ImGui::NextColumn();
			ImGui::Text(std::to_string(item.players).c_str());
			ImGui::NextColumn();
			ImGui::Text(std::to_string(item.time_updated).c_str());
			ImGui::NextColumn();
			if (own_listing)
			{
				ImGui::PopStyleColor();
			}
		}
	} // end of mutex scope
	ImGui::EndChild();
	ImGui::Dummy(ImVec2(0, 0));

	ImGui::Columns(1, 0, false);
	bool connectButtonEnabled = m_guistate.selectedIndex != -1;
	if (!connectButtonEnabled)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}
	ImGui::PushItemWidth(125);
	ImGui::InputText("Password", m_guistate.password, IM_ARRAYSIZE(m_guistate.password), ImGuiInputTextFlags_Password);
	ImGui::SameLine();
	if (ImGui::Button("Connect") && connectButtonEnabled /*guistate.selectedItem != nullptr*/)
	{
		//example host string Labs_Cosmic_V4_P?game=TAGame.GameInfo_Soccar_TA?Playtest?GameTags=BotsNone,PlayerCount8?NumPublicConnections=10?NumOpenPublicConnections=10?Lan?Listen
		// Labs_DoubleGoal_V2_P?game=TAGame.GameInfo_Soccar_TA?Playtest?GameTags=BotsNone,PlayerCount4?NumPublicConnections=10?NumOpenPublicConnections=10?Lan?Listen
		std::string joinStr = "open " + m_matches._data[m_guistate.selectedIndex].host + ":7777?Lan??" + "?Password=" + m_guistate.password;

		//log(joinStr);
		gameWrapper->Execute([joinStr](GameWrapper* gw) { gw->ExecuteUnrealCommand(joinStr); });
	}
	if (!connectButtonEnabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
	ImGui::EndChild();
}

void RLLobby::SetMatchColumnWidths()
{
	auto width = ImGui::GetWindowWidth();
	auto dynWidth = width - 100 - 50; //total width - two last colum widths - 50 as padding
	ImGui::SetColumnWidth(0, dynWidth * 0.6f);
	ImGui::SetColumnWidth(1, dynWidth * 0.4f);
	ImGui::SetColumnWidth(2, 60);
	ImGui::SetColumnWidth(3, 40);
}


std::string RLLobby::GetPluginName()
{
	return "RLLobby";
}

//GUI boilerplate


void RLLobby::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> RLLobby
void RLLobby::RenderSettings()
{
	ImGui::TextUnformatted("RLLobby plugin settings");
}


// Name of the menu that is used to toggle the window.
std::string RLLobby::GetMenuName()
{
	return "RLLobby";
}

// Title to give the menu
std::string RLLobby::GetMenuTitle()
{
	return menuTitle_;
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool RLLobby::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool RLLobby::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void RLLobby::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void RLLobby::OnClose()
{
	isWindowOpen_ = false;
}
