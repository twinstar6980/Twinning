module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.zlib.version;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::PopCap::ZLib {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Boolean) variant_64,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.variant_64 = k_false},
		Version{.variant_64 = k_true}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                               it,
		std::initializer_list<Boolean::Value> const & variant_64
	) -> ZBoolean {
		auto result = true;
		result &= VersionPackage::has(it);
		result &= (variant_64.size() == 0 || Range::has(variant_64, it.variant_64.value));
		return result;
	}

}
