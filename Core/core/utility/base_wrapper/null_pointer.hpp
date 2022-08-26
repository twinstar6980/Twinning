#pragma once

#include "core/utility/base_wrapper/pointer.hpp"

namespace TwinKleS::Core {

	#pragma region type

	class NullPointerWrapper final {

	public: //

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
	) -> Boolean {
		return k_true;
	}

	inline constexpr auto operator != (
		NullPointerWrapper const & thix,
		NullPointerWrapper const & that
	) -> Boolean {
		return !(thix == that);
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator == (
		It const &                 thix,
		NullPointerWrapper const & that
	) -> Boolean {
		return mbw<Boolean>(thix.value == nullptr);
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator != (
		It const &                 thix,
		NullPointerWrapper const & that
	) -> Boolean {
		return !(thix == that);
	}

	#pragma endregion

	#pragma region alias

	using NullPointer = NullPointerWrapper;

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_null_pointer = NullPointer{};

	#pragma endregion

}
