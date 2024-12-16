module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.marmalade.dzip.common;
import twinning.kernel.utility;
import twinning.kernel.tool.marmalade.dzip.version;
import twinning.kernel.tool.marmalade.dzip.definition;
import twinning.kernel.tool.marmalade.dzip.structure;

export namespace Twinning::Kernel::Tool::Marmalade::DZip {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

	};

}
