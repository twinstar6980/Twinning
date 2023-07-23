//

#include "shell/common.hpp"
#include "shell/utility/interaction.hpp"
#include "shell/utility/miscellaneous.hpp"
#include "shell/bridge/dynamic_library.hpp"
#include "shell/bridge/cli_host.hpp"
#include "shell/bridge/launcher.hpp"

#if defined M_vld
#include "vld.h"
#endif

#pragma region main

M_declare_native_main_function {
	try {
		auto args = TwinStar::Shell::parse_raw_native_string(std::span{argv, static_cast<std::size_t>(argc)});
		assert_test(args.size() >= 3);
		auto kernel = args[1];
		auto script = args[2];
		auto argument = std::vector<std::string>{args.begin() + 3, args.end()};
		auto library = TwinStar::Shell::Bridge::DynamicLibrary{nullptr, kernel};
		auto host = TwinStar::Shell::Bridge::CLIHost{nullptr};
		auto result = TwinStar::Shell::Bridge::Launcher::launch(host, library, script, argument);
		TwinStar::Shell::Interaction::error("SUCCEEDED");
		TwinStar::Shell::Interaction::error("\n");
		TwinStar::Shell::Interaction::error(result);
		TwinStar::Shell::Interaction::error("\n");
		return 0;
	} catch (...) {
		auto exception = TwinStar::Shell::parse_current_exception();
		TwinStar::Shell::Interaction::error("FAILED");
		TwinStar::Shell::Interaction::error("\n");
		TwinStar::Shell::Interaction::error(exception);
		TwinStar::Shell::Interaction::error("\n");
		return 1;
	}
}

#pragma endregion
