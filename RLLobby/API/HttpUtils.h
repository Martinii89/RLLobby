#pragma once

#include <string>
#include "nlohmann/json.hpp"
#include "bakkesmod/core/http_structs.h"

namespace http_utils
{
	template <typename T>
	struct ParseResult
	{
		std::string error_message;
		std::optional<T> value;
	};


	template <typename T>
	ParseResult<T> TryParse(const std::string& body)
	{
		try
		{
			json json_body = json::parse(body.begin(), body.end());
			auto res = json_body.get<T>();
			return ParseResult<T>{"", res};
		}
		catch (std::exception& e)
		{
			return ParseResult<T>{std::format("Body was: {}\n Exception: {}", body,  e.what()), {}};
		}
	}

	template <typename T>
	void HandleRequest(
		int expected_http_code,
		const CurlRequest& req,
		const std::function<void(const T&)>& success_callback,
		const std::function<void(const std::string&)>& error_callback)
	{
		HttpWrapper::SendCurlJsonRequest(req, [expected_http_code, success_callback, error_callback](int response_code, std::string body)
		{
			if (response_code != expected_http_code)
			{
				error_callback(std::format("Request failed({}): {}", response_code, body));
				return;
			}

			const auto [error_message, value] = TryParse<T>(body);
			if (value.has_value())
			{
				success_callback(value.value());
			}
			else if (error_callback)
			{
				error_callback(error_message);
			}
		});
	}

	void HandleRequest(
		int expected_http_code,
		const CurlRequest& req,
		const std::function<void()>& success_callback,
		const std::function<void(const std::string&)>& error_callback);
}
