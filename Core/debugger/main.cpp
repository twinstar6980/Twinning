//

#include <span>
#include <iostream>

#if defined M_system_windows
#include <codecvt>
#include "Windows.h"
#endif

#if defined M_vld
#include "vld.h"
#endif

#include "./shell/callback.hpp"

// ----------------

namespace Core = TwinKleS::Core::Interface;

namespace Shell = TwinKleS::Shell;

// ----------------

#if defined M_system_windows
inline auto convert_string (
	wchar_t * const & source
) -> std::string {
	auto source_view = std::wstring_view{source};
	return std::wstring_convert<std::codecvt_utf8_utf16<std::wstring_view::value_type>, std::wstring_view::value_type>{}.to_bytes(source_view.data(), source_view.data() + source_view.size());
}
#endif
#if defined M_system_linux || defined M_system_macos || defined M_system_android
inline auto convert_string (
	char * const & source
) -> std::string {
	return std::string{source};
}
#endif

// ----------------

#if defined M_system_windows
auto wmain (
	int       argc,
	wchar_t * argv[]
) -> int
#endif
#if defined M_system_linux || defined M_system_macos || defined M_system_android
auto main (
	int    argc,
	char * argv[]
) -> int
#endif
{
	assert_condition(argc >= (1 + 1));
	auto args = std::span{argv + 1, static_cast<std::size_t>(argc - 1)};
	#if defined M_system_windows
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	#endif
	std::cout << "TwinKleS.ToolKit.Core " << (*Core::version()).value << std::endl;
	auto raw_script = convert_string(args[0]);
	auto argument = [&] {
		auto it = std::vector<std::string>{};
		it.reserve(args.size() - 1);
		for (auto & arg : args.subspan(1)) {
			it.emplace_back(convert_string(arg));
		}
		return it;
	}();
	auto script_is_file = !raw_script.empty() && raw_script[0] == '@';
	auto script = !script_is_file ? raw_script : raw_script.substr(1);
	auto script_s = Shell::to_string_structure(script);
	auto script_is_path_s = Shell::to_boolean_structure(script_is_file);
	auto argument_s = Shell::alloc_string_list_structure(argument);
	auto result_s = Core::execute(
		script_s,
		script_is_path_s,
		argument_s,
		&Shell::Callback::CLI::shell_callback
	);
	Shell::free_string_list_structure(argument_s);
	if (result_s) {
		auto result = Shell::from_string_structure(*result_s);
		std::cout << "Exception :\n" << result << std::endl;
	}
	return 0;
}
