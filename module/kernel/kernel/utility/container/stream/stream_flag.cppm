module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.stream.stream_flag;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel {

	#pragma region type

	M_enumeration_specific(
		M_wrap(StreamFlag),
		M_wrap(ZIntegerU8),
		M_wrap(
			none,
		),
		M_wrap(
			none = 0,
		),
	);

	#pragma endregion

}
