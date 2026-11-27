module;

#include "kernel/common.hpp"

export module twinning.kernel.interface.data;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Interface {

	#pragma region type

	struct Message {

		ZPointer<ZByte> data{nullptr};

		ZSize size{0_szz};

	};

	struct Executor {

		ZPointer<Void (ZPointer<Executor> self, ZPointer<Executor> callback, ZPointer<Message> argument, ZPointer<Message> result, ZPointer<Message> exception)> invoke{nullptr};

		ZPointer<Void (ZPointer<Executor> self, ZPointer<Executor> callback, ZPointer<Message> argument, ZPointer<Message> result, ZPointer<Message> exception)> clear{nullptr};

	};

	#pragma endregion

}
