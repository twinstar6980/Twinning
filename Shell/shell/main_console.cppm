module;

#include "shell/common.hpp"

export module twinning.shell.main_console;
import twinning.shell.utility.exception;
import twinning.shell.utility.string;
import twinning.shell.utility.interaction;
import twinning.shell.utility.system_native_string;
import twinning.shell.bridge.service;
import twinning.shell.bridge.library;
import twinning.shell.bridge.client;
import twinning.shell.bridge.launcher;
import twinning.shell.third.system.windows;

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
			return;
		}

		// ----------------

		MainConsoleBridgeClient (
		) :
			m_running{false} {
			return;
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
				case hash_string("name"sv): {
					assert_test(argument.size() == 1);
					auto   detail = thiz.callback_name();
					auto & detail_name = std::get<0>(detail);
					result.emplace_back(std::move(detail_name));
					break;
				}
				case hash_string("version"sv): {
					assert_test(argument.size() == 1);
					auto   detail = thiz.callback_version();
					auto & detail_version = std::get<0>(detail);
					result.emplace_back(std::move(detail_version));
					break;
				}
				case hash_string("output_text"sv): {
					assert_test(argument.size() == 2);
					auto & detail_text = argument[1];
					auto   detail = thiz.callback_output_text(detail_text);
					break;
				}
				case hash_string("input_text"sv): {
					assert_test(argument.size() == 1);
					auto   detail = thiz.callback_input_text();
					auto & detail_text = std::get<0>(detail);
					result.emplace_back(std::move(detail_text));
					break;
				}
				default: throw std::runtime_error{std::format("Exception: invalid method")};
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

		#pragma endregion

	};

	// ----------------

	struct MainConsole {

		#pragma region utility

		inline static auto launch_session (
			Bridge::Service * const &        kernel_symbol,
			std::string const &              kernel,
			std::string const &              script,
			std::vector<std::string> const & argument
		) -> std::optional<std::vector<std::string>> {
			auto result = std::optional<std::vector<std::string>>{};
			auto exception = std::optional<std::string>{};
			try {
				auto client = MainConsoleBridgeClient{};
				auto library = kernel_symbol != nullptr ? Bridge::Library{kernel_symbol} : Bridge::Library{kernel};
				result.emplace(Bridge::Launcher::launch(client, library, script, argument));
			}
			catch (...) {
				exception.emplace(parse_current_exception());
			}
			if (!exception.has_value()) {
				Interaction::output_text("SUCCEEDED");
				Interaction::output_text("\n");
				for (auto & result_item : result.value()) {
					Interaction::output_text(result_item);
					Interaction::output_text("\n");
				}
			}
			else {
				Interaction::output_text("FAILED");
				Interaction::output_text("\n");
				Interaction::output_text(exception.value());
				Interaction::output_text("\n");
			}
			return exception.has_value() ? std::nullopt : result;
		}

		#pragma endregion

		#pragma region life

		inline static auto run (
			int const &                  argc,
			void const * const * const & argv,
			void * const &               extra
		) -> int {
			#if defined M_build_release
			try
			#endif
			{
				auto args = std::vector<std::string>{};
				args.reserve(static_cast<std::size_t>(argc));
				for (auto & arg : std::span{argv, static_cast<std::size_t>(argc)}) {
					#if defined M_system_windows
					args.emplace_back(SystemNativeString::wide_to_utf8(std::wstring_view{static_cast<wchar_t const *>(arg)}));
					#endif
					#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
					args.emplace_back(static_cast<char const *>(arg));
					#endif
				}
				#if defined M_system_windows
				Third::system::windows::$SetProcessPreferredUILanguages(Third::system::windows::$MUI_LANGUAGE_NAME, L"en-US\0\0", nullptr);
				#endif
				std::locale::global(std::locale::classic());
				assert_test(args.size() >= 3);
				auto result = launch_session(static_cast<Bridge::Service *>(extra), args[1], args[2], std::vector<std::string>{args.begin() + 3, args.end()});
				return result.has_value() ? 0 : 1;
			}
			#if defined M_build_release
			catch (...) {
				auto exception = parse_current_exception();
				Interaction::output_text("EXCEPTION");
				Interaction::output_text("\n");
				Interaction::output_text(exception);
				Interaction::output_text("\n");
				return 1;
			}
			#endif
		}

		#pragma endregion

	};

	#pragma endregion

}
