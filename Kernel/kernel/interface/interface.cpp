//

#include "kernel/utility/utility.hpp"
#include "kernel/interface/service.hpp"
#include "kernel/interface/proxy.hpp"
#include "kernel/executor/executor.hpp"
#include <clocale>

#if defined M_vld
#include "vld.h"
#endif

namespace TwinStar::Kernel::Interface {

	#pragma region implement

	inline auto service_executor_execute (
		ExecutorProxy const & callback,
		String const &        script,
		List<String> const &  argument
	) -> Tuple<List<String>> {
		auto script_text = String{};
		if (script.size() >= 1_sz && script.first() == '$'_c) {
			script_text = script.tail(script.size() - 1_sz);
		}
		else {
			auto script_data = FileSystem::read_file(Path{script});
			script_text.bind(from_byte_view<Character>(script_data.view()));
			script_data.unbind();
		}
		auto result = Kernel::Executor::execute(callback, script_text, argument);
		return make_tuple_of(result);
	}

	// ----------------

	inline auto service_executor (
		ExecutorProxy const & callback_proxy,
		MessageProxy const &  argument_proxy,
		MessageProxy &        result_proxy
	) -> Void {
		std::setlocale(LC_ALL, "C");
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

	#if defined M_compiler_msvc
	#pragma warning(push)
	#endif
	#if defined M_compiler_clang
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wextern-initializer"
	#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
	#endif

	#if defined M_compiler_msvc
	__declspec(dllexport)
	#endif
	#if defined M_compiler_clang
	__attribute__((visibility("default")))
	#endif
	extern Service service = Service{};

	#if defined M_compiler_msvc
	#pragma warning(pop)
	#endif
	#if defined M_compiler_clang
	#pragma clang diagnostic pop
	#endif

	// ----------------

	static auto _ = Finalizer{make_finalizer(
		[] {
			service.executor = new Executor{};
			ExecutorProxy::construct(*service.executor, ExecutorProxy{&service_executor});
		},
		[] {
			ExecutorProxy::destruct(*service.executor);
			delete service.executor;
		}
	)};

	#pragma endregion

}
