#include "pch.h"
#include "jsonUtils.h"

#include <signalrclient/signalr_value.h>

char getBase64Value(uint32_t i)
{
	char index = (char)i;
	if (index < 26)
	{
		return 'A' + index;
	}
	if (index < 52)
	{
		return 'a' + (index - 26);
	}
	if (index < 62)
	{
		return '0' + (index - 52);
	}
	if (index == 62)
	{
		return '+';
	}
	if (index == 63)
	{
		return '/';
	}

	throw std::out_of_range("base64 table index out of range: " + std::to_string(index));
}

std::string base64Encode(const std::vector<uint8_t>& data)
{
	std::string base64result;

	size_t i = 0;
	while (i <= data.size() - 3)
	{
		uint32_t b = ((uint32_t)data[i] << 16) | ((uint32_t)data[i + 1] << 8) | (uint32_t)data[i + 2];
		base64result.push_back(getBase64Value((b >> 18) & 0x3F));
		base64result.push_back(getBase64Value((b >> 12) & 0x3F));
		base64result.push_back(getBase64Value((b >> 6) & 0x3F));
		base64result.push_back(getBase64Value(b & 0x3F));

		i += 3;
	}
	if (data.size() - i == 2)
	{
		uint32_t b = ((uint32_t)data[i] << 8) | (uint32_t)data[i + 1];
		base64result.push_back(getBase64Value((b >> 10) & 0x3F));
		base64result.push_back(getBase64Value((b >> 4) & 0x3F));
		base64result.push_back(getBase64Value((b << 2) & 0x3F));
		base64result.push_back('=');
	}
	else if (data.size() - i == 1)
	{
		uint32_t b = (uint32_t)data[i];
		base64result.push_back(getBase64Value((b >> 2) & 0x3F));
		base64result.push_back(getBase64Value((b << 4) & 0x3F));
		base64result.push_back('=');
		base64result.push_back('=');
	}

	return base64result;
}

//shamelessly converted from https://github.com/aspnet/SignalR-Client-Cpp/blob/main/src/signalrclient/json_helpers.cpp


json signalr::json_utils::CreateJson(const signalr::value& v)
{
	switch (v.type())
	{
	case signalr::value_type::map:
		{
			const auto& obj = v.as_map();
			auto json_obj = json::object();
			for (const auto& [name, value] : obj)
			{
				json_obj[name] = CreateJson(value);
			}
			return json_obj;
		}
	case signalr::value_type::array: ;
		{
			const auto& array = v.as_array();
			auto json_array = json::array();
			for (auto& value : array)
			{
				json_array.push_back(CreateJson(value));
			}
			return json_array;
		}

	case signalr::value_type::string: ;
		{
			return v.as_string();
		}
	case signalr::value_type::float64: ;
		{
			auto value = v.as_double();
			double intPart;
			// Workaround for 1.0 being output as 1.0 instead of 1
			// because the server expects certain values to be 1 instead of 1.0 (like protocol version)
			if (std::modf(value, &intPart) == 0)
			{
				if (value < 0)
				{
					if (value >= (double)INT64_MIN)
					{
						// Fits within int64_t
						return (static_cast<int64_t>(intPart));
					}
					// Remain as double
					return (value);
				}
				if (value <= (double)UINT64_MAX)
				{
					// Fits within uint64_t
					return (static_cast<uint64_t>(intPart));
				}
				// Remain as double
				return (value);
			}
			return v.as_double();
		}
	case signalr::value_type::null: ;
		{
			return nullptr;
		}
	case signalr::value_type::boolean: ;
		{
			return v.as_bool();
		}
	case signalr::value_type::binary: ;
		{
			const auto& bin = v.as_binary();
			return base64Encode(bin);
		}
	}
	throw std::exception("bad type?");
}
