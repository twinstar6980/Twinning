module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.record;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.variant.variant;

export namespace Twinning::Kernel {

	#pragma region type

	struct Record {

		inline friend constexpr auto operator == (
			Record const & thix,
			Record const & that
		) -> bool = default;

	};

	// ----------------

	struct DataRecord :
		Record {

		inline friend constexpr auto operator == (
			DataRecord const & thix,
			DataRecord const & that
		) -> bool = default;

	};

	struct ListRecord :
		Record {

		inline friend constexpr auto operator == (
			ListRecord const & thix,
			ListRecord const & that
		) -> bool = default;

	};

	struct MapRecord :
		Record {

		inline friend constexpr auto operator == (
			MapRecord const & thix,
			MapRecord const & that
		) -> bool = default;

	};

	// ----------------

	template <typename ... TValue> requires
		CategoryConstraint<IsPureInstance<TValue ...>>
		&& (sizeof...(TValue) > 0_szz)
	struct ListRecordVariant :
		Variant<TValue ...> {

		inline friend constexpr auto operator == (
			ListRecordVariant const & thix,
			ListRecordVariant const & that
		) -> bool = default;

	};

	template <typename ... TValue> requires
		CategoryConstraint<IsPureInstance<TValue ...>>
		&& (sizeof...(TValue) > 0_szz)
	struct MapRecordVariant :
		Variant<TValue ...> {

		inline friend constexpr auto operator == (
			MapRecordVariant const & thix,
			MapRecordVariant const & that
		) -> bool = default;

	};

	#pragma endregion

}
