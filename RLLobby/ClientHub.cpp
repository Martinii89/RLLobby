#include "pch.h"
#include "ClientHub.h"

#define NO_SIGNALRCLIENT_EXPORTS

#include <future>
#include "signalrclient/hub_connection.h"
#include "signalrclient/hub_connection_builder.h"
#include "signalrclient/signalr_value.h"

#pragma comment(lib, "crypt32")
#pragma comment(lib, "winhttp")


class logger : public signalr::log_writer
{
	// Inherited via log_writer
	virtual void __cdecl write(const std::string& entry) override
	{
		LOG("{}", entry);
	}
};



signalr::hub_connection ClientHub::CreateConnection(const std::string& hub_endpoint) const
{
	return signalr::hub_connection_builder::create(hub_endpoint)
	       .with_logging(std::make_shared<logger>(), signalr::trace_level::verbose)
	       .build();
}

void ClientHub::StartAsync()
{

	StopAsync();

	std::thread t ([this]
	{
		StartImpl();
	});

	m_client_thread = std::move(t);
	
}

ClientHub::ClientHub(std::string hub_endpoint):
	m_hub_endpoint(std::move(hub_endpoint)), m_connection(CreateConnection(m_hub_endpoint))
{
}

void ClientHub::StopAsync()
{
	m_stop_task.set_value();
	if (m_client_thread.joinable())
	{
		m_client_thread.join();
	}
}

ClientHub::~ClientHub()
{
	StopAsync();
}

void ClientHub::StartImpl()
{
	std::promise<void> start_task;
	m_stop_task = {};

	m_connection.start([&start_task](const std::exception_ptr& exception)
	{
		LOG("Started");
		try
		{
			if (exception)
			{
				std::rethrow_exception(exception);
			}
		}
		catch (const std::exception& e)
		{
			LOG("{}", e.what());
		}
		start_task.set_value();
	});

	m_stop_task.get_future().get();
	std::promise<void> stopped_task;

	m_connection.stop([&stopped_task](const std::exception_ptr& exception)
	{
		LOG("Stopped");
		stopped_task.set_value();
	});

	stopped_task.get_future().get();
}
