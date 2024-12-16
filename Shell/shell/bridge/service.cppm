module;

#include "shell/common.hpp"

export module twinning.shell.bridge.service;
import twinning.shell.bridge.data;

export namespace Twinning::Shell::Bridge {

	#pragma region type

	struct Service {

		Executor * executor{nullptr};

		std::add_pointer_t<void  ()> initialize{nullptr};

		std::add_pointer_t<void  ()> finalize{nullptr};

	};

	#pragma endregion

}
