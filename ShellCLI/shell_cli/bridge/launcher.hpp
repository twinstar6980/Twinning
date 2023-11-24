#pragma once

#include "shell_cli/common.hpp"
#include "shell_cli/bridge/library.hpp"
#include "shell_cli/bridge/invoker.hpp"
#include "shell_cli/bridge/host.hpp"

namespace TwinStar::ShellCLI::Bridge {

	#pragma region type

	struct Launcher {

		#pragma region function

		static auto launch (
			Host &                           host,
			Library &                        library,
			std::string const &              script,
			std::vector<std::string> const & argument
		) -> std::string {
			auto result = std::string{};
			auto exception = std::optional<std::exception_ptr>{};
			host.start();
			try {
				result = Invoker::execute(library, std::bind(&Host::execute, &host, std::placeholders::_1), script, argument);
			}
			catch (...) {
				exception.emplace(std::current_exception());
			}
			host.finish();
			if (exception.has_value()) {
				std::rethrow_exception(exception.value());
			}
			return result;
		}

		#pragma endregion

	};

	#pragma endregion

}
