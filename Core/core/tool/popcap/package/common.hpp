#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/package/version.hpp"
#include "core/tool/popcap/package/definition.hpp"
#include "core/tool/popcap/package/structure.hpp"

namespace TwinStar::Core::Tool::PopCap::Package {

	template <auto version> requires (check_version(version, {}, {}))
	struct Common {

		using Definition = Definition<version>;

	};

}
