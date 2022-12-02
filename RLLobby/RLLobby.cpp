#include "pch.h"
#include "RLLobby.h"

#include <utility>

#include "ClientHub.h"
#include "API/RLLobbyApi.h"
#include "API/Contracts/Requests/CreateLobbyRequest.h"
#include "upnp/miniupnpwrapper.h"
#include "register_command.h"

BAKKESMOD_PLUGIN(RLLobby, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

struct TestCommand
{
	int a;
	int b;

	static std::optional<TestCommand> Parse(const std::vector<std::string>& args)
	{
		if (args.size() < 3)
		{
			return {};
		}

		return TestCommand{
			.a = get_safe_int(args[1]),
			.b = get_safe_int(args[2])
		};


	}
};

void RLLobby::onLoad()
{
	_globalCvarManager = cvarManager;

	m_upnp = std::make_shared<Miniupnpwrapper>();

	api_endpoint = std::make_shared<std::string>("");
	auto api_cvar = cvarManager->registerCvar("rllobby_endpoint", "https://rllobby.martinn.no/rllobby");
	api_cvar.addOnValueChanged([this](auto val, auto cvar)
	{
		m_lobby_api = std::make_shared<rllobby_api::RLLobbyApi>(cvar.getStringValue());
	});
	api_cvar.bindTo(api_endpoint);
	LOG("endpoint: {}", *api_endpoint);
	m_lobby_api = std::make_shared<rllobby_api::RLLobbyApi>(*api_endpoint);

	//m_hub = std::make_shared<ClientHub>("https://localhost:44397/signalr/lobby");
	//m_hub->StartAsync();

	//m_hub->ListenEvent<rllobby_api::responses::GetLobbyResponse>("LobbyCreated", [this](auto& res)
	//{
	//	auto lobby = FromLobbyToMatchListing(res);
	//	m_matches.AddMatch(lobby);
	//});

	cvarManager->registerNotifier("rllobby_host", [this](std::vector<std::string> args)
	{
		if (args.size() < 4)
		{
			LOG("Usage: {} name host ip", args[0]);
		}
		ApiAddMatch(args[1], args[2], get_safe_int(args[3]), false);
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
			return player.GetLoginURL().ToString();
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
