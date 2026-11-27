module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.null_pointer;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box.base;
import twinning.kernel.utility.box.pointer;

export namespace Twinning::Kernel {

	#pragma region type

	class NullPointerBox {

	public:

		#pragma region constructor

		constexpr ~NullPointerBox(
		) = default;

		// ----------------

		constexpr NullPointerBox(
		) {
			return;
		}

		constexpr NullPointerBox(
			NullPointerBox const & that
		) = default;

		constexpr NullPointerBox(
			NullPointerBox && that
		) = default;

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			NullPointerBox const & that
		) -> NullPointerBox & = default;

		constexpr auto operator =(
			NullPointerBox && that
		) -> NullPointerBox & = default;

		#pragma endregion

		#pragma region operator

		template <typename TIt> requires
			CategoryConstraint<IsPureInstance<TIt>>
			&& (IsPointerBox<TIt>)
		implicit constexpr operator TIt() const {
			return TIt{static_cast<ZPointer<typename TIt::Target>>(nullptr)};
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region operator

	inline constexpr auto operator ==(
		NullPointerBox const & thix,
		NullPointerBox const & that
	) -> bool {
		return true;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator ==(
		TIt const &            thix,
		NullPointerBox const & that
	) -> bool {
		return thix.value == nullptr;
	}

	#pragma endregion

	#pragma region alias

	using NullPointer = NullPointerBox;

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_null_pointer = NullPointer{};

	#pragma endregion

}
