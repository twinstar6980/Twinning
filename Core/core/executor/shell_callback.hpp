#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Executor {

	#pragma region type

	using ShellCallback = AsGlobalFunction<List<String> const &, List<String> const &>;

	#pragma endregion

}
