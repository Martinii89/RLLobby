#include "pch.h"
#include "RLLobby.h"

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


int RLLobby::GetPlayerCount()
{
	if (gameWrapper->IsInGame())
	{
		auto server = gameWrapper->GetGameEventAsServer();
		if (!server.IsNull())
		{
			return server.GetPRIs().Count();
		}
	}
	/*0 actually gives a validation error on the backend.
	But this is fine, this will prevent anyone from holding a match "live" while sitting in the menu */
	return 0;
}

std::string RLLobby::GetLobbyParams()
{
	auto server = gameWrapper->GetGameEventAsServer();
	std::string lobby_params = "";
	if (!server.IsNull())
	{
		//cvarManager->log("got server");
		auto player = server.GetLocalPrimaryPlayer();
		if (!player.IsNull())
		{
			//cvarManager->log("got local player");
			//log(player.GetLoginURL().ToString());
			lobby_params = player.GetLoginURL().ToString();
		}
	}
	return lobby_params;
}

std::string RLLobby::GetMap()
{
	return gameWrapper->GetCurrentMap();
}


void RLLobby::PortForward(std::string duration)
{
	std::thread t([this, duration]()
	{
		m_upnp->Portforward(duration);
	});
	t.detach();
}

void RLLobby::PortForwardRemove()
{
	std::thread t([this]()
	{
		m_upnp->RemovPortForward();
	});
	t.detach();
}

//void RLLobby::ApiGetMatch(std::vector<std::string> params)
//{
//	if (params.size() < 2)
//	{
//		cvarManager->log("usage: api_match <id>");
//		return;
//	}
//	std::string id = params[1];
//	api->GetMatch([this](MatchListing m) {
//		stringstream ss;
//		ss << "Got match: " << m.name << " | " << m.map_name << " | " /*<< m.host << ":" << m.port << " | "*/ << "The listing was updated by the host for " << m.time_updated << "s ago" << std::endl;
//		cvarManager->log(ss.str());
//		}, id);
//}

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
	//api->GetMatches([this](std::vector<MatchListing> matches) {
	//	stringstream ss;
	//	ss << std::endl;
	//	for (const auto& m : matches)
	//	{
	//		// Got match: name | map | host | port | time
	//		ss << "Got match: " << m.name << " | " << m.map_name << " | " /*<< m.host << ":" << m.port << " | "*/ << "The listing was updated by the host for " << m.time_updated << "s ago" << std::endl;
	//	}
	//	_matches.ClearMatches();
	//	_matches.AddMatches(matches);
	//	log(ss.str());
	//	}, set_status);
}

bool StringContains(std::string s, std::string pattern)
{
	return s.find(pattern) != std::string::npos;
}

void RLLobby::ApiAddMatch(std::vector<std::string> params)
{
	//if (params.size() < 2)
	//{
	//	cvarManager->log("usage: api_post_match <name>");
	//	return;
	//}
	//auto current_map = GetMap(); // gameWrapper->GetCurrentMap();

	//// TODO: Add this to the PostMatch struct. Need to decide if it needs any formatting..
	//auto lobby_params = GetLobbyParams();
	//if (!StringContains(lobby_params, "?Lan?Listen"))
	//{
	//	api->SetStatus("You're not hosting a Lan lobby!");
	//	log("You're not hosting a Lan lobby!", true);
	//	guistate.hosting = false;
	//	return;
	//}

	//auto match_details = PostMatch{ params[1], current_map, 7777 };
	//log("posting match: " + params[1] + " | Map:" + current_map, true);
	//api->AddMatch([this](MatchAuth auth) {
	//	if (auth.id < 0)
	//	{
	//		//Posting failed for some reason.
	//		guistate.hosting = false;
	//		return;
	//	}
	//	stringstream ss;
	//	ss << "Posted match with id: " << auth.id << " and token: " << auth.token << std::endl;
	//	log(ss.str());
	//	guistate.hosting = true;
	//	guistate.token = auth.token;
	//	guistate.id = auth.id;
	//	ApiGetMatches();
	//	}, match_details);
}

void RLLobby::ApiMatchKeepAlive()
{
	//int players = GetPlayerCount();
	//auto current_map = GetMap(); // gameWrapper->GetCurrentMap();
	//auto lobby_params = GetLobbyParams();

	//log("keeping match alive: " + std::to_string(guistate.id) + ":" + guistate.token);
	//auto auth = MatchAuth{ guistate.id, guistate.token };
	//auto args = KeepAliveParams{ auth, players, current_map, lobby_params };
	//api->KeepMatchAlive([this](std::string msg) {
	//	log(msg);
	//	if (msg != "Updated time")
	//	{
	//		log("Removing hosting state");
	//		guistate.hosting = false;
	//		guistate.token = "";
	//		guistate.id = -1;
	//	}
	//	}, args);
}

// TODO
void RLLobby::APiMatchDelete()
{
	//log("Deleting match: " + std::to_string(guistate.id) + ":" + guistate.token, true);
	////auto auth = MatchAuth{ params[1], params[2] };
	//auto auth = MatchAuth{ guistate.id, guistate.token };
	//api->DeleteMatch([this](std::string msg) {
	//	log(msg);
	//	//if (msg == "Deleted match")
	//	//{
	//	log("Removing hosting state");
	//	guistate.hosting = false;
	//	guistate.token = "";
	//	guistate.id = -1;
	//	ApiGetMatches();
	//	//}
	//	}, auth);
}

void RLLobby::KeepMatchAliveLoop()
{
	//if (guistate.hosting)
	//{
	//	ApiMatchKeepAlive();
	//}
	//if (isWindowOpen)
	//{
	//	ApiGetMatches();
	//}
	//gameWrapper->SetTimeout([this](GameWrapper* gw) {KeepMatchAliveLoop(); }, keepAliveInterval);
}
