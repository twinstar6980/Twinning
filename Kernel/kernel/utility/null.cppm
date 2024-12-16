module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.null;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;

export namespace Twinning::Kernel {

	#pragma region type

	class Null final {

	public:

		#pragma region operator

		inline friend constexpr auto operator == (
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
