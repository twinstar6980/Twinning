module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.boolean;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box.base;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseBoxValue<TValue>)
	class BooleanBox :
		public BaseBox<TValue> {

	private:

		using BaseBox = BaseBox<TValue>;

	public:

		using typename BaseBox::Value;

	public:

		#pragma region constructor

		constexpr ~BooleanBox(
		) = default;

		// ----------------

		constexpr BooleanBox(
		) = default;

		constexpr BooleanBox(
			BooleanBox const & that
		) = default;

		constexpr BooleanBox(
			BooleanBox && that
		) = default;

		// ----------------

		implicit constexpr BooleanBox(
			Value const & value
		) :
			BaseBox{value} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			BooleanBox const & that
		) -> BooleanBox & = default;

		constexpr auto operator =(
			BooleanBox && that
		) -> BooleanBox & = default;

		// ----------------

		implicit constexpr operator Value &() {
			return thiz.value;
		}

		implicit constexpr operator Value const &() const {
			return thiz.value;
		}

		#pragma endregion

	};

	// ----------------

	template <typename TIt>
	concept IsBooleanBox = IsTemplateInstanceOfTt<TIt, BooleanBox>;

	#pragma endregion

	#pragma region operator

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsBooleanBox<TIt>)
	inline constexpr auto operator ==(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value == that.value;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsBooleanBox<TIt>)
	inline constexpr auto operator !(
		TIt const & thix
	) -> TIt {
		return TIt{!thix.value};
	}

	#pragma endregion

	#pragma region alias

	using Boolean8 = BooleanBox<ZBoolean8>;

	#pragma endregion

	#pragma region regular type

	M_simple_derived_class(Boolean, Boolean8, Boolean8);

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_false = Boolean{mbox<Boolean>(false)};

	inline constexpr auto k_true = Boolean{mbox<Boolean>(true)};

	#pragma endregion

}
