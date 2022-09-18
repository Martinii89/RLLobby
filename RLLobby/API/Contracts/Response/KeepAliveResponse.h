#pragma once

namespace rllobby_api::responses
{
	struct KeepAliveResponse
	{
		bool result;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(KeepAliveResponse, result)
}
