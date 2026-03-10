module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.base;
import twinning.kernel.utility.exception.exception;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;

export namespace Twinning::Kernel {

	#pragma region value

	template <typename TIt>
	concept IsBaseBoxValue =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsBuiltinFundamental<TIt> || IsBuiltinPointer<TIt> || IsBuiltinEnumeration<TIt>)
		;

	#pragma endregion

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseBoxValue<TValue>)
	class BaseBox {

	public:

		using Value = TValue;

	public:

		Value value;

	public:

		#pragma region constructor

		constexpr ~BaseBox(
		) = default;

		// ----------------

		constexpr BaseBox(
		) :
			value{} {
			return;
		}

		constexpr BaseBox(
			BaseBox const & that
		) = default;

		constexpr BaseBox(
			BaseBox && that
		) = default;

		// ----------------

		explicit constexpr BaseBox(
			Value const & value
		) :
			value{value} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			BaseBox const & that
		) -> BaseBox & = default;

		constexpr auto operator =(
			BaseBox && that
		) -> BaseBox & = default;

		#pragma endregion

	};

	// ----------------

	template <typename TIt>
	concept IsBaseBox = IsTemplateInstanceOfTt<TIt, BaseBox>;

	#pragma endregion

	#pragma region utility

	// NOTE: ALIAS: make_box
	template <typename TBox, typename TValue> requires
		CategoryConstraint<IsPureInstance<TBox> && IsPureInstance<TValue>>
		&& (IsBaseBox<TBox>)
		&& (IsBaseBoxValue<TValue>)
	inline constexpr auto mbox(
		TValue const & value
	) -> TBox {
		return TBox{static_cast<TBox::Value>(value)};
	}

	// ----------------

	// NOTE: ALIAS: cast_box
	template <typename TDestination, typename TSource> requires
		CategoryConstraint<IsPureInstance<TDestination> && IsPureInstance<TSource>>
		&& (IsBaseBox<TDestination>)
		&& (IsBaseBox<TSource>)
	inline constexpr auto cbox(
		TSource const & source
	) -> TDestination {
		return TDestination{static_cast<TDestination::Value>(source.value)};
	}

	#pragma endregion

}
