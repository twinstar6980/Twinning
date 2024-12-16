module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.render_effect.version;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::PopCap::RenderEffect {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Integer) number,
			(Integer) variant,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.number = 1_i, .variant = 1_i},
		Version{.number = 1_i, .variant = 2_i},
		Version{.number = 1_i, .variant = 3_i}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                               it,
		std::initializer_list<Integer::Value> const & number,
		std::initializer_list<Integer::Value> const & variant
	) -> ZBoolean {
		auto result = true;
		result &= VersionPackage::has(it);
		result &= (number.size() < 1 || *(number.begin() + 0) <= it.number.value) && (number.size() < 2 || *(number.begin() + 1) > it.number.value);
		result &= (variant.size() < 1 || *(variant.begin() + 0) <= it.variant.value) && (variant.size() < 2 || *(variant.begin() + 1) > it.variant.value);
		return result;
	}

}
