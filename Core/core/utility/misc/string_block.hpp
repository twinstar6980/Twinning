#pragma once

#include "core/utility/string/string.hpp"

namespace TwinKleS::Core {

	#pragma region type

	template <typename TLength> requires
		CategoryConstraint<IsPureInstance<TLength>>
		&& (IsIntegerWrapper<TLength>)
	struct StringBlock {

		using Length = TLength;

		// ----------------

		String value;

	};

	#pragma endregion

	#pragma region alias

	using StringBlock8 = StringBlock<IntegerU8>;

	using StringBlock16 = StringBlock<IntegerU16>;

	using StringBlock32 = StringBlock<IntegerU32>;

	#pragma endregion

}
