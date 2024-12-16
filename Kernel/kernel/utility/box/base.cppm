module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.base;
import twinning.kernel.utility.exception.exception;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;

export namespace Twinning::Kernel {

	#pragma region value

	template <typename It>
	concept IsBaseBoxValue =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsBuiltinFundamental<It> || IsBuiltinPointer<It> || IsBuiltinEnumeration<It>)
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

		#pragma region structor

		constexpr ~BaseBox (
		) = default;

		// ----------------

		constexpr BaseBox (
		) :
			value{} {
		}

		constexpr BaseBox (
			BaseBox const & that
		) = default;

		constexpr BaseBox (
			BaseBox && that
		) = default;

		// ----------------

		explicit constexpr BaseBox (
			Value const & value
		) :
			value{value} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			BaseBox const & that
		) -> BaseBox & = default;

		constexpr auto operator = (
			BaseBox && that
		) -> BaseBox & = default;

		#pragma endregion

	};

	// ----------------

	template <typename It>
	concept IsBaseBox = IsTemplateInstanceOfT<It, BaseBox>;

	#pragma endregion

	#pragma region utility

	// NOTE : ALIAS - make_box
	template <typename Box, typename Value> requires
		CategoryConstraint<IsPureInstance<Box> && IsPureInstance<Value>>
		&& (IsBaseBox<Box>)
		&& (IsBaseBoxValue<Value>)
	inline constexpr auto mbox (
		Value const & value
	) -> Box {
		return Box{static_cast<typename Box::Value>(value)};
	}

	// ----------------

	// NOTE : ALIAS - cast_box
	template <typename Destination, typename Source> requires
		CategoryConstraint<IsPureInstance<Destination> && IsPureInstance<Source>>
		&& (IsBaseBox<Destination>)
		&& (IsBaseBox<Source>)
	inline constexpr auto cbox (
		Source const & source
	) -> Destination {
		return Destination{static_cast<typename Destination::Value>(source.value)};
	}

	#pragma endregion

}
