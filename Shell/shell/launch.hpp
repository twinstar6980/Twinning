#pragma once

#include "shell/base.hpp"
#include "shell/library/library.hpp"
#include "shell/host/host.hpp"

namespace TwinStar::Shell {

	#pragma region launch

	inline auto launch (
		Host &                           host,
		Library &                        library,
		std::string const &              script,
		bool const &                     script_is_path,
		std::vector<std::string> const & argument
	) -> std::optional<std::string> {
		static auto host_manager = std::unordered_map<std::thread::id, Host *>{};
		// TODO : RAII wrapper
		auto thread_id = std::this_thread::get_id();
		host_manager.emplace(thread_id, &host);
		host.start();
		auto result = library.wrapped_execute(
			script,
			script_is_path,
			argument,
			[] (
			Core::StringList const & argument
		) -> Core::StringList const& {
				auto & host = *host_manager[std::this_thread::get_id()];
				return host.wrapped_callback(argument);
			}
		);
		host.finish();
		host_manager.erase(thread_id);
		return result;
	}

	#pragma endregion

}
