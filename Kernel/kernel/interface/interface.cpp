//

#if defined M_vld
#include "vld.h"
#endif

#include "kernel/common.hpp"

import twinning.kernel.utility;
import twinning.kernel.interface.data;
import twinning.kernel.interface.service;
import twinning.kernel.interface.proxy;
import twinning.kernel.executor.executor;

namespace Twinning::Kernel::Interface {

	#pragma region implement

	static auto service_executor_execute (
		ExecutorProxy const & callback,
		String const &        script,
		List<String> const &  argument
	) -> Tuple<List<String>> {
		auto script_text = String{};
		if (script.size() >= 1_sz && script.first() == '$'_c) {
			script_text = script.tail(script.size() - 1_sz);
		}
		else {
			auto script_data = Storage::read_file(Path{script});
			script_text.bind(from_byte_view<Character>(script_data.view()));
			script_data.unbind();
		}
		auto result = Kernel::Executor::execute(callback, script_text, argument);
		return make_tuple_of(result);
	}

	// ----------------

	static auto service_executor (
		ExecutorProxy const & callback_proxy,
		MessageProxy const &  argument_proxy,
		MessageProxy &        result_proxy
	) -> Void {
		std::setlocale(stddef::$LC_ALL, "C");
		auto & argument = argument_proxy.value;
		auto & result = result_proxy.value;
		switch (argument[1_ix].hash().value) {
			case "execute"_shz : {
				auto & detail_script = argument[2_ix];
				auto   detail_argument = List<String>{argument.tail(argument.size() - 3_ix)};
				auto   detail = service_executor_execute(
					callback_proxy,
					detail_script,
					detail_argument
				);
				auto & detail_result = detail.get<1_ix>();
				result.append_list(detail_result);
				break;
			}
			default : {
				throw UnnamedException{mss("invalid method"_sv)};
			}
		}
		return;
	}

	#pragma endregion

	#pragma region interface

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wextern-initializer"
	#pragma clang diagnostic ignored "-Wmissing-variable-declarations"

	#if defined M_system_windows
	__declspec(dllexport)
	#endif
	#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
	__attribute__((visibility("default")))
	#endif
	extern "C++" Service service = Service{
		.executor = nullptr,
		.initialize = [] () {
			service.executor = new Executor{};
			ExecutorProxy::construct(*service.executor, ExecutorProxy{&service_executor});
			return;
		},
		.finalize = [] () {
			ExecutorProxy::destruct(*service.executor);
			delete service.executor;
			return;
		},
	};

	#pragma clang diagnostic pop

	#pragma endregion

}
