module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.package.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.package.version;
import twinning.kernel.tool.popcap.package.definition;
import twinning.kernel.tool.popcap.package.structure;

export namespace Twinning::Kernel::Tool::Popcap::Package {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Common {

		using Definition = Definition<t_version>;

	};

}
