#include "pch.h"
#include "HttpUtils.h"

void http_utils::HandleRequest(int expected_http_code, const CurlRequest& req, const std::function<void()>& success_callback,
                               const std::function<void(const std::string&)>& error_callback)
{
	HttpWrapper::SendCurlJsonRequest(req, [expected_http_code, success_callback, error_callback](int response_code, std::string body)
	{
		if (response_code != expected_http_code)
		{
			error_callback(std::format("Request failed({}): {}", response_code, body));
			return;
		}
		success_callback();
	});
}
