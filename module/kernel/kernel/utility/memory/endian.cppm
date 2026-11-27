module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.memory.endian;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel::Memory {

	#pragma region type

	M_enumeration(
		M_wrap(Endian),
		M_wrap(
			little,
			big,
		),
	);

	#pragma endregion

}
