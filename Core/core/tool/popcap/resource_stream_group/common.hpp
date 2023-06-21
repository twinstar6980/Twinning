#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/resource_stream_group/version.hpp"
#include "core/tool/popcap/resource_stream_group/definition.hpp"
#include "core/tool/popcap/resource_stream_group/structure.hpp"

namespace TwinStar::Core::Tool::PopCap::ResourceStreamGroup {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

	};

}
