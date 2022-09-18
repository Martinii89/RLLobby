#include "pch.h"
#include "RLLobby.h"

#include "API/RLLobbyApi.h"
#include "API/Contracts/Requests/CreateLobbyRequest.h"


BAKKESMOD_PLUGIN(RLLobby, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLLobby::onLoad()
{
	_globalCvarManager = cvarManager;

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
