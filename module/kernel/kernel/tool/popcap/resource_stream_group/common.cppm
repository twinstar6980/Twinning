module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_group.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_group.version;
import twinning.kernel.tool.popcap.resource_stream_group.definition;
import twinning.kernel.tool.popcap.resource_stream_group.structure;

export namespace Twinning::Kernel::Tool::Popcap::ResourceStreamGroup {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Common {

		using Definition = Definition<t_version>;

	};

}
