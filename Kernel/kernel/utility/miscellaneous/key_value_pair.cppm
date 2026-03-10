module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.key_value_pair;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TKey, typename TValue> requires
		CategoryConstraint<IsPureInstance<TKey> && IsPureInstance<TValue>>
	struct KeyValuePair {

		using Key = TKey;

		using Value = TValue;

		// ----------------

		Key key{};

		Value value{};

		// ----------------

		implicit constexpr KeyValuePair(
		) = default;

		template <typename TKeyObject, typename TValueObject> requires
			CategoryConstraint<IsValid<TKeyObject> && IsValid<TValueObject>>
			&& (IsConstructible<Key, TKeyObject &&> && IsConstructible<Value, TValueObject &&>)
		explicit constexpr KeyValuePair(
			TKeyObject &&   key,
			TValueObject && value
		) :
			key{as_forward<TKeyObject>(key)},
			value{as_forward<TValueObject>(value)} {
		}

		// ----------------

		inline friend auto operator ==(
			KeyValuePair const & thix,
			KeyValuePair const & that
		) -> bool = default;

	};

	#pragma endregion

}
