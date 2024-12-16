//

#if defined M_vld
#include "vld.h"
#endif

#include "kernel/interface/interface.hpp"
#include "shell/common.hpp"

import twinning.shell.utility.exception;
import twinning.shell.utility.library;
import twinning.shell.utility.interaction;
import twinning.shell.utility.miscellaneous;
import twinning.shell.bridge.library;
import twinning.shell.bridge.launcher;
import twinning.shell.main_console_bridge_client;

#pragma region main

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"

M_declare_native_main_function {
	#if defined M_build_release
	try
	#endif
	{
		auto args = Twinning::Shell::parse_native_main_function_argument(argc, argv);
		assert_test(args.size() >= 3);
		auto kernel = args[1];
		auto script = args[2];
		auto argument = std::vector<std::string>{args.begin() + 3, args.end()};
		auto library = Twinning::Shell::Bridge::Library{&Twinning::Kernel::Interface::service};
		auto client = Twinning::Shell::MainConsoleBridgeClient{};
		auto result = Twinning::Shell::Bridge::Launcher::launch(client, library, script, argument);
		Twinning::Shell::Interaction::output_text("SUCCEEDED");
		Twinning::Shell::Interaction::output_text("\n");
		for (auto & result_item : result) {
			Twinning::Shell::Interaction::output_text(result_item);
			Twinning::Shell::Interaction::output_text("\n");
		}
		return 0;
	}
	#if defined M_build_release
	catch (...) {
		auto exception = Twinning::Shell::parse_current_exception();
		Twinning::Shell::Interaction::output_text("FAILED");
		Twinning::Shell::Interaction::output_text("\n");
		Twinning::Shell::Interaction::output_text(exception);
		Twinning::Shell::Interaction::output_text("\n");
		return 1;
	}
	#endif
}

#pragma clang diagnostic pop

#pragma endregion
