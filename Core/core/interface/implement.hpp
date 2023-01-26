#pragma once

#include "core/executor/executor.hpp"

namespace TwinStar::Core::Interface::Implement {

	#pragma region function

	inline auto version (
	) -> Size {
		return mbw<Size>(M_version);
	}

	inline auto execute (
		Executor::Callback const & callback,
		String const &             script,
		List<String> const &       argument
	) -> Optional<String> {
		auto script_value = String{};
		if (script.size() >= 1_sz && script.first() == '?'_c) {
			auto script_data = FileSystem::read_file(Path{String{script.tail(script.size() - 1_sz)}});
			// NOTE : avoid clang bug : explicit provide template argument <ListView>
			script_value.bind(from_byte_view<Character, ListView>(script_data.view()));
			script_data.unbind();
		} else {
			script_value = script;
		}
		return Executor::execute(callback, script_value, argument);
	}

	#pragma endregion

}
