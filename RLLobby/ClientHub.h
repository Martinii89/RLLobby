#pragma once
#include <future>
#include <signalrclient/hub_connection.h>
#include "jsonUtils.h"

class ClientHub
{
public:
	signalr::hub_connection CreateConnection(const std::string& hub_endpoint) const;
	explicit ClientHub(std::string hub_endpoint);
	void StartAsync();
	void StopAsync();

	template<typename T>
	void ListenEvent(const std::string& event_name, std::function<void(T&)> callback);

	ClientHub(const ClientHub& other) = delete;
	ClientHub(ClientHub&& other) noexcept = delete;
	ClientHub& operator=(const ClientHub& other) = delete;
	ClientHub& operator=(ClientHub&& other) noexcept = delete;
	~ClientHub();
private:
	std::string m_hub_endpoint;
	signalr::hub_connection m_connection;
	std::thread m_client_thread;
	std::promise<void> m_stop_task;
	std::atomic<bool> m_running;

	void StartImpl();
};

template <typename T>
void ClientHub::ListenEvent(const std::string& event_name, std::function<void(T&)> callback)
{
	m_connection.on(event_name, [callback](const std::vector<signalr::value>& m)
	{
		try
		{
			const json json_data = signalr::json_utils::CreateJson(m[0]);
			T data = json_data;
			callback(data);
		}
		catch (...)
		{
		}
	});
}

