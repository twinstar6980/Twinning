module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.reflection_object_notation.version;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::PopCap::ReflectionObjectNotation {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Integer) number,
			(Boolean) native_string_encoding_use_utf8,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.number = 1_i, .native_string_encoding_use_utf8 = k_false},
		Version{.number = 1_i, .native_string_encoding_use_utf8 = k_true}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                               it,
		std::initializer_list<Integer::Value> const & number,
		std::initializer_list<Boolean::Value> const & native_string_encoding_use_utf8
	) -> ZBoolean {
		auto result = true;
		result &= VersionPackage::has(it);
		result &= (number.size() < 1 || *(number.begin() + 0) <= it.number.value) && (number.size() < 2 || *(number.begin() + 1) > it.number.value);
		result &= (native_string_encoding_use_utf8.size() == 0 || Range::has(native_string_encoding_use_utf8, it.native_string_encoding_use_utf8.value));
		return result;
	}

}
