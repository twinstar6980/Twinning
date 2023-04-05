#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Executor {

	#pragma region type

	using Callback = AsGlobalFunction<String *, List<String> * *, List<String> * *>;

	#pragma endregion

}
