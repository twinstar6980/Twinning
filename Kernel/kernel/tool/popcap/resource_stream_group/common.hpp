#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/resource_stream_group/version.hpp"
#include "kernel/tool/popcap/resource_stream_group/definition.hpp"
#include "kernel/tool/popcap/resource_stream_group/structure.hpp"

namespace Twinning::Kernel::Tool::PopCap::ResourceStreamGroup {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

	};

}
