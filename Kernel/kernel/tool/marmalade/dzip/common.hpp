#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/marmalade/dzip/version.hpp"
#include "kernel/tool/marmalade/dzip/definition.hpp"
#include "kernel/tool/marmalade/dzip/structure.hpp"

namespace TwinStar::Kernel::Tool::Marmalade::DZip {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

	};

}
