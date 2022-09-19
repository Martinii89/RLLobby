#pragma once

#include "GuiState.h"
#include "MatchesContainer.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"

class Miniupnpwrapper;

namespace rllobby_api
{
	class RLLobbyApi;
}

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class RLLobby : public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{
	std::shared_ptr<Miniupnpwrapper> m_upnp;
	std::shared_ptr<rllobby_api::RLLobbyApi> m_lobby_api;
	MatchesContainer m_matches;
	GuiState m_guistate;
	int m_keep_alive_interval = 15;

	// Plugin functions
	int GetPlayerCount();
	std::string GetLobbyParams();
	std::string GetMap();

	//upnp
	void PortForward(std::string duration);
	void PortForwardRemove();

	// rllobby api
	void ApiGetMatches(bool set_status = false);
	void ApiAddMatch(std::vector<std::string> params);
	void ApiMatchKeepAlive();
	void APiMatchDelete();

	void KeepMatchAliveLoop();

	//Boilerplate
	void onLoad() override;
	void onUnload() override;

	// Inherited via PluginSettingsWindow

	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;


	// Inherited via PluginWindow



	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "RLLobby";

	void DrawHostTab();
	void DrawJoinTab();
	void SetMatchColumnWidths();

	void Render() override;
	std::string GetMenuName() override;
	std::string GetMenuTitle() override;
	bool ShouldBlockInput() override;
	bool IsActiveOverlay() override;
	void OnOpen() override;
	void OnClose() override;
};
