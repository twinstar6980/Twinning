module;

#include "shell/common.hpp"

export module twinning.shell.bridge.data;

export namespace Twinning::Shell::Bridge {

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
