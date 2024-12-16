module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.u_texture.version;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::PopCap::UTexture {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Boolean) compress_texture_data,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.compress_texture_data = k_false},
		Version{.compress_texture_data = k_true}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                               it,
		std::initializer_list<Boolean::Value> const & compress_texture_data
	) -> ZBoolean {
		auto result = true;
		result &= VersionPackage::has(it);
		result &= (compress_texture_data.size() == 0 || Range::has(compress_texture_data, it.compress_texture_data.value));
		return result;
	}

}
