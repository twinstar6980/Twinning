module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.hashing.crc.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Data::Hashing::Crc {

	M_enumeration(
		M_wrap(Mode),
		M_wrap(
			standard,
			castagnoli,
		),
	);

	// ----------------

	struct Common {

	};

}
