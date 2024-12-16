module;

#include "shell/common.hpp"

export module twinning.shell.main_console_bridge_client;
import twinning.shell.utility.string;
import twinning.shell.utility.interaction;
import twinning.shell.bridge.client;

export namespace Twinning::Shell {

	#pragma region type

	class MainConsoleBridgeClient :
		public Bridge::Client {

	protected:

		bool m_running;

	public:

		#pragma region structor

		virtual ~MainConsoleBridgeClient (
		) override {
		}

		// ----------------

		MainConsoleBridgeClient (
		) :
			m_running{false} {
		}

		MainConsoleBridgeClient (
			MainConsoleBridgeClient const & that
		) = delete;

		MainConsoleBridgeClient (
			MainConsoleBridgeClient && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator = (
			MainConsoleBridgeClient const & that
		) -> MainConsoleBridgeClient & = delete;

		auto operator = (
			MainConsoleBridgeClient && that
		) -> MainConsoleBridgeClient & = delete;

		#pragma endregion

		#pragma region life

		virtual auto start (
		) -> void override {
			assert_test(!thiz.m_running);
			thiz.m_running = true;
			return;
		}

		virtual auto finish (
		) -> void override {
			assert_test(thiz.m_running);
			thiz.m_running = false;
			return;
		}

		// ----------------

		virtual auto callback (
			std::vector<std::string> const & argument
		) -> std::vector<std::string> override {
			assert_test(thiz.m_running);
			auto result = std::vector<std::string>{};
			assert_test(argument.size() >= 1);
			switch (hash_string(argument[0])) {
				case hash_string("name"sv) : {
					assert_test(argument.size() == 1);
					auto   detail = thiz.callback_name();
					auto & detail_name = std::get<0>(detail);
					result.emplace_back(std::move(detail_name));
					break;
				}
				case hash_string("version"sv) : {
					assert_test(argument.size() == 1);
					auto   detail = thiz.callback_version();
					auto & detail_version = std::get<0>(detail);
					result.emplace_back(std::move(detail_version));
					break;
				}
				case hash_string("output_text"sv) : {
					assert_test(argument.size() == 2);
					auto & detail_text = argument[1];
					auto   detail = thiz.callback_output_text(detail_text);
					break;
				}
				case hash_string("input_text"sv) : {
					assert_test(argument.size() == 1);
					auto   detail = thiz.callback_input_text();
					auto & detail_text = std::get<0>(detail);
					result.emplace_back(std::move(detail_text));
					break;
				}
				case hash_string("pick_storage_item"sv) : {
					assert_test(argument.size() == 2);
					auto & detail_type = argument[1];
					auto   detail = thiz.callback_pick_storage_item(detail_type);
					auto & detail_target = std::get<0>(detail);
					result.emplace_back(std::move(detail_target));
					break;
				}
				case hash_string("push_system_notification"sv) : {
					assert_test(argument.size() == 3);
					auto & detail_title = argument[1];
					auto & detail_description = argument[2];
					auto   detail = thiz.callback_push_system_notification(detail_title, detail_description);
					break;
				}
				default : {
					throw std::runtime_error{"invalid method"s};
				}
			}
			return result;
		}

		auto callback_name (
		) -> std::tuple<std::string> {
			auto name = std::string{"basic"};
			return std::make_tuple(name);
		}

		auto callback_version (
		) -> std::tuple<std::string> {
			auto version = std::string{M_version};
			return std::make_tuple(version);
		}

		auto callback_output_text (
			std::string const & text
		) -> std::tuple<> {
			Interaction::output_text(text);
			return std::make_tuple();
		}

		auto callback_input_text (
		) -> std::tuple<std::string> {
			auto text = Interaction::input_text();
			return std::make_tuple(text);
		}

		auto callback_pick_storage_item (
			std::string const & type
		) -> std::tuple<std::string> {
			auto target = Interaction::pick_storage_item(type).value_or(""s);
			return std::make_tuple(target);
		}

		auto callback_push_system_notification (
			std::string const & title,
			std::string const & description
		) -> std::tuple<> {
			Interaction::push_system_notification(title, description);
			return std::make_tuple();
		}

		#pragma endregion

	};

	#pragma endregion

}
