module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.character_font_widget_2.version;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::PopCap::CharacterFontWidget2 {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
		),
	);

	using VersionPackage = ValuePackage<
		Version{}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const & it
	) -> ZBoolean {
		auto result = true;
		result &= VersionPackage::has(it);
		return result;
	}

}
