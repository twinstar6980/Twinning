#pragma once

#include "shell/common.hpp"
#include "shell/host/host.hpp"
#include "shell/core/library.hpp"
#include "shell/core/invoker.hpp"

namespace TwinStar::Shell {

	#pragma region type

	struct Launcher {

		#pragma region function

		static auto launch (
			Host &                           host,
			Core::Library &                  core,
			std::string const &              script,
			std::vector<std::string> const & argument
		) -> std::string {
			host.start();
			Core::Invoker::version(core);
			Core::Invoker::prepare(core);
			auto callback = std::bind(&Host::execute, &host, std::placeholders::_1);
			auto result = Core::Invoker::execute(core, callback, script, argument);
			host.finish();
			return result;
		}

		#pragma endregion

	};

	#pragma endregion

}
