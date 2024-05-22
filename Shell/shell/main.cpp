//

#include "shell/common.hpp"
#include "shell/utility/interaction.hpp"
#include "shell/utility/miscellaneous.hpp"
#include "shell/bridge/library.hpp"
#include "shell/bridge/launcher.hpp"
#include "shell/main_console_bridge_client.hpp"

#if defined M_vld
#include "vld.h"
#endif

#pragma region main

M_declare_native_main_function {
	#if defined M_build_release
	try
	#endif
	{
		auto args = TwinStar::Shell::parse_native_main_function_argument(argc, argv);
		assert_test(args.size() >= 3);
		auto kernel = args[1];
		auto script = args[2];
		auto argument = std::vector<std::string>{args.begin() + 3, args.end()};
		auto library = TwinStar::Shell::Bridge::Library{kernel};
		auto client = TwinStar::Shell::MainConsoleBridgeClient{};
		auto result = TwinStar::Shell::Bridge::Launcher::launch(client, library, script, argument);
		TwinStar::Shell::Interaction::output_text("SUCCEEDED");
		TwinStar::Shell::Interaction::output_text("\n");
		for (auto & result_item : result) {
			TwinStar::Shell::Interaction::output_text(result_item);
			TwinStar::Shell::Interaction::output_text("\n");
		}
		return 0;
	}
	#if defined M_build_release
	catch (...) {
		auto exception = TwinStar::Shell::parse_current_exception();
		TwinStar::Shell::Interaction::output_text("FAILED");
		TwinStar::Shell::Interaction::output_text("\n");
		TwinStar::Shell::Interaction::output_text(exception);
		TwinStar::Shell::Interaction::output_text("\n");
		return 1;
	}
	#endif
}

#pragma endregion
