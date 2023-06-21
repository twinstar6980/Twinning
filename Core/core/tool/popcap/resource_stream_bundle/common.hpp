#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/resource_stream_bundle/version.hpp"
#include "core/tool/popcap/resource_stream_bundle/definition.hpp"
#include "core/tool/popcap/resource_stream_bundle/description.hpp"
#include "core/tool/popcap/resource_stream_bundle/structure.hpp"
#include "core/tool/popcap/resource_stream_bundle/shared.hpp"

namespace TwinStar::Core::Tool::PopCap::ResourceStreamBundle {

	template <auto version> requires (check_version(version, {}, {}))
	struct Common {

		using Definition = Definition<version>;

		using Description = Description<version>;

		// ----------------

		inline static auto const k_suffix_of_composite_shell_upper = CStringView{"_COMPOSITESHELL"_sv};

		inline static auto const k_suffix_of_composite_shell = CStringView{"_CompositeShell"_sv};

		inline static auto const k_suffix_of_automation_pool = CStringView{"_AutoPool"_sv};

	};

}
