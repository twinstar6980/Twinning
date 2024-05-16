#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/executor/context.hpp"
#include "kernel/executor/environment.hpp"
#include "kernel/interface/proxy.hpp"

namespace TwinStar::Kernel::Executor {

	#pragma region function

	inline auto execute (
		Interface::ExecutorProxy const & callback,
		String const &                   script,
		List<String> const &             argument
	) -> List<String> {
		auto lock = std::lock_guard{JavaScript::g_mutex};
		auto context = Context{callback};
		Environment::inject(context);
		auto data = context.context().new_value();
		data.set_object_of_object();
		data.define_object_property("argument"_s, context.context().new_value(argument));
		data.define_object_property("result"_s, context.context().new_value());
		data.define_object_property("exception"_s, context.context().new_value());
		auto state = context.context().evaluate(script, "<main>"_s, k_false).call(make_list<JavaScript::Value>(data));
		while (context.runtime().has_pending_job()) {
			context.runtime().execute_pending_job(as_lvalue(JavaScript::Context::new_reference(nullptr)));
		}
		assert_test(state.is_undefined());
		auto result = data.get_object_property("result"_s).to_of<List<String>>();
		auto exception = data.get_object_property("exception"_s);
		if (!exception.is_undefined()) {
			throw JavaScript::ExecutionException{as_lvalue(exception)};
		}
		return result;
	}

	#pragma endregion

}
