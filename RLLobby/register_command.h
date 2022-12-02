#pragma once

#include <vector>
#include <string>
#include <optional>
#include "utils/parser.h"

namespace bm_commands
{
	template <typename T>
	concept BmCommandArgument =
	requires(T, const std::vector<std::string>& args)
	{
		{ T::Parse(args) } -> std::convertible_to<std::optional<T>>;
	};

	template <BmCommandArgument T>
	void RegisterCommand(const std::shared_ptr<CVarManagerWrapper>& cv, const std::string& command, std::function<void(const T&)> callback,
	                     const std::string& description = "", unsigned char permissions = 0)
	{
		cv->registerNotifier(command, [cv, callback](std::vector<std::string> args)
		{
			std::optional<T> command_argument = T::Parse(args);
			if (command_argument.has_value())
			{
				callback(command_argument.value());
			}
			else
			{
				cv->log("Failed to parse argument");
			}
		}, description, permissions);
	};
}
