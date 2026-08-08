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

export namespace Twinning::Shell {

	#pragma region type

	class MainConsoleBridgeClient :
		public Bridge::Client {

	protected:

		bool m_running;

	public:

		#pragma region constructor

		virtual ~MainConsoleBridgeClient(
		) override {
			return;
		}

		// ----------------

		MainConsoleBridgeClient(
		) :
			m_running{false} {
			return;
		}

		MainConsoleBridgeClient(
			MainConsoleBridgeClient const & that
		) = delete;

		MainConsoleBridgeClient(
			MainConsoleBridgeClient && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator =(
			MainConsoleBridgeClient const & that
		) -> MainConsoleBridgeClient & = delete;

		auto operator =(
			MainConsoleBridgeClient && that
		) -> MainConsoleBridgeClient & = delete;

		#pragma endregion

		#pragma region life

		virtual auto start(
		) -> void override {
			assert_test(!thiz.m_running);
			thiz.m_running = true;
			return;
		}

		virtual auto finish(
		) -> void override {
			assert_test(thiz.m_running);
			thiz.m_running = false;
			return;
		}

		#pragma endregion

		#pragma region handle

		virtual auto handle(
			std::vector<std::string> const & argument
		) -> std::vector<std::string> override {
			assert_test(thiz.m_running);
			auto result = std::vector<std::string>{};
			assert_test(argument.size() >= 1);
			switch (hash_string(argument[0])) {
				case hash_string("query_context"sv): {
					assert_test(argument.size() == 2);
					auto detail = thiz.handle_query_context(
						argument[1]
					);
					result.emplace_back(std::move(std::get<0>(detail)));
					break;
				}
				case hash_string("check_mode"sv): {
					assert_test(argument.size() == 2);
					auto detail = thiz.handle_check_mode(
						argument[1]
					);
					result.emplace_back(std::get<0>(detail) ? "true"sv : "false"sv);
					break;
				}
				case hash_string("output_text"sv): {
					assert_test(argument.size() == 2);
					auto detail = thiz.handle_output_text(
						argument[1]
					);
					break;
				}
				case hash_string("input_text"sv): {
					assert_test(argument.size() == 1);
					auto detail = thiz.handle_input_text(
					);
					result.emplace_back(std::move(std::get<0>(detail)));
					break;
				}
				default: throw std::runtime_error{std::format("Exception: invalid method")};
			}
			return result;
		}

		// ----------------

		auto handle_query_context(
			std::string const & name
		) -> std::tuple<std::string> {
			assert_test(name == "name"sv || name == "version"sv || name == "terminate"sv);
			auto value = std::string{};
			if (name == "name"sv) {
				value = "basic"sv;
			}
			if (name == "version"sv) {
				value = std::string_view{M_version};
			}
			if (name == "terminate"sv) {
				value = "false"sv;
			}
			return std::make_tuple(value);
		}

		// ----------------

		auto handle_check_mode(
			std::string const & name
		) -> std::tuple<bool> {
			assert_test(name == "input"sv || name == "output"sv);
			auto handle = Interaction::get_handle(name == "input"sv, name == "output"sv);
			auto mode = Interaction::check_mode(handle);
			return std::make_tuple(mode);
		}

		auto handle_output_text(
			std::string const & text
		) -> std::tuple<> {
			Interaction::output_text(text);
			return std::make_tuple();
		}

		auto handle_input_text(
		) -> std::tuple<std::string> {
			auto text = Interaction::input_text();
			return std::make_tuple(text);
		}

		#pragma endregion

	};

	// ----------------

	class MainConsole {

	public:

		#pragma region singleton

		inline static auto instance(
		) -> MainConsole & {
			static auto field = MainConsole{nullptr};
			return field;
		}

		#pragma endregion

	protected:

		#pragma region constructor

		explicit MainConsole(
			std::nullptr_t placeholder
		) {
			return;
		}

		#pragma endregion

	public:

		#pragma region constructor

		~MainConsole(
		) {
			return;
		}

		// ----------------

		MainConsole(
		) = delete;

		MainConsole(
			MainConsole const & that
		) = delete;

		MainConsole(
			MainConsole && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator =(
			MainConsole const & that
		) -> MainConsole & = delete;

		auto operator =(
			MainConsole && that
		) -> MainConsole & = delete;

		#pragma endregion

		#pragma region life

		auto run(
			int const &                  argc,
			void const * const * const & argv,
			void * const &               extra
		) -> int {
			#if defined M_build_release
			try
			#endif
			{
				Interaction::configure_locale();
				Interaction::configure_terminal(true, false);
				Interaction::configure_terminal(false, true);
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
				assert_test(args.size() >= 3);
				auto result = thiz.launch_session(static_cast<Bridge::Service *>(extra), args[1], args[2], std::vector<std::string>{args.begin() + 3, args.end()});
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

		#pragma region utility

		auto launch_session(
			Bridge::Service * const &        kernel_library_symbol,
			std::string const &              kernel,
			std::string const &              script,
			std::vector<std::string> const & argument
		) -> std::optional<std::vector<std::string>> {
			auto result = std::optional<std::vector<std::string>>{};
			auto exception = std::optional<std::string>{};
			try {
				auto client = MainConsoleBridgeClient{};
				auto library = kernel_library_symbol != nullptr ? Bridge::Library{kernel_library_symbol} : Bridge::Library{kernel};
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

	};

	#pragma endregion

}
