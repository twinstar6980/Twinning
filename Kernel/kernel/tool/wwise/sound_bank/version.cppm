module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.wwise.sound_bank.version;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Wwise::SoundBank {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Integer) number,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.number = 72_i},
		Version{.number = 88_i},
		Version{.number = 112_i},
		Version{.number = 113_i},
		Version{.number = 118_i},
		Version{.number = 120_i},
		Version{.number = 125_i},
		Version{.number = 128_i},
		Version{.number = 132_i},
		Version{.number = 134_i},
		Version{.number = 135_i},
		Version{.number = 140_i},
		Version{.number = 145_i},
		Version{.number = 150_i}
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
