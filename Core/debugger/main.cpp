//

#include "shell/library/static_library.hpp"
#include "shell/host/cli_host.hpp"
#include "shell/launch.hpp"
#include <span>
#include <iostream>

#if defined M_vld
#include "vld.h"
#endif

// ----------------

#if defined M_system_windows
auto wmain (
	int       argc,
	wchar_t * argv[]
) -> int
#endif
#if defined M_system_linux || defined M_system_macos || defined M_system_android || defined M_system_ios
auto main (
	int    argc,
	char * argv[]
) -> int
#endif
{
	auto args = [&] {
		auto it = std::vector<std::string>{};
		auto raw_args = std::span{argv, static_cast<std::size_t>(argc)};
		it.reserve(raw_args.size());
		for (auto & raw_arg : raw_args) {
			#if defined M_system_windows
			auto raw_arg_8 = TwinKleS::Shell::utf16_to_utf8(std::u16string_view{reinterpret_cast<char16_t const *>(raw_arg)});
			it.emplace_back(std::move(reinterpret_cast<std::string &>(raw_arg_8)));
			#endif
			#if defined M_system_linux || defined M_system_macos || defined M_system_android || defined M_system_ios
			it.emplace_back(raw_arg);
			#endif
		}
		return it;
	}();
	auto exception_message = std::optional<std::string>{};
	std::cout << "TwinKleS.ToolKit.Shell " << std::size_t{M_version} << "\n" << std::flush;
	assert_condition(args.size() >= 4);
	auto library_file = args[1];
	auto script = args[2];
	auto script_is_path = TwinKleS::Shell::string_to_boolean(args[3]);
	auto argument = std::vector<std::string>{args.begin() + 4, args.end()};
	auto library = TwinKleS::Shell::StaticLibrary{nullptr};
	std::cout << "TwinKleS.ToolKit.Core " << library.wrapped_version() << "\n" << std::flush;
	auto host = TwinKleS::Shell::CLIHost{nullptr};
	auto result = TwinKleS::Shell::launch(host, library, script, script_is_path, argument);
	if (result) {
		exception_message.emplace(result.value());
	}
	if (exception_message) {
		std::cout << "\n" << std::flush;
		std::cout << "Exception :\n" << exception_message.value() << "\n" << std::flush;
		std::cout << "\n" << std::flush;
		return 1;
	}
	return 0;
}
