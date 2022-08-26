#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/pak/version.hpp"

namespace TwinKleS::Core::Tool::PopCap::PAK::Structure {

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
	struct ResourceInformation<t_version> {
		StringBlock8 path;
		IntegerU32   size;
		IntegerU64   time;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}, {true}))
	struct ResourceInformation<t_version> {
		StringBlock8 path;
		IntegerU32   size;
		IntegerU32   size_original;
		IntegerU64   time;
	};

	// ----------------

	template <auto t_version>
	struct Information;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}, {}))
	struct Information<t_version> {
		List<ResourceInformation<t_version>> resource_information;
	};

	#pragma endregion

	#pragma region misc

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

namespace TwinKleS::Core {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAK::check_version(t_version, {0}, {false}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::PAK::Structure::ResourceInformation<t_version>),
		M_wrap(path, size, time),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAK::check_version(t_version, {0}, {true}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::PopCap::PAK::Structure::ResourceInformation<t_version>),
		M_wrap(path, size, size_original, time),
	);

}
