module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.package.structure;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.package.version;

export namespace Twinning::Kernel::Tool::PopCap::Package::Structure {

	#pragma region magic identifier

	using MagicIdentifier = IntegerU32;

	inline constexpr auto k_magic_identifier = MagicIdentifier{0xBAC04AC0_iu32};

	#pragma endregion

	#pragma region version number

	using VersionNumber = IntegerU32;

	#pragma endregion

	#pragma region information

	template <auto t_version>
	struct ResourceInformation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}, {false}))
	M_record_of_data(
		M_wrap(ResourceInformation<t_version>),
		M_wrap(
			(StringBlock8) path,
			(IntegerU32) size,
			(IntegerU64) time,
		),
	);

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}, {true}))
	M_record_of_data(
		M_wrap(ResourceInformation<t_version>),
		M_wrap(
			(StringBlock8) path,
			(IntegerU32) size,
			(IntegerU32) size_original,
			(IntegerU64) time,
		),
	);

	// ----------------

	template <auto t_version>
	struct Information;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}, {}))
	M_record_of_data(
		M_wrap(Information<t_version>),
		M_wrap(
			(List<ResourceInformation<t_version>>) resource_information,
		),
	);

	#pragma endregion

	#pragma region miscellaneous

	template <auto t_version>
	struct ResourceInformationListStateFlag;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}, {}))
	struct ResourceInformationListStateFlag<t_version> {

		inline static constexpr auto next = IntegerU8{0x00_iu8};

		inline static constexpr auto done = IntegerU8{0x80_iu8};

	};

	#pragma endregion

}
