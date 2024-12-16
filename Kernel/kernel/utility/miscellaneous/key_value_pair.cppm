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

		implicit constexpr KeyValuePair (
		) = default;

		template <typename KeyObject, typename ValueObject> requires
			CategoryConstraint<IsValid<KeyObject> && IsValid<ValueObject>>
			&& (IsConstructible<Key, KeyObject &&> && IsConstructible<Value, ValueObject &&>)
		explicit constexpr KeyValuePair (
			KeyObject &&   key,
			ValueObject && value
		) :
			key{as_forward<KeyObject>(key)},
			value{as_forward<ValueObject>(value)} {
		}

		// ----------------

		inline friend auto operator == (
			KeyValuePair const & thix,
			KeyValuePair const & that
		) -> bool = default;

	};

	#pragma endregion

}
