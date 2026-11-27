// twinning.kernel.interface

#include "kernel/common.hpp"

import twinning.kernel.utility;
import twinning.kernel.interface.data;
import twinning.kernel.interface.service;
import twinning.kernel.interface.proxy;
import twinning.kernel.executor.executor;

namespace Twinning::Kernel::Interface {

	#pragma region implement

	static auto service_executor_execute(
		ExecutorProxy const & callback,
		String const &        script,
		List<String> const &  argument
	) -> Tuple<List<String>> {
		auto script_text = String{};
		if (script.size() >= 1_sz && script.first() == '$'_c) {
			script_text = script.tail(script.size() - 1_sz);
		}
		else {
			auto script_path = Path{script};
			auto script_data = Binary{Storage::size_file(script_path)};
			Storage::read_file(script_path, 0_sz, script_data.as_view());
			script_text.bind(cast_pointer<Character>(script_data.data()), script_data.size(), script_data.capacity());
			script_data.unbind();
		}
		auto result = Kernel::Executor::execute(callback, script_text, argument);
		return make_tuple_of(result);
	}

	// ----------------

	static auto service_executor(
		ExecutorProxy const & callback_proxy,
		MessageProxy const &  argument_proxy,
		MessageProxy &        result_proxy
	) -> Void {
		auto & argument = argument_proxy.value;
		auto & result = result_proxy.value;
		assert_test(argument.size() >= 1_sz);
		switch (argument[1_ix].hash().value) {
			case "execute"_shz: {
				assert_test(argument.size() >= 2_sz);
				auto & detail_script = argument[2_ix];
				auto   detail_argument = make_list_of<String>(Range::slice_tail(argument, argument.size() - 3_ix));
				auto   detail = service_executor_execute(
					callback_proxy,
					detail_script,
					detail_argument
				);
				auto & detail_result = detail.get<1_ix>();
				result.append_range(Range::make_moveable_range_of(detail_result));
				break;
			}
			default: {
				throw UnnamedException{to_std_string("invalid method"_sv)};
			}
		}
		return;
	}

	#pragma endregion

	#pragma region interface

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wmissing-variable-declarations"

	// @formatter:off
	__attribute__((visibility("default")))
	// @formatter:on
	extern "C++" Service service = Service{
		.executor = nullptr,
		.initialize = []() {
			assert_test(service.executor == nullptr);
			service.executor = new Executor{};
			ExecutorProxy::construct(*service.executor, ExecutorProxy{Function<Void, ExecutorProxy const &, MessageProxy const &, MessageProxy &>{&service_executor}});
			return;
		},
		.finalize = []() {
			assert_test(service.executor != nullptr);
			ExecutorProxy::destruct(*service.executor);
			delete service.executor;
			service.executor = nullptr;
			return;
		},
	};

	#pragma clang diagnostic pop

	#pragma endregion

}
