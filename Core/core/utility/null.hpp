#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"

namespace TwinKleS::Core {

	#pragma region type

	class Null final {

	public: //

		#pragma region operator

		friend constexpr auto operator == (
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
