module;

#include "kernel/common.hpp"

export module twinning.kernel.executor.executor;
import twinning.kernel.utility;
import twinning.kernel.executor.context;
import twinning.kernel.executor.environment;
import twinning.kernel.interface.proxy;

export namespace Twinning::Kernel::Executor {

	#pragma region function

	inline auto execute (
		Interface::ExecutorProxy const & callback,
		String const &                   script,
		List<String> const &             argument
	) -> List<String> {
		auto locker = Thread::Locker{JavaScript::g_mutex};
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
