module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.compression.brotli.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Data::Compression::Brotli {

	M_enumeration(
		M_wrap(Container),
		M_wrap(
			none,
		),
	);

	M_enumeration(
		M_wrap(Mode),
		M_wrap(
			generic,
			text,
			font,
		),
	);

	// ----------------

	struct Common {

	};

}
