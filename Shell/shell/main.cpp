//

#include "./loader.hpp"
#include "./callback.hpp"
#include <optional>

#if defined M_system_windows
#include "Windows.h"
#endif

#pragma region version

inline constexpr auto k_version = std::size_t{11};

#pragma endregion

#pragma region config

#if defined M_system_windows
inline constexpr auto k_library_file = std::string_view{"C:\\Program Files\\TwinKleS\\ToolKit\\core.dll"};
inline constexpr auto k_script_file = std::string_view{"C:\\Program Files\\TwinKleS\\ToolKit\\script\\main.js"};
#endif
#if defined M_system_linux
inline constexpr auto k_library_file = std::string_view{"/opt/TwinKleS/ToolKit/core.so"};
inline constexpr auto k_script_file = std::string_view{"/opt/TwinKleS/ToolKit/script/main.js"};
#endif
#if defined M_system_macos
inline constexpr auto k_library_file = std::string_view{"/opt/TwinKleS/ToolKit/core.dylib"};
inline constexpr auto k_script_file = std::string_view{"/opt/TwinKleS/ToolKit/script/main.js"};
#endif
#if defined M_system_android
inline constexpr auto k_library_file = std::string_view{"/opt/TwinKleS/ToolKit/core.so"};
inline constexpr auto k_script_file = std::string_view{"/opt/TwinKleS/ToolKit/script/main.js"};
#endif

#pragma endregion

#pragma region main

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
	assert_condition(argc >= (1));
	auto args = std::span{argv + 1, static_cast<std::size_t>(argc - 1)};
	#if defined M_system_windows
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	#endif
	std::cout << "TwinKleS.ToolKit.Shell " << k_version << std::endl;
	auto argument = [&] {
		auto it = std::vector<std::string>{};
		it.reserve(args.size());
		for (auto & arg : args) {
			#if defined M_system_windows
			auto arg_8 = TwinKleS::Shell::utf16_to_utf8(std::u16string_view{reinterpret_cast<char16_t const *>(arg)});
			it.emplace_back(std::move(reinterpret_cast<std::string &>(arg_8)));
			#endif
			#if defined M_system_linux || defined M_system_macos || defined M_system_android
			it.emplace_back(arg);
			#endif
		}
		return it;
	}();
	auto result = std::optional<std::string>{};
	try {
		auto library = TwinKleS::Shell::Loader::Library{k_library_file};
		std::cout << "TwinKleS.ToolKit.Core " << library.version() << std::endl;
		result = library.execute(
			std::string{k_script_file},
			true,
			argument,
			&TwinKleS::Shell::Callback::CLI::shell_callback
		);
	} catch (std::exception & exception) {
		result.emplace(exception.what());
	} catch (...) {
		result.emplace("unknown exception");
	}
	if (result) {
		std::cout << "Exception :\n" << result.value() << std::endl;
		#if defined M_system_windows
		std::system("pause");
		#endif
		#if defined M_system_linux || defined M_system_macos || defined M_system_android
		std::system("echo pause... && read _");
		#endif
		return 1;
	}
	return 0;
}

#pragma endregion
