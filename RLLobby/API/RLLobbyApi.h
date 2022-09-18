#pragma once

struct CreateLobbyRequest;

class RLLobbyApi
{
public:
	explicit RLLobbyApi(std::string endpoint);

	void CreateLobby(const CreateLobbyRequest& create_lobby_request);

private:
	std::string m_endpoint;
};
