#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/executor/callback.hpp"
#include "kernel/executor/context.hpp"
#include "kernel/executor/interface.hpp"

namespace TwinStar::Kernel::Executor {

	#pragma region function

	inline auto execute (
		Callback const &     callback,
		String const &       script,
		List<String> const & argument
	) -> String {
		auto guard = std::lock_guard{JavaScript::g_mutex};
		auto context = Context{callback};
		Interface::inject(context);
		auto data = context.context().new_value();
		data.set_object_of_object();
		data.define_object_property("argument"_s, context.context().new_value(argument));
		data.define_object_property("result"_s, context.context().new_value());
		data.define_object_property("error"_s, context.context().new_value());
		auto state = context.context().evaluate(script, "<main>"_s, k_false).call(make_list<JavaScript::Value>(data));
		while (context.runtime().has_pending_job()) {
			auto job_context = JavaScript::Context::new_reference(nullptr);
			context.runtime().execute_pending_job(job_context);
		}
		assert_test(state.is_undefined());
		auto result = data.get_object_property("result"_s).to_of<String>();
		auto error = data.get_object_property("error"_s);
		if (!error.is_undefined()) {
			throw JavaScript::ExecutionException{as_lvalue(error)};
		}
		return result;
	}

	#pragma endregion

}
