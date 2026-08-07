module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.string_block;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.string;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TLength> requires
		CategoryConstraint<IsPure<TLength>>
		&& (IsVoid<TLength> || IsIntegerBox<TLength>)
	struct StringBlock {

		using Length = TLength;

		// ----------------

		String value{};

	};

	#pragma endregion

	#pragma region alias

	using StringBlockZeroed = StringBlock<Void>;

	using StringBlock8 = StringBlock<IntegerU8>;

	using StringBlock16 = StringBlock<IntegerU16>;

	using StringBlock32 = StringBlock<IntegerU32>;

	#pragma endregion

}
