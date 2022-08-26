#pragma once

#include "core/utility/utility.hpp"

namespace TwinKleS::Core::Tool::PopCap::Reanim::Structure {

	#pragma region magic identifier

	using MagicIdentifier = IntegerU32;

	inline constexpr auto k_magic_identifier_desktop_32 = MagicIdentifier{0xB393B4C0_iu32};

	inline constexpr auto k_magic_identifier_phone_32 = MagicIdentifier{0xFF2565B5_iu32};

	inline constexpr auto k_magic_identifier_phone_64 = MagicIdentifier{0xC046E570_iu32};

	#pragma endregion

}
