#pragma once

#include "core/utility/base_wrapper/base.hpp"
#include "core/utility/base_wrapper/pointer.hpp"

namespace TwinStar::Core {

	#pragma region type

	class NullPointerWrapper final {

	public:

		#pragma region operator

		template <typename It> requires
			CategoryConstraint<IsPureInstance<It>>
			&& (IsPointerWrapper<It>)
		implicit constexpr operator It () const {
			return It{static_cast<ZPointer<typename It::Target>>(nullptr)};
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region operator

	inline constexpr auto operator == (
		NullPointerWrapper const & thix,
		NullPointerWrapper const & that
	) -> bool {
		return true;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator == (
		It const &                 thix,
		NullPointerWrapper const & that
	) -> bool {
		return thix.value == nullptr;
	}

	#pragma endregion

	#pragma region alias

	using NullPointer = NullPointerWrapper;

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_null_pointer = NullPointer{};

	#pragma endregion

}
