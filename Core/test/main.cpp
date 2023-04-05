//

#include "shell/common.hpp"
#include "shell/utility/interaction.hpp"
#include "shell/utility/miscellaneous.hpp"
#include "shell/core/static_library.hpp"
#include "shell/host/cli_host.hpp"
#include "shell/host/launcher.hpp"

#if defined M_vld
#include "vld.h"
#endif

#pragma region main

M_declare_native_main_function {
	#if defined M_build_release
	try {
	#endif
		auto args = TwinStar::Shell::parse_raw_native_string(std::span{argv, static_cast<std::size_t>(argc)});
		assert_test(args.size() >= 3);
		auto core_path = args[1];
		auto script = args[2];
		auto argument = std::vector<std::string>{args.begin() + 3, args.end()};
		auto core = TwinStar::Shell::Core::StaticLibrary{nullptr};
		auto host = TwinStar::Shell::CLIHost{nullptr};
		auto result = TwinStar::Shell::Launcher::launch(host, core, script, argument);
		TwinStar::Shell::Interaction::error("SUCCEEDED");
		TwinStar::Shell::Interaction::error("\n");
		TwinStar::Shell::Interaction::error(result);
		TwinStar::Shell::Interaction::error("\n");
		return 0;
	#if defined M_build_release
	} catch (...) {
		auto exception = TwinStar::Shell::parse_current_exception();
		TwinStar::Shell::Interaction::error("FAILED");
		TwinStar::Shell::Interaction::error("\n");
		TwinStar::Shell::Interaction::error(exception);
		TwinStar::Shell::Interaction::error("\n");
		return 1;
	}
	#endif
}

#pragma endregion
