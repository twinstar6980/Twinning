module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.stream.stream_mode;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.list.list_view;

export namespace Twinning::Kernel {

	#pragma region type

	M_enumeration(
		M_wrap(StreamMode),
		M_wrap(
			io,
			i,
			o,
		),
	);

	#pragma endregion

}
