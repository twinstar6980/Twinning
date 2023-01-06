#pragma once

#include "core/executor/executor.hpp"

namespace TwinStar::Core::Interface::Implement {

	#pragma region function

	inline auto version (
	) -> Size {
		return mbw<Size>(M_version);
	}

	inline auto execute (
		String const &                  script,
		Boolean const &                 script_is_path,
		List<String> const &            argument,
		Executor::ShellCallback const & shell_callback
	) -> Optional<String> {
		auto script_text = String{};
		auto script_path = Optional<Path>{};
		if (!script_is_path) {
			script_text = script;
		} else {
			script_path.set(script);
			auto data = FileSystem::read_file(Path{script});
			// NOTE : avoid clang bug : explicit provide template argument <ListView>
			script_text.bind(from_byte_view<Character, ListView>(data.view()));
			data.unbind();
		}
		return Executor::execute(script_text, script_path, argument, shell_callback);
	}

	#pragma endregion

}
