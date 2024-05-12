#pragma once

#include "kernel/interface/proxy.hpp"
#include "kernel/executor/executor.hpp"
#include <clocale>

namespace TwinStar::Kernel::Interface::Implement {

	#pragma region executor

	inline auto executor_execute (
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

	inline auto executor (
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
				auto   detail = executor_execute(
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

}
