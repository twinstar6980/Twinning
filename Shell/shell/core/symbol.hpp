#pragma once

#include "shell/common.hpp"
#include "shell/core/interface.hpp"

namespace TwinStar::Shell::Core {

	#pragma region type

	struct SymbolTable {
		decltype(Interface::version) * version;
		decltype(Interface::execute) * execute;
		decltype(Interface::prepare) * prepare;
	};

	struct SymbolNameTable {
		#if defined M_system_windows
		inline static constexpr std::string_view version = "?version@Interface@Core@TwinStar@@YAPEBUSize@123@XZ"sv;
		inline static constexpr std::string_view execute = "?execute@Interface@Core@TwinStar@@YAPEBUString@123@PEBUCallback@123@PEBU4123@PEBUStringList@123@@Z"sv;
		inline static constexpr std::string_view prepare = "?prepare@Interface@Core@TwinStar@@YAPEBUString@123@XZ"sv;
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		inline static constexpr std::string_view version = "_ZN8TwinStar4Core9Interface7versionEv"sv;
		inline static constexpr std::string_view execute = "_ZN8TwinStar4Core9Interface7executeEPKNS1_8CallbackEPKNS1_6StringEPKNS1_10StringListE"sv;
		inline static constexpr std::string_view prepare = "_ZN8TwinStar4Core9Interface7prepareEv"sv;
		#endif
	};

	#pragma endregion

}
