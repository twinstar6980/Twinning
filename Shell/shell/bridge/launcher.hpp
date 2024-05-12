#pragma once

#include "shell/common.hpp"
#include "shell/bridge/proxy.hpp"
#include "shell/bridge/library.hpp"
#include "shell/bridge/client.hpp"

namespace TwinStar::Shell::Bridge {

	#pragma region type

	struct Launcher {

		#pragma region function

		static auto launch (
			Client &                         client,
			Library const &                  library,
			std::string const &              script,
			std::vector<std::string> const & argument
		) -> std::vector<std::string> {
			auto result = std::vector<std::string>{};
			auto exception = std::exception_ptr{};
			client.start();
			#if defined M_build_release
			try
			#endif
			{
				auto executor_callback = ExecutorProxy{};
				executor_callback.value = [&client] (auto & callback_proxy, auto & argument_proxy, auto & result_proxy) -> auto {
					result_proxy.value = client.callback(argument_proxy.value);
					return;
				};
				auto executor_argument = MessageProxy{};
				executor_argument.value.emplace_back("execute"s);
				executor_argument.value.emplace_back(script);
				executor_argument.value.insert(executor_argument.value.end(), argument.begin(), argument.end());
				auto executor_result = MessageProxy{};
				ExecutorProxy::parse(*library.symbol().executor).value(
					executor_callback,
					executor_argument,
					executor_result
				);
				result = std::move(executor_result.value);
			}
			#if defined M_build_release
			catch (...) {
				exception = std::current_exception();
			}
			#endif
			client.finish();
			if (exception != nullptr) {
				std::rethrow_exception(exception);
			}
			return result;
		}

		#pragma endregion

	};

	#pragma endregion

}
