#pragma once

#include "shell/common.hpp"
#include "shell/bridge/library.hpp"
#include "shell/bridge/invoker.hpp"
#include "shell/bridge/host.hpp"

namespace TwinStar::Shell::Bridge {

	#pragma region type

	struct Launcher {

		#pragma region function

		static auto launch (
			Host &                           host,
			Library &                        library,
			std::string const &              script,
			std::vector<std::string> const & argument
		) -> std::string {
			host.start();
			Invoker::version(library);
			Invoker::prepare(library);
			auto callback = std::bind(&Host::execute, &host, std::placeholders::_1);
			auto result = Invoker::execute(library, callback, script, argument);
			host.finish();
			return result;
		}

		#pragma endregion

	};

	#pragma endregion

}
