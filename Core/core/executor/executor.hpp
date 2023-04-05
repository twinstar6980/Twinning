#pragma once

#include "core/utility/utility.hpp"
#include "core/executor/callback.hpp"
#include "core/executor/context.hpp"
#include "core/executor/interface.hpp"

namespace TwinStar::Core::Executor {

	#pragma region function

	inline auto execute (
		Callback const &     callback,
		String const &       script,
		List<String> const & argument
	) -> String {
		auto guard = std::lock_guard{JavaScript::g_mutex};
		auto context = Context{callback};
		Interface::inject(context);
		auto result = context.context().evaluate(script).call(make_list<JavaScript::Value>(context.context().new_value(argument)));
		if (result.is_exception()) {
			throw JavaScript::ExecutionException{as_lvalue(context.context().query_exception())};
		}
		assert_test(result.is_string());
		return result.get_string();
	}

	#pragma endregion

}
