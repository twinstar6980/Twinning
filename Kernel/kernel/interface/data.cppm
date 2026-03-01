module;

#include "kernel/common.hpp"

export module twinning.kernel.interface.data;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Interface {

	#pragma region type

	struct Message {

		std::uint8_t * data{nullptr};

		std::size_t size{0};

	};

	struct Executor {

		std::add_pointer_t<void  (Executor * self, Executor * callback, Message * argument, Message * result, Message * exception)> invoke{nullptr};

		std::add_pointer_t<void  (Executor * self, Executor * callback, Message * argument, Message * result, Message * exception)> clear{nullptr};

	};

	#pragma endregion

}
