//

#include "shell_cli/common.hpp"
#include "shell_cli/utility/interaction.hpp"
#include "shell_cli/utility/miscellaneous.hpp"
#include "shell_cli/bridge/dynamic_library.hpp"
#include "shell_cli/bridge/cli_host.hpp"
#include "shell_cli/bridge/launcher.hpp"

#if defined M_vld
#include "vld.h"
#endif

#pragma region main

M_declare_native_main_function {
	try {
		auto args = TwinStar::ShellCLI::parse_raw_native_string(std::span{argv, static_cast<std::size_t>(argc)});
		assert_test(args.size() >= 3);
		auto kernel = args[1];
		auto script = args[2];
		auto argument = std::vector<std::string>{args.begin() + 3, args.end()};
		auto library = TwinStar::ShellCLI::Bridge::DynamicLibrary{nullptr, kernel};
		auto host = TwinStar::ShellCLI::Bridge::CLIHost{nullptr};
		auto result = TwinStar::ShellCLI::Bridge::Launcher::launch(host, library, script, argument);
		TwinStar::ShellCLI::Interaction::error("SUCCEEDED");
		TwinStar::ShellCLI::Interaction::error("\n");
		TwinStar::ShellCLI::Interaction::error(result);
		TwinStar::ShellCLI::Interaction::error("\n");
		return 0;
	}
	catch (...) {
		auto exception = TwinStar::ShellCLI::parse_current_exception();
		TwinStar::ShellCLI::Interaction::error("FAILED");
		TwinStar::ShellCLI::Interaction::error("\n");
		TwinStar::ShellCLI::Interaction::error(exception);
		TwinStar::ShellCLI::Interaction::error("\n");
		return 1;
	}
}

#pragma endregion
