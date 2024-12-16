module;

#include "shell/common.hpp"

export module twinning.shell.utility.miscellaneous;
import twinning.shell.utility.string;

export namespace Twinning::Shell {

	#pragma region native main function

	inline auto parse_native_main_function_argument (
		#if defined M_system_windows
		int       argc,
		wchar_t * argv[]
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		int    argc,
		char * argv[]
		#endif
	) -> std::vector<std::string> {
		auto source = std::span{argv, static_cast<std::size_t>(argc)};
		auto destination = std::vector<std::string>{};
		destination.reserve(source.size());
		for (auto & source_element : source) {
			#if defined M_system_windows
			auto source_element_8 = utf16_to_utf8(std::u16string_view{reinterpret_cast<char16_t const *>(source_element)});
			destination.emplace_back(std::move(reinterpret_cast<std::string &>(source_element_8)));
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			destination.emplace_back(source_element);
			#endif
		}
		return destination;
	}

	#pragma endregion

}
