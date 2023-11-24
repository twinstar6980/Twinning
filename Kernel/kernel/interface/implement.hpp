#pragma once

#include "kernel/executor/executor.hpp"
#include <clocale>

namespace TwinStar::Kernel::Interface::Implement {

	#pragma region function

	inline auto execute (
		Executor::Callback const & callback,
		String const &             script,
		List<String> const &       argument,
		String &                   result
	) -> Void {
		std::setlocale(LC_ALL, "C");
		auto script_value = String{};
		if (script.size() >= 1_sz && script.first() == '$'_c) {
			script_value = script.tail(script.size() - 1_sz);
		}
		else {
			auto script_data = FileSystem::read_file(Path{script});
			script_value.bind(from_byte_view<Character>(script_data.view()));
			script_data.unbind();
		}
		result = Executor::execute(callback, script_value, argument);
		return;
	}

	#pragma endregion

}
