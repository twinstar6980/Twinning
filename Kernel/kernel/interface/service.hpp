#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/interface/data.hpp"

namespace Twinning::Kernel::Interface {

	#pragma region type

	struct Service {

		Executor * executor{nullptr};

		std::add_pointer_t<void  ()> initialize{nullptr};

		std::add_pointer_t<void  ()> finalize{nullptr};

	};

	#pragma endregion

}
