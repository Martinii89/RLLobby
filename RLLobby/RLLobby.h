#pragma once

#include "GuiData.h"
#include "MatchesContainer.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"

class ClientHub;
class Miniupnpwrapper;

namespace rllobby_api
{
	class RLLobbyApi;
}

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class RLLobby : public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{
	std::shared_ptr<std::string> api_endpoint;
	std::shared_ptr<Miniupnpwrapper> m_upnp;
	std::shared_ptr<rllobby_api::RLLobbyApi> m_lobby_api;
	//std::shared_ptr<ClientHub> m_hub;
	MatchesContainer m_matches;
	GuiData m_gui_data;
	int m_keep_alive_interval = 15;
	std::string m_last_status_message;

	void SetStatus(const std::string& message);

	// Plugin functions
	int GetPlayerCount() const;
	std::string GetLobbyParams() const;
	std::string GetMap() const;

	//upnp
	void PortForward(int duration_seconds) const;
	void PortForwardRemove() const;

	// rllobby api
	void ApiGetMatches(bool set_status = false);
	void ApiAddMatch(std::string name, std::string host, int port, bool has_password);
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
	void JoinLobby(const MatchListing& lobby);
	void DrawJoinTab();
	static void SetMatchColumnWidths();

	void RenderRLLobby();
	void Render() override;
	std::string GetMenuName() override;
	std::string GetMenuTitle() override;
	bool ShouldBlockInput() override;
	bool IsActiveOverlay() override;
	void OnOpen() override;
	void OnClose() override;
};
