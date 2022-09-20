#include "pch.h"
#include "RLLobby.h"

#include <utility>

#include "API/RLLobbyApi.h"
#include "API/Contracts/Requests/CreateLobbyRequest.h"
#include "upnp/miniupnpwrapper.h"


BAKKESMOD_PLUGIN(RLLobby, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLLobby::onLoad()
{
	_globalCvarManager = cvarManager;
	m_upnp = std::make_shared<Miniupnpwrapper>();
	m_lobby_api = std::make_shared<rllobby_api::RLLobbyApi>("https://rllobby.martinn.no/rllobby");

	cvarManager->registerNotifier("create_lobby", [this](...)
	{
		const rllobby_api::requests::CreateLobbyRequest req{"TestLobby", "TestMap", "TestDescription", 1, {"ME"}, false, "1.2.3.4", 7777};
		m_lobby_api->CreateLobby(req,
		                         [](const auto& res)
		                         {
			                         json j = res;
			                         LOG("{}", j.dump(4));
		                         },
		                         [](const std::string& basic_string)
		                         {
			                         LOG(basic_string);
		                         });
	}, "", 0);

	cvarManager->registerNotifier("get_lobbies", [this](...)
	{
		const rllobby_api::requests::CreateLobbyRequest req{"TestLobby", "TestMap", "TestDescription", 1, {"ME"}, false, "1.2.3.4", 7777};
		m_lobby_api->GetAllLobbyies(
			[](const auto& res)
			{
				json j = res;
				LOG("{}", j.dump(4));
			},
			[](const std::string& basic_string)
			{
				LOG(basic_string);
			});
	}, "", 0);
}

void RLLobby::onUnload()
{
}


void RLLobby::SetStatus(const std::string& message)
{
	m_last_status_message = message;
}

int RLLobby::GetPlayerCount() const
{
	if (gameWrapper->IsInGame())
	{
		auto server = gameWrapper->GetGameEventAsServer();
		if (!server.IsNull())
		{
			return server.GetPRIs().Count();
		}
	}
	return 0;
}

std::string RLLobby::GetLobbyParams() const
{
	auto server = gameWrapper->GetGameEventAsServer();
	if (!server.IsNull())
	{
		auto player = server.GetLocalPrimaryPlayer();
		if (!player.IsNull())
		{
			return  player.GetLoginURL().ToString();
		}
	}
	return "";
}

std::string RLLobby::GetMap() const
{
	return gameWrapper->GetCurrentMap();
}


void RLLobby::PortForward(int duration_seconds) const
{
	std::thread t([this, duration_seconds]()
	{
		m_upnp->Portforward(std::to_string(duration_seconds));
	});
	t.detach();
}

void RLLobby::PortForwardRemove() const
{
	std::thread t([this]()
	{
		m_upnp->RemovPortForward();
	});
	t.detach();
}


void RLLobby::ApiGetMatches(bool set_status)
{
	auto on_success = [this, set_status](const rllobby_api::responses::GetAllLobbiesResponse& res)
	{
		m_matches.ClearMatches();
		std::vector<MatchListing> matches;
		for (const auto& match : res.lobbies)
		{
			matches.push_back(FromLobbyToMatchListing(match));
		}
		m_matches.AddMatches(matches);
	};

	auto on_error = [this](const std::string& errro)
	{
		LOG("{}", errro);
	};

	m_lobby_api->GetAllLobbyies(on_success, on_error);
}


void RLLobby::ApiAddMatch(std::string name, std::string host, int port, bool has_password)
{
	auto server = gameWrapper->GetGameEventAsServer();
	if (!server || !server.IsPlayingLan())
	{
		SetStatus("Not hosting a LAN match");
		m_gui_data.hosting = false;
		return;
	}


	auto on_success = [this](const rllobby_api::responses::CreateLobbyResponse& res)
	{
		m_gui_data.SetHostingState(res.id, res.token, true);
		KeepMatchAliveLoop();
		SetStatus("Lobby hosted");
	};

	auto on_error = [this](const std::string& errro)
	{
		m_gui_data.hosting = false;
		LOG("KeepAlive fail: {}", errro);
	};

	auto req = rllobby_api::requests::CreateLobbyRequest{
		.name = std::move(name),
		.map = GetMap(),
		.description = "",
		.playerCount = GetPlayerCount(),
		.players = {},
		.hasPassword = has_password,
		.ipAddress = std::move(host),
		.port = port
	};

	m_lobby_api->CreateLobby(req, on_success, on_error);
}

void RLLobby::ApiMatchKeepAlive()
{
	const auto req = rllobby_api::requests::KeepAliveRequest{
		.id = m_gui_data.id,
		.token = m_gui_data.token
	};

	auto on_success = [this](const rllobby_api::responses::KeepAliveResponse res)
	{
		if (!res.result)
		{
			LOG("Keep alive fail");
		}
	};

	auto on_error = [this](const std::string& errro)
	{
		LOG("{}", errro);
	};

	m_lobby_api->KeepLobbyAlive(req, on_success, on_error);
}


void RLLobby::APiMatchDelete()
{
	auto req = rllobby_api::requests::DeleteLobbyRequest{
		.id = m_gui_data.id,
		.token = m_gui_data.token
	};

	auto on_success = [this]()
	{
		m_gui_data.SetHostingState("", "", false);
		SetStatus("Lobby deleted");
	};

	auto on_error = [this](const std::string& errro)
	{
		LOG("{}", errro);
	};

	m_lobby_api->DeleteLobby(req, on_success, on_error);
}

void RLLobby::KeepMatchAliveLoop()
{
	if (m_gui_data.hosting)
	{
		ApiMatchKeepAlive();
	}
	gameWrapper->SetTimeout([this](GameWrapper* gw) { KeepMatchAliveLoop(); }, m_keep_alive_interval);
}
