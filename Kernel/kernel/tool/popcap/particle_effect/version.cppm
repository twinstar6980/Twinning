module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.particle_effect.version;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::PopCap::ParticleEffect {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Integer) number,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.number = 1_i}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                               it,
		std::initializer_list<Integer::Value> const & number
	) -> ZBoolean {
		auto result = true;
		result &= VersionPackage::has(it);
		result &= (number.size() < 1 || *(number.begin() + 0) <= it.number.value) && (number.size() < 2 || *(number.begin() + 1) > it.number.value);
		return result;
	}

}