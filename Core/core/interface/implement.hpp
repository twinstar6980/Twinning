#pragma once

#include "core/executor/executor.hpp"
#include <clocale>

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

	inline auto prepare (
	) -> Optional<String> {
		auto result = Optional<String>{};
		std::setlocale(LC_ALL, "C");
		// NOTE : LC_NUMERIC的设置会影响quickjs对字符串的解析，这在一些不以点作为小数点的地区（如俄罗斯）本地化设置下将导致脚本运行出错
		std::setlocale(LC_CTYPE, ".utf-8");
		return result;
	}

	#pragma endregion

}
