#include "pch.h"
#include "RLLobby.h"

#include "API/RLLobbyApi.h"
#include "API/Contracts/Requests/CreateLobbyRequest.h"


BAKKESMOD_PLUGIN(RLLobby, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLLobby::onLoad()
{
	_globalCvarManager = cvarManager;

	m_lobby_api = std::make_shared<RLLobbyApi>("localhost:5000");

	cvarManager->registerNotifier("create_lobby", [this](...)
	{
		CreateLobbyRequest req {"TestLobby", "TestMap", "TestDescription", 1, {"ME"}, false, "1.2.3.4", 7777};
		m_lobby_api->CreateLobby(req);
	}, "", 0);
}

void RLLobby::onUnload()
{
}