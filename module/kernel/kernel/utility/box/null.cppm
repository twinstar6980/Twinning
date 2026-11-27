module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.null;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;

export namespace Twinning::Kernel {

	#pragma region type

	class Null {

	public:

		#pragma region constructor

		constexpr ~Null(
		) = default;

		// ----------------

		constexpr Null(
		) {
			return;
		}

		constexpr Null(
			Null const & that
		) = default;

		constexpr Null(
			Null && that
		) = default;

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			Null const & that
		) -> Null & = default;

		constexpr auto operator =(
			Null && that
		) -> Null & = default;

		#pragma endregion

		#pragma region operator

		inline friend constexpr auto operator ==(
			Null const & thix,
			Null const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_null = Null{};

	#pragma endregion

}
