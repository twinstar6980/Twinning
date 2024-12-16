module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_bundle.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.definition;
import twinning.kernel.tool.popcap.resource_stream_bundle.manifest;
import twinning.kernel.tool.popcap.resource_stream_bundle.structure;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundle {

	template <auto version> requires (check_version(version, {}, {}))
	struct Common {

		using Definition = Definition<version>;

		using Manifest = Manifest<version>;

		// ----------------

		inline static auto const k_suffix_of_composite_shell_upper = CStringView{"_COMPOSITESHELL"_sv};

		inline static auto const k_suffix_of_composite_shell = CStringView{"_CompositeShell"_sv};

		inline static auto const k_suffix_of_automation_pool = CStringView{"_AutoPool"_sv};

	};

}
