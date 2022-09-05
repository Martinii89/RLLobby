#include "pch.h"
#include "RLLobby.h"


BAKKESMOD_PLUGIN(RLLobby, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLLobby::onLoad()
{
	_globalCvarManager = cvarManager;
	LOG("Test {}", "asd");
}

void RLLobby::onUnload()
{
}