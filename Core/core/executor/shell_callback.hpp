#pragma once

#include "core/utility/utility.hpp"

namespace TwinKleS::Core::Executor {

	#pragma region type

	using ShellCallback = AsGlobalFunction<List<String> const &, List<String> const &>;

	#pragma endregion

}
