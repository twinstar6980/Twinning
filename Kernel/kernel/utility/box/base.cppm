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
		&& (IsBuiltinArithmetic<TIt> || IsBuiltinEnumeration<TIt> || IsBuiltinPointer<TIt>)
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

	namespace Detail {

		template <typename TDestination, typename TSource> requires
			CategoryConstraint<IsPureInstance<TDestination> && IsPureInstance<TSource>>
			&& (IsBaseBoxValue<TDestination>)
			&& (IsBaseBoxValue<TSource>)
		inline constexpr auto narrow_box_value(
			TSource const & source
		) -> TDestination {
			constexpr auto map_underlying_type = []<typename TValue>() {
				if constexpr (IsBuiltinBoolean<TValue>) {
					return declare<TValue>();
				}
				if constexpr (IsBuiltinNumber<TValue>) {
					return declare<TValue>();
				}
				if constexpr (IsBuiltinCharacter<TValue>) {
					if constexpr (IsSame<TValue, ZCharacter8>) {
						return declare<ZIntegerU8>();
					}
					if constexpr (IsSame<TValue, ZCharacter16>) {
						return declare<ZIntegerU16>();
					}
					if constexpr (IsSame<TValue, ZCharacter32>) {
						return declare<ZIntegerU32>();
					}
					if constexpr (IsSame<TValue, ZCharacterN>) {
						return declare<ZIntegerU8>();
					}
					if constexpr (IsSame<TValue, ZCharacterW>) {
						if constexpr (sizeof(ZCharacterW) == 2_szz) {
							return declare<ZIntegerU16>();
						}
						if constexpr (sizeof(ZCharacterW) == 4_szz) {
							return declare<ZIntegerU32>();
						}
					}
				}
				if constexpr (IsBuiltinEnumeration<TValue>) {
					return declare<std::underlying_type_t<TValue>>();
				}
				if constexpr (IsBuiltinPointer<TValue>) {
					return declare<std::intptr_t>();
				}
			};
			using SourceValue = decltype(map_underlying_type.template operator ()<TSource>());
			using DestinationValue = decltype(map_underlying_type.template operator ()<TDestination>());
			constexpr auto will_lose_digit = std::numeric_limits<SourceValue>::digits > std::numeric_limits<DestinationValue>::digits;
			constexpr auto compute_power_of_two = []<typename TValue>(ZSize const & count) -> TValue {
				auto result = static_cast<TValue>(1);
				for (auto index = 0_szz; index < count; ++index) {
					result *= static_cast<TValue>(2);
				}
				return result;
			};
			auto source_value = static_cast<SourceValue>(source);
			if constexpr (!IsBuiltinBoolean<TSource> && IsBuiltinBoolean<TDestination>) {
				if (source_value != static_cast<SourceValue>(0) && source_value != static_cast<SourceValue>(1)) {
					throw ConversionException{typeid(TSource), typeid(TDestination)};
				}
			}
			if constexpr (IsBuiltinInteger<SourceValue> && IsBuiltinInteger<DestinationValue>) {
				if constexpr (IsBuiltinSignedInteger<SourceValue> && (will_lose_digit || !IsBuiltinSignedInteger<DestinationValue>)) {
					if (source_value < static_cast<SourceValue>(std::numeric_limits<DestinationValue>::min())) {
						throw ConversionException{typeid(TSource), typeid(TDestination)};
					}
				}
				if constexpr (will_lose_digit) {
					if (source_value > static_cast<SourceValue>(std::numeric_limits<DestinationValue>::max())) {
						throw ConversionException{typeid(TSource), typeid(TDestination)};
					}
				}
			}
			if constexpr (IsBuiltinInteger<SourceValue> && IsBuiltinFloater<DestinationValue>) {
				if constexpr (IsBuiltinSignedInteger<SourceValue> && (will_lose_digit)) {
					if (source_value < -compute_power_of_two.template operator()<SourceValue>(std::numeric_limits<DestinationValue>::digits)) {
						throw ConversionException{typeid(TSource), typeid(TDestination)};
					}
				}
				if constexpr (will_lose_digit) {
					if (source_value > +compute_power_of_two.template operator()<SourceValue>(std::numeric_limits<DestinationValue>::digits)) {
						throw ConversionException{typeid(TSource), typeid(TDestination)};
					}
				}
			}
			if constexpr (IsBuiltinFloater<SourceValue> && IsBuiltinFloater<DestinationValue>) {
				if constexpr (will_lose_digit) {
					if (std::isfinite(source_value)) {
						if (source_value < -static_cast<SourceValue>(std::numeric_limits<DestinationValue>::max())) {
							throw ConversionException{typeid(TSource), typeid(TDestination)};
						}
						if (source_value > +static_cast<SourceValue>(std::numeric_limits<DestinationValue>::max())) {
							throw ConversionException{typeid(TSource), typeid(TDestination)};
						}
					}
				}
			}
			if constexpr (IsBuiltinFloater<SourceValue> && IsBuiltinInteger<DestinationValue>) {
				if constexpr (true) {
					if (!std::isfinite(source_value)) {
						throw ConversionException{typeid(TSource), typeid(TDestination)};
					}
				}
				if constexpr (will_lose_digit || !IsBuiltinSignedInteger<DestinationValue>) {
					if (source_value < static_cast<SourceValue>(std::numeric_limits<DestinationValue>::min())) {
						throw ConversionException{typeid(TSource), typeid(TDestination)};
					}
				}
				else {
					if (source_value < -compute_power_of_two.template operator()<SourceValue>(std::numeric_limits<SourceValue>::digits)) {
						throw ConversionException{typeid(TSource), typeid(TDestination)};
					}
				}
				if constexpr (will_lose_digit) {
					if (source_value > static_cast<SourceValue>(std::numeric_limits<DestinationValue>::max())) {
						throw ConversionException{typeid(TSource), typeid(TDestination)};
					}
				}
				else {
					if (source_value > +compute_power_of_two.template operator()<SourceValue>(std::numeric_limits<SourceValue>::digits)) {
						throw ConversionException{typeid(TSource), typeid(TDestination)};
					}
				}
				if constexpr (true) {
					if (source_value != static_cast<SourceValue>(static_cast<DestinationValue>(source_value))) {
						throw ConversionException{typeid(TSource), typeid(TDestination)};
					}
				}
			}
			return static_cast<TDestination>(source_value);
		}

	}

	// ----------------

	template <typename TBox, typename TValue> requires
		CategoryConstraint<IsPureInstance<TBox> && IsPureInstance<TValue>>
		&& (IsBaseBox<TBox>)
		&& (IsBaseBoxValue<TValue>)
	inline constexpr auto make_box(
		TValue const & value
	) -> TBox {
		return TBox{Kernel::Detail::narrow_box_value<typename TBox::Value>(value)};
	}

	template <typename TBox, typename TValue> requires
		CategoryConstraint<IsPureInstance<TBox> && IsPureInstance<TValue>>
		&& (IsBaseBox<TBox>)
		&& (IsBaseBoxValue<TValue>)
	inline constexpr auto make_box_unsafe(
		TValue const & value
	) -> TBox {
		return TBox{static_cast<TBox::Value>(value)};
	}

	// ----------------

	template <typename TValue, typename TBox> requires
		CategoryConstraint<IsPureInstance<TValue> && IsPureInstance<TBox>>
		&& (IsBaseBoxValue<TValue>)
		&& (IsBaseBox<TBox>)
	inline constexpr auto unmake_box(
		TBox const & box
	) -> TValue {
		return Kernel::Detail::narrow_box_value<TValue>(box.value);
	}

	template <typename TValue, typename TBox> requires
		CategoryConstraint<IsPureInstance<TValue> && IsPureInstance<TBox>>
		&& (IsBaseBoxValue<TValue>)
		&& (IsBaseBox<TBox>)
	inline constexpr auto unmake_box_unsafe(
		TBox const & box
	) -> TValue {
		return static_cast<TValue>(box.value);
	}

	// ----------------

	template <typename TDestination, typename TSource> requires
		CategoryConstraint<IsPureInstance<TDestination> && IsPureInstance<TSource>>
		&& (IsBaseBox<TDestination>)
		&& (IsBaseBox<TSource>)
	inline constexpr auto cast_box(
		TSource const & source
	) -> TDestination {
		return TDestination{Kernel::Detail::narrow_box_value<typename TDestination::Value>(source.value)};
	}

	template <typename TDestination, typename TSource> requires
		CategoryConstraint<IsPureInstance<TDestination> && IsPureInstance<TSource>>
		&& (IsBaseBox<TDestination>)
		&& (IsBaseBox<TSource>)
	inline constexpr auto cast_box_unsafe(
		TSource const & source
	) -> TDestination {
		return TDestination{static_cast<TDestination::Value>(source.value)};
	}

	#pragma endregion

}
