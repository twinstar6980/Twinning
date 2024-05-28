#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/package/version.hpp"
#include "kernel/tool/popcap/package/definition.hpp"
#include "kernel/tool/popcap/package/structure.hpp"

namespace Twinning::Kernel::Tool::PopCap::Package {

	template <auto version> requires (check_version(version, {}, {}))
	struct Common {

		using Definition = Definition<version>;

	};

}
