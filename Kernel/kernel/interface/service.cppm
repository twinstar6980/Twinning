module;

#include "kernel/common.hpp"

export module twinning.kernel.interface.service;
import twinning.kernel.utility;
import twinning.kernel.interface.data;

export namespace Twinning::Kernel::Interface {

	#pragma region type

	struct Service {

		Executor * executor{nullptr};

		std::add_pointer_t<void  ()> initialize{nullptr};

		std::add_pointer_t<void  ()> finalize{nullptr};

	};

	#pragma endregion

}
