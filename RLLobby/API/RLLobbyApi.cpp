#include "pch.h"
#include "RLLobbyApi.h"

#include "API/Contracts/Requests/CreateLobbyRequest.h"


RLLobbyApi::RLLobbyApi(std::string endpoint): m_endpoint(std::move(endpoint))
{
	
}

void RLLobbyApi::CreateLobby(const CreateLobbyRequest& create_lobby_request)
{
	CurlRequest req;
	req.verb = "POST";
	const json json = create_lobby_request;
	req.body = json.dump();
	req.url = std::format("{}/lobby", m_endpoint);

	HttpWrapper::SendCurlJsonRequest(req, [](int i, std::string basic_string)
	{
		LOG("Code: {}\n Data: {}", i, basic_string);
	});
}
