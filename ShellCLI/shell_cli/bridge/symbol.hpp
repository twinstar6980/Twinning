#pragma once

#include "shell_cli/common.hpp"
#include "shell_cli/bridge/interface.hpp"

namespace TwinStar::ShellCLI::Bridge {

	#pragma region type

	struct SymbolTable {
		decltype(Interface::execute) * execute;
	};

	struct SymbolNameTable {
		#if defined M_system_windows
		inline static constexpr std::string_view execute = "?execute@Interface@Kernel@TwinStar@@YAPEAUString@123@PEAPEAUCallback@123@PEAPEAU4123@PEAPEAUStringList@123@1@Z"sv;
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		inline static constexpr std::string_view execute = "_ZN8TwinStar6Kernel9Interface7executeEPPNS1_8CallbackEPPNS1_6StringEPPNS1_10StringListES7_"sv;
		#endif
	};

	#pragma endregion

}
