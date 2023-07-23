#pragma once

#include "shell/common.hpp"
#include "shell/bridge/interface.hpp"

namespace TwinStar::Shell::Bridge {

	#pragma region type

	struct SymbolTable {
		decltype(Interface::version) * version;
		decltype(Interface::execute) * execute;
		decltype(Interface::prepare) * prepare;
	};

	struct SymbolNameTable {
		#if defined M_system_windows
		inline static constexpr std::string_view version = "?version@Interface@Kernel@TwinStar@@YAPEAUString@123@PEAPEAUSize@123@@Z"sv;
		inline static constexpr std::string_view execute = "?execute@Interface@Kernel@TwinStar@@YAPEAUString@123@PEAPEAUCallback@123@PEAPEAU4123@PEAPEAUStringList@123@1@Z"sv;
		inline static constexpr std::string_view prepare = "?prepare@Interface@Kernel@TwinStar@@YAPEAUString@123@XZ"sv;
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		inline static constexpr std::string_view version = "_ZN8TwinStar6Kernel9Interface7versionEPPNS1_4SizeE"sv;
		inline static constexpr std::string_view execute = "_ZN8TwinStar6Kernel9Interface7executeEPPNS1_8CallbackEPPNS1_6StringEPPNS1_10StringListES7_"sv;
		inline static constexpr std::string_view prepare = "_ZN8TwinStar6Kernel9Interface7prepareEv"sv;
		#endif
	};

	#pragma endregion

}
