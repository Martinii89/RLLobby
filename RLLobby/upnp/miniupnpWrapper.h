#pragma once

#include <string>
#include <memory>
#include <winsock2.h>



enum class WrapperStatus
{
	Unknown,
	InitSuccess,
	InitFail
};

class Miniupnpwrapper
{
public:
	WrapperStatus status = WrapperStatus::Unknown;
	Miniupnpwrapper();

	bool Portforward(std::string duration);
	bool RemovPortForward();

private:
	WSADATA wsaData;
};
