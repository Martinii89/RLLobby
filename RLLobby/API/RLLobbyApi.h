#pragma once
#include "Contracts/contract_includes.h"

namespace rllobby_api
{
	class RLLobbyApi
	{
	public:
		explicit RLLobbyApi(std::string endpoint);

		void CreateLobby(
			const requests::CreateLobbyRequest& create_lobby_request,
			const std::function<void(const responses::CreateLobbyResponse&)>& success_callback,
			const std::function<void(const std::string&)>& error_callback);

		void GetAllLobbyies(
			const std::function<void(const responses::GetAllLobbiesResponse&)>&
			success_callback, const std::function<void(const std::string&)>& error_callback);

		void KeepLobbyAlive(
			const requests::KeepAliveRequest&,
			const std::function<void(responses::KeepAliveResponse)>& success_callback,
			const std::function<void(const std::string&)>& error_callback);

		void DeleteLobby(
			const requests::DeleteLobbyRequest& delete_lobby_request,
			const std::function<void()>& success_callback,
			const std::function<void(const std::string&)>& error_callback);

	private:
		std::string m_endpoint;
	};
}
