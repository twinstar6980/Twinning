#pragma once

#include "shell/common.hpp"
#include "shell/bridge/data.hpp"

namespace Twinning::Shell::Bridge {

	#pragma region type

	struct Service {

		Executor * executor{nullptr};

		std::add_pointer_t<void  ()> initialize{nullptr};

		std::add_pointer_t<void  ()> finalize{nullptr};

	};

	#pragma endregion

}
