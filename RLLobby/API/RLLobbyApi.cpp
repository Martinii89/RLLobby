#include "pch.h"
#include "RLLobbyApi.h"

#include "HttpUtils.h"
#include "API/Contracts/Requests/CreateLobbyRequest.h"
#include "API/Contracts/Requests/KeepAliveRequest.h"
#include "Contracts/Response/CreateLobbyResponse.h"
#include "Contracts/Response/KeepAliveResponse.h"


rllobby_api::RLLobbyApi::RLLobbyApi(std::string endpoint): m_endpoint(std::move(endpoint))
{
}

void rllobby_api::RLLobbyApi::CreateLobby(
	const requests::CreateLobbyRequest& create_lobby_request,
	const std::function<void(const responses::CreateLobbyResponse&)>& success_callback,
	const std::function<void(const std::string&)>& error_callback)
{
	CurlRequest req;
	req.verb = "POST";
	const json json = create_lobby_request;
	req.body = json.dump();
	req.url = std::format("{}/lobby", m_endpoint);

	http_utils::HandleRequest<responses::CreateLobbyResponse>(201, req, success_callback, error_callback);
}

void rllobby_api::RLLobbyApi::GetAllLobbyies(
	const std::function<void(const responses::GetAllLobbiesResponse&)>& success_callback,
	const std::function<void(const std::string&)>& error_callback)
{
	CurlRequest req;
	req.verb = "GET";
	req.url = std::format("{}/lobby", m_endpoint);

	http_utils::HandleRequest<responses::GetAllLobbiesResponse>(200, req, success_callback, error_callback);
}

void rllobby_api::RLLobbyApi::KeepLobbyAlive(
	const requests::KeepAliveRequest& params,
	const std::function<void(responses::KeepAliveResponse)>& success_callback,
	const std::function<void(const std::string&)>& error_callback)
{
	CurlRequest req;
	req.verb = "POST";
	req.url = std::format("{}/lobby/{}/alive", m_endpoint, params.id);
	req.headers["Token"] = params.token;
	req.body = "{}"; //too lazy to handle no body special case

	http_utils::HandleRequest<responses::KeepAliveResponse>(200, req, success_callback, error_callback);
}

void rllobby_api::RLLobbyApi::DeleteLobby(const requests::DeleteLobbyRequest& params, const std::function<void()>& success_callback,
	const std::function<void(const std::string&)>& error_callback)
{
	CurlRequest req;
	req.verb = "DELETE";
	req.url = std::format("{}/lobby/{}", m_endpoint, params.id);
	req.headers["Token"] = params.token;
	req.body = "{}"; //too lazy to handle no body special case

	http_utils::HandleRequest(204, req, success_callback, error_callback);
}
