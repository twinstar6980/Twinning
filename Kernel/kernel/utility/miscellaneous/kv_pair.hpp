#pragma once

#include "kernel/utility/base_wrapper/wrapper.hpp"

namespace Twinning::Kernel {

	#pragma region type

	template <typename TKey, typename TValue> requires
		CategoryConstraint<IsPureInstance<TKey> && IsPureInstance<TValue>>
	struct KVPair {

		using Key = TKey;

		using Value = TValue;

		// ----------------

		Key key{};

		Value value{};

		// ----------------

		implicit constexpr KVPair (
		) = default;

		template <typename KeyObject, typename ValueObject> requires
			CategoryConstraint<IsValid<KeyObject> && IsValid<ValueObject>>
			&& (IsConstructible<Key, KeyObject &&> && IsConstructible<Value, ValueObject &&>)
		explicit constexpr KVPair (
			KeyObject &&   key,
			ValueObject && value
		) :
			key{as_forward<KeyObject>(key)},
			value{as_forward<ValueObject>(value)} {
		}

		// ----------------

		friend auto operator == (
			KVPair const & thix,
			KVPair const & that
		) -> bool = default;

	};

	#pragma endregion

}
