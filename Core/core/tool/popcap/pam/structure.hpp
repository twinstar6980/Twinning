#pragma once

#include "core/utility/utility.hpp"

namespace TwinKleS::Core::Tool::PopCap::PAM::Structure {

	#pragma region magic identifier

	using MagicIdentifier = IntegerU32;

	inline constexpr auto k_magic_identifier = MagicIdentifier{0xBAF01954_iu32};

	#pragma endregion

	#pragma region version number

	using VersionNumber = IntegerU32;

	#pragma endregion

}
