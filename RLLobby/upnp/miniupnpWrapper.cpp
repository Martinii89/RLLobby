#include "pch.h"
#include "miniupnpwrapper.h"

#define MINIUPNP_STATICLIB

#include "miniupnpc/miniupnpc.h"
#include "miniupnpc/upnpcommands.h"
#include "miniupnpc/upnperrors.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "IPHlpApi.Lib")

const char* protofix(const char* proto)
{
	static const char proto_tcp[4] = {'T', 'C', 'P', 0};
	static const char proto_udp[4] = {'U', 'D', 'P', 0};
	int i, b;
	for (i = 0, b = 1; i < 4; i++)
		b = b && ((proto[i] == proto_tcp[i])
			|| (proto[i] == (proto_tcp[i] | 32)));
	if (b)
		return proto_tcp;
	for (i = 0, b = 1; i < 4; i++)
		b = b && ((proto[i] == proto_udp[i])
			|| (proto[i] == (proto_udp[i] | 32)));
	if (b)
		return proto_udp;
	return 0;
}

static int SetRedirectAndTest(UPNPUrls* urls,
                              IGDdatas* data,
                              const char* iaddr,
                              const char* iport,
                              const char* eport,
                              const char* proto,
                              const char* leaseDuration,
                              const char* remoteHost,
                              const char* description,
                              int addAny)
{
	char externalIPAddress[40];
	char intClient[40];
	char intPort[6];
	char reservedPort[6];
	char duration[16];
	int r;

	if (!iaddr || !iport || !eport || !proto)
	{
		LOG("Wrong arguments");
		return -1;
	}
	proto = protofix(proto);
	if (!proto)
	{
		LOG("invalid protocol");
		return -1;
	}

	r = UPNP_GetExternalIPAddress(urls->controlURL,
	                              data->first.servicetype,
	                              externalIPAddress);
	if (r != UPNPCOMMAND_SUCCESS)
		LOG("GetExternalIPAddress failed.");
	//else
	//	LOG("ExternalIPAddress = " + string(externalIPAddress));

	if (addAny)
	{
		r = UPNP_AddAnyPortMapping(urls->controlURL, data->first.servicetype,
		                           eport, iport, iaddr, description,
		                           proto, remoteHost, leaseDuration, reservedPort);
		if (r == UPNPCOMMAND_SUCCESS)
			eport = reservedPort;
		else
		{
			LOG(
				"AddAnyPortMapping(" + std::string(eport) + ", " + std::string(iport) + ", " + std::string(iaddr) + ") failed with code " + std::to_string(r) +
				": " + std::string(strupnperror(r)));
		}
	}
	else
	{
		r = UPNP_AddPortMapping(urls->controlURL, data->first.servicetype,
		                        eport, iport, iaddr, description,
		                        proto, remoteHost, leaseDuration);
		if (r != UPNPCOMMAND_SUCCESS)
		{
			LOG(
				"AddPortMapping(" + std::string(eport) + ", " + std::string(iport) + ", " + std::string(iaddr) + ") failed with code " + std::to_string(r) +
				": " + std::string(strupnperror(r)));
			return -2;
		}
	}

	r = UPNP_GetSpecificPortMappingEntry(urls->controlURL,
	                                     data->first.servicetype,
	                                     eport, proto, remoteHost,
	                                     intClient, intPort, NULL/*desc*/,
	                                     NULL/*enabled*/, duration);
	if (r != UPNPCOMMAND_SUCCESS)
	{
		LOG("GetSpecificPortMappingEntry() failed with code " + std::to_string(r) + ": " + std::string(strupnperror(r)));
		return -2;
	}
	else
	{
		//LOG("InternalIP:Port =" + string(intClient) + ":" + string(intPort));
		LOG(
			"external:Port =" + std::string(externalIPAddress) + ":" + std::string(eport) + " " + std::string(proto) + " is redirected to internal " +
			std::string(intClient) + ":" + std::string(intPort) + "(duration=" + std::string(duration) + ")");
	}
	return 0;
}

static int
RemoveRedirect(UPNPUrls* urls,
               IGDdatas* data,
               const char* eport,
               const char* proto,
               const char* remoteHost)
{
	int r;
	if (!proto || !eport)
	{
		LOG("RemoveRedirect: invalid arguments");
		return -1;
	}
	proto = protofix(proto);
	if (!proto)
	{
		LOG("RemoveRedirect: protocol invalid");
		return -1;
	}
	r = UPNP_DeletePortMapping(urls->controlURL, data->first.servicetype, eport, proto, remoteHost);
	if (r != UPNPCOMMAND_SUCCESS)
	{
		LOG("RemoveRedirect: UPNP_DeletePortMapping() failed with code : " + std::to_string(r));
		return -2;
	}
	else
	{
		//LOG("UPNP_DeletePortMapping() succesfull");
	}
	return 0;
}

Miniupnpwrapper::Miniupnpwrapper()
{
}

bool Miniupnpwrapper::Portforward(std::string duration)
{
	int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nResult != NO_ERROR)
	{
		LOG("WSAStartup() failed." + std::to_string(nResult));
		//status = WrapperStatus::InitSuccess;
	}
	const char* multicastif = 0;
	UPNPDev* devlist = 0;
	const char* minissdpdpath = 0;
	int localport = UPNP_LOCAL_PORT_ANY;
	int error = 0;
	int ipv6 = 0;
	unsigned char ttl = 2; /* defaulting to 2 */
	char lanaddr[64] = "unset"; /* my ip address on the LAN */

	if (devlist = upnpDiscover(2000, multicastif, minissdpdpath, localport, ipv6, ttl, &error))
	{
		//LOG("Found upnp devices");
		UPNPUrls urls;
		IGDdatas data;
		if (UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr)))
		{
			int redRes = SetRedirectAndTest(&urls, &data, lanaddr, "7777", "7777", "udp", duration.c_str(), NULL, "RL port forwards for local play", 0);

			if (redRes != 0)
			{
				LOG("Redirect error.. code: " + std::to_string(redRes));
			}

			int redRes2 = SetRedirectAndTest(&urls, &data, lanaddr, "7777", "7777", "tcp", duration.c_str(), NULL, "RL port forwards for local play", 0);
			if (redRes2 != 0)
			{
				LOG("Redirect error.. code: " + std::to_string(redRes));
			}

			bool success = (redRes == 0) && (redRes2 == 0);
			if (success)
			{
				LOG("upnp forwarding added");
			}
			else
			{
				LOG("upnp forwarding failed");
			}
			return success;
		}
		else
		{
			LOG("did not get validGD");
		}
	}
	else
	{
		LOG("Did not get a devlist");
	}
	return false;
}

bool Miniupnpwrapper::RemovPortForward()
{
	int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nResult != NO_ERROR)
	{
		LOG("WSAStartup() failed." + std::to_string(nResult));
		//status = WrapperStatus::InitSuccess;
	}
	const char* multicastif = 0;
	UPNPDev* devlist = 0;
	const char* minissdpdpath = 0;
	int localport = UPNP_LOCAL_PORT_ANY;
	int error = 0;
	int ipv6 = 0;
	unsigned char ttl = 2; /* defaulting to 2 */
	char lanaddr[64] = "unset"; /* my ip address on the LAN */
	if (devlist = upnpDiscover(2000, multicastif, minissdpdpath, localport, ipv6, ttl, &error))
	{
		//LOG("Found upnp devices");
		UPNPUrls urls;
		IGDdatas data;
		if (UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr)))
		{
			int redRes = RemoveRedirect(&urls, &data, "7777", "udp", NULL);
			int redRes2 = RemoveRedirect(&urls, &data, "7777", "tcp", NULL);
			bool success = (redRes == 0) && (redRes2 == 0);
			if (success)
			{
				LOG("upnp forwarding removed");
			}
			else
			{
				LOG("upnp forwarding removal failed");
			}
			return success;
		}
		else
		{
			LOG("did not get validGD");
		}
	}
	else
	{
		LOG("Did not get a devlist");
	}
}
