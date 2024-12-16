module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_group.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_group.version;
import twinning.kernel.tool.popcap.resource_stream_group.definition;
import twinning.kernel.tool.popcap.resource_stream_group.structure;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamGroup {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

	};

}
