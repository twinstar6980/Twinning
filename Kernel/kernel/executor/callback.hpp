#pragma once

#include "kernel/utility/utility.hpp"

namespace TwinStar::Kernel::Executor {

	#pragma region type

	using Callback = AsGlobalFunction<String *, List<String> * *, List<String> * *>;

	#pragma endregion

}
