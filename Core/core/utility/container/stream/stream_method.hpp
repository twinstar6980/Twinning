#pragma once

#include "core/utility/container/list/list_view.hpp"

namespace TwinKleS::Core {

	#pragma region type

	enum class StreamMethod : ZIntegerU8 {
		io,
		i,
		o,
	};

	#pragma endregion

}

namespace TwinKleS::Core::Trait::Reflection {

	M_reflection_enum(
		M_wrap(StreamMethod),
		M_wrap(StreamMethod),
		M_wrap(io, i, o),
	);

}
