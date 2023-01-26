//

#include "shell/library/static_library.hpp"
#include "shell/host/cli_host.hpp"
#include "shell/launch.hpp"
#include <span>

#if defined M_vld
#include "vld.h"
#endif

#pragma region main

#if defined M_system_windows
auto wmain (
	int       argc,
	wchar_t * argv[]
) -> int
#endif
#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
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
			auto raw_arg_8 = TwinStar::Shell::utf16_to_utf8(std::u16string_view{reinterpret_cast<char16_t const *>(raw_arg)});
			it.emplace_back(std::move(reinterpret_cast<std::string &>(raw_arg_8)));
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			it.emplace_back(raw_arg);
			#endif
		}
		return it;
	}();
	auto exception_message = std::optional<std::string>{};
	{
		assert_condition(args.size() >= 3);
		auto library_file = args[1];
		auto script = args[2];
		auto argument = std::vector<std::string>{args.begin() + 3, args.end()};
		auto library = TwinStar::Shell::StaticLibrary{nullptr};
		auto host = TwinStar::Shell::CLIHost{nullptr};
		auto result = TwinStar::Shell::launch(host, library, script, argument);
		if (result) {
			exception_message.emplace(result.value());
		}
	}
	if (exception_message) {
		TwinStar::Shell::output("\n");
		TwinStar::Shell::output("Exception :\n");
		TwinStar::Shell::output(exception_message.value());
		TwinStar::Shell::output("\n");
		TwinStar::Shell::output("\n");
		TwinStar::Shell::output("Press <ENTER> to exit ... ");
		TwinStar::Shell::input();
		return 1;
	}
	return 0;
}

#pragma endregion
