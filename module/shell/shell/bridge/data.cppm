module;

#include "shell/common.hpp"

export module twinning.shell.bridge.data;

export namespace Twinning::Shell::Bridge {

	#pragma region type

	struct Message {

		std::add_pointer_t<std::uint8_t> data{nullptr};

		std::size_t size{0zu};

	};

	struct Executor {

		std::add_pointer_t<void (std::add_pointer_t<Executor> self, std::add_pointer_t<Executor> callback, std::add_pointer_t<Message> argument, std::add_pointer_t<Message> result, std::add_pointer_t<Message> exception)> invoke{nullptr};

		std::add_pointer_t<void (std::add_pointer_t<Executor> self, std::add_pointer_t<Executor> callback, std::add_pointer_t<Message> argument, std::add_pointer_t<Message> result, std::add_pointer_t<Message> exception)> clear{nullptr};

	};

	#pragma endregion

}
