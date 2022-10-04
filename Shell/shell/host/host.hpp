#pragma once

#include "shell/base.hpp"
#include "shell/library/library.hpp"

namespace TwinKleS::Shell {

	#pragma region type

	class Host {

	public: //

		#pragma region structor

		virtual ~Host (
		) = default;

		// ----------------

		Host (
		) = default;

		Host (
			Host const & that
		) = delete;

		Host (
			Host && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Host const & that
		) -> Host& = delete;

		auto operator = (
			Host && that
		) -> Host& = delete;

		#pragma endregion

		#pragma region life-time

		virtual auto running (
		) -> bool = 0;

		virtual auto start (
		) -> void = 0;

		virtual auto finish (
		) -> void = 0;

		#pragma endregion

		#pragma region callback

		virtual auto callback (
			std::vector<std::string> const & argument
		) -> std::vector<std::string> = 0;

		#pragma endregion

		#pragma region callback wrapper

		auto wrapped_callback (
			Core::StringList const & argument
		) -> Core::StringList const& {
			thread_local auto result_handler = CTypeStringListHandler{};
			// result handler control result structure
			auto result = std::vector<std::string>{};
			try {
				auto argument_value = from_string_list_structure(argument);
				auto valid_result = thiz.callback(argument_value);
				result.emplace_back(""s);
				result.insert(result.end(), std::make_move_iterator(valid_result.begin()), std::make_move_iterator(valid_result.end()));
			} catch (std::exception & exception) {
				result.emplace_back(""s + typeid(exception).name() + " : " + exception.what());
			} catch (...) {
				result.emplace_back("unknown exception"s);
			}
			result_handler.imbue(allocate_string_list_structure(result));
			return result_handler.value();
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region function

	inline auto execute_on_host (
		Host &                           host,
		Library &                        library,
		std::string const &              script,
		bool const &                     script_is_path,
		std::vector<std::string> const & argument
	) -> std::optional<std::string> {
		static auto host_manager = std::unordered_map<std::thread::id, Host *>{};
		// host manager
		auto force_exit_before_execute = false;
		auto result = std::optional<std::string>{};
		auto work_thread = std::thread{
			[&] {
				while (!force_exit_before_execute && !host.running()) {
					std::this_thread::yield();
				}
				if (!force_exit_before_execute) {
					// TODO : RAII wrapper
					auto thread_id = std::this_thread::get_id();
					host_manager.emplace(thread_id, &host);
					result = library.wrapped_execute(
						script,
						script_is_path,
						argument,
						[] (
						Core::StringList const & argument
					) -> Core::StringList const& {
							auto & host = *host_manager[std::this_thread::get_id()];
							return host.wrapped_callback(argument);
						}
					);
					host_manager.erase(thread_id);
					host.finish();
				}
			}
		};
		try {
			host.start();
		} catch (...) {
			force_exit_before_execute = true;
			work_thread.join();
			throw;
		}
		work_thread.join();
		return result;
	}

	#pragma endregion

}
