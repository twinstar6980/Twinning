#pragma once

#include "shell/common.hpp"
#include "shell/utility/exception.hpp"

namespace TwinStar::Shell {

	#pragma region proxy dynamic function

	template <auto id, typename Result, typename ... Argument, typename Function>
	inline auto proxy_dynamic_function_in_current_thread (
		Function const & function
	) -> Result(*) (Argument ...) {
		static auto map = std::unordered_map<std::thread::id, Function>{};
		//
		auto thread_id = std::this_thread::get_id();
		auto item_pair = map.try_emplace(thread_id, function);
		assert_test(item_pair.second);
		return [] (Argument ... argument) -> Result {
			auto thread_id = std::this_thread::get_id();
			return map.at(thread_id)(std::forward<Argument>(argument) ...);
		};
	}

	#pragma endregion

}
