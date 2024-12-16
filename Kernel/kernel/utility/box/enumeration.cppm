module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.enumeration;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box.base;
import twinning.kernel.utility.box.enumerated;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseBoxValue<TValue>)
	class EnumerationBox :
		public EnumeratedBox<TValue> {

	private:

		using EnumeratedBox = EnumeratedBox<TValue>;

	public:

		using typename EnumeratedBox::Value;

		using Underlying = Kernel::EnumeratedBox<std::underlying_type_t<Value>>;

	public:

		#pragma region structor

		constexpr ~EnumerationBox (
		) = default;

		// ----------------

		constexpr EnumerationBox (
		) = default;

		constexpr EnumerationBox (
			EnumerationBox const & that
		) = default;

		constexpr EnumerationBox (
			EnumerationBox && that
		) = default;

		// ----------------

		using EnumeratedBox::EnumeratedBox;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			EnumerationBox const & that
		) -> EnumerationBox & = default;

		constexpr auto operator = (
			EnumerationBox && that
		) -> EnumerationBox & = default;

		#pragma endregion

		#pragma region underlying

		auto as_underlying (
		) -> Underlying & {
			return self_cast<Underlying>(thiz);
		}

		auto as_underlying (
		) const -> Underlying const & {
			return self_cast<Underlying>(thiz);
		}

		// ----------------

		constexpr auto underlying (
		) const -> Underlying {
			return cbox<Underlying>(thiz);
		}

		#pragma endregion

	};

	// ----------------

	template <typename It>
	concept IsEnumerationBox = IsTemplateInstanceOfT<It, EnumerationBox>;

	#pragma endregion

	#pragma region alias

	template <typename Value> requires
		AutoConstraint
	using Enumeration = EnumerationBox<Value>;

	#pragma endregion

}
