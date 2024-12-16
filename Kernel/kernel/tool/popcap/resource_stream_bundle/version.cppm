module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle.version;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundle {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Integer) number,
			(Integer) extended_texture_information_for_pvz2_cn,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.number = 1_i, .extended_texture_information_for_pvz2_cn = 0_i},
		Version{.number = 3_i, .extended_texture_information_for_pvz2_cn = 0_i},
		Version{.number = 4_i, .extended_texture_information_for_pvz2_cn = 0_i},
		Version{.number = 4_i, .extended_texture_information_for_pvz2_cn = 1_i},
		Version{.number = 4_i, .extended_texture_information_for_pvz2_cn = 2_i},
		Version{.number = 4_i, .extended_texture_information_for_pvz2_cn = 3_i}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                               it,
		std::initializer_list<Integer::Value> const & number,
		std::initializer_list<Integer::Value> const & extended_texture_information_for_pvz2_cn
	) -> ZBoolean {
		auto result = true;
		result &= VersionPackage::has(it);
		result &= (number.size() < 1 || *(number.begin() + 0) <= it.number.value) && (number.size() < 2 || *(number.begin() + 1) > it.number.value);
		result &= (extended_texture_information_for_pvz2_cn.size() < 1 || *(extended_texture_information_for_pvz2_cn.begin() + 0) <= it.extended_texture_information_for_pvz2_cn.value) && (extended_texture_information_for_pvz2_cn.size() < 2 || *(extended_texture_information_for_pvz2_cn.begin() + 1) > it.extended_texture_information_for_pvz2_cn.value);
		return result;
	}

}
