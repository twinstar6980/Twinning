#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/marmalade/dzip/version.hpp"
#include "core/tool/marmalade/dzip/definition.hpp"
#include "core/tool/marmalade/dzip/structure.hpp"

namespace TwinStar::Core::Tool::Marmalade::DZip {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

	};

}
