#pragma once

#include "core/utility/utility.hpp"

namespace TwinKleS::Core::Tool::PopCap::ZLib::Structure {

	#pragma region magic identifier

	using MagicIdentifier = IntegerU32;

	inline constexpr auto k_magic_identifier = MagicIdentifier{0xDEADFED4_iu32};

	#pragma endregion

}
