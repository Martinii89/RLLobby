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


	if (ImGui::BeginTabBar("#", ImGuiTabBarFlags_Reorderable))
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
				PortForward(3600);
			}
			if (ImGui::Button("Remove upnp forwarding"))
			{
				PortForwardRemove();
			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}


	ImGui::TextUnformatted(m_last_status_message.c_str());

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
	ImGui::InputText("##lobbyNameInput", &m_gui_data.hostname);

	if (!m_gui_data.hosting)
	{
		if (ImGui::Button("Host"))
		{
			auto hostname_str = m_gui_data.hostname;
			gameWrapper->Execute([this, hostname_str](GameWrapper* gw)
			{
				ApiAddMatch(m_gui_data.hostname, "127.0.0.1", 7777, false);
			});
			m_gui_data.hosting = true;
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
	ImGui::InputText("Name filter", &m_gui_data.namefilter);
	ImGui::PushItemWidth(50);
	ImGui::SameLine();
	ImGui::InputText("map filter", &m_gui_data.mapfilter);
	ImGui::SameLine();
	Utils::HelpMarker("Use * for wildcard [add a * at the end of your filter unless you want a exeact match]");

	// Refresh matches button
	ImGui::SameLine(ImGui::GetWindowWidth() - 60);
	if (ImGui::Button("Refresh"))
	{
		m_gui_data.selected_id = "";
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


	bool own_listing = false;
	for (auto& [id, name, host, port, map_name, players, time_updated] : m_matches.GetMatches())
	{
		if (id == m_gui_data.id) own_listing = true;
		if (own_listing)
		{
			ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			ImGui::PushStyleColor(ImGuiCol_Text, color * ImVec4(1, 0.7, 0.7, 0.66));
		}
		if (!m_gui_data.namefilter.empty() && !Utils::StringMatch(m_gui_data.namefilter.c_str(), name.c_str())) continue;
		if (!m_gui_data.mapfilter.empty()  && !Utils::StringMatch(m_gui_data.mapfilter.c_str(), map_name.c_str())) continue;

		const std::string& item_name = name + (own_listing ? " (you)" : "") + "##" + id;
		if (ImGui::Selectable(item_name.c_str(), m_gui_data.selected_id == id, ImGuiSelectableFlags_SpanAllColumns))
		{
			m_gui_data.selected_id = id;
		};
		ImGui::NextColumn();
		ImGui::TextUnformatted(map_name.c_str());
		ImGui::NextColumn();
		ImGui::TextUnformatted(std::to_string(players).c_str());
		ImGui::NextColumn();
		ImGui::TextUnformatted(std::to_string(time_updated).c_str());
		ImGui::NextColumn();
		if (own_listing)
		{
			ImGui::PopStyleColor();
		}
	}

	ImGui::EndChild();
	ImGui::Dummy(ImVec2(0, 0));

	ImGui::Columns(1, 0, false);
	const auto selected_lobby = m_matches.GetMatch(m_gui_data.selected_id);
	const bool connect_button_enabled = selected_lobby.has_value();
	if (!connect_button_enabled)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}
	ImGui::PushItemWidth(125);
	ImGui::InputText("Password", &m_gui_data.password, ImGuiInputTextFlags_Password);
	ImGui::SameLine();
	if (ImGui::Button("Connect") && connect_button_enabled /*guistate.selectedItem != nullptr*/)
	{
		auto& lobby = selected_lobby.value();
		//example host string Labs_Cosmic_V4_P?game=TAGame.GameInfo_Soccar_TA?Playtest?GameTags=BotsNone,PlayerCount8?NumPublicConnections=10?NumOpenPublicConnections=10?Lan?Listen
		// Labs_DoubleGoal_V2_P?game=TAGame.GameInfo_Soccar_TA?Playtest?GameTags=BotsNone,PlayerCount4?NumPublicConnections=10?NumOpenPublicConnections=10?Lan?Listen
		std::string joinStr = "open " + lobby.host + ":7777?Lan??" + "?Password=" + m_gui_data.password;
		std::string joinStr2 = std::format("open {}:{}Lan?Password={}", lobby.host, lobby.port, m_gui_data.password);

		//log(joinStr);
		gameWrapper->Execute([joinStr](GameWrapper* gw) { gw->ExecuteUnrealCommand(joinStr); });
	}
	if (!connect_button_enabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
	ImGui::EndChild();
}

void RLLobby::SetMatchColumnWidths()
{
	const auto width = ImGui::GetWindowWidth();
	const auto dynWidth = width - 100 - 50; //total width - two last colum widths - 50 as padding
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
