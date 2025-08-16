// twinning.shell.main

#include "shell/common.hpp"

import twinning.shell.utility.exception;
import twinning.shell.utility.library;
import twinning.shell.utility.interaction;
import twinning.shell.utility.system_native_string;
import twinning.shell.bridge.library;
import twinning.shell.bridge.launcher;
import twinning.shell.main_console_bridge_client;
import twinning.shell.third.system.windows;

namespace Twinning::Shell::Main {

	#pragma region implement

	static auto launch_session (
		std::string const &              kernel,
		std::string const &              script,
		std::vector<std::string> const & argument
	) -> std::optional<std::vector<std::string>> {
		auto result = std::optional<std::vector<std::string>>{};
		auto exception = std::optional<std::string>{};
		try {
			auto client = MainConsoleBridgeClient{};
			auto library = Bridge::Library{
				#if defined M_kernel_library_linked
				&Twinning::Kernel::Interface::service
				#else
				kernel
				#endif
			};
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

	// ----------------

	static auto run (
		int const &                  argc,
		void const * const * const & argv
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
			auto result = launch_session(args[1], args[2], std::vector<std::string>{args.begin() + 3, args.end()});
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

}

#pragma region main

#if defined M_system_windows
auto wmain (
	int       argc,
	wchar_t * argv[]
)
#endif
#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
auto main (
	int    argc,
	char * argv[]
)
#endif
	-> int {
	return Twinning::Shell::Main::run(argc, reinterpret_cast<void * *>(argv));
}

#pragma endregion
