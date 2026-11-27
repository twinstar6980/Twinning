module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.common.binary_stream;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool {

	#pragma region exchange

	template <auto t_mode>
	struct CommonBinaryStreamExchangeBase;

	template <>
	struct CommonBinaryStreamExchangeBase<StreamMode::Constant::input()> {

		using ExchangeableBinaryStreamView = InputBinaryStreamView;

		template <typename TType> requires
			CategoryConstraint<IsPureInstance<TType>>
		using ExchangeableValue = TType;

		template <typename TType> requires
			CategoryConstraint<IsPureInstance<TType>>
		using ExchangeableConstantValue = TType const;

	};

	template <>
	struct CommonBinaryStreamExchangeBase<StreamMode::Constant::output()> {

		using ExchangeableBinaryStreamView = OutputBinaryStreamView;

		template <typename TType> requires
			CategoryConstraint<IsPureInstance<TType>>
		using ExchangeableValue = TType const;

		template <typename TType> requires
			CategoryConstraint<IsPureInstance<TType>>
		using ExchangeableConstantValue = TType const;

	};

	// ----------------

	template <auto t_mode>
	struct CommonBinaryStreamExchange :
		CommonBinaryStreamExchangeBase<t_mode> {

		#pragma region alias

		using ExchangeableBinaryStreamView = CommonBinaryStreamExchangeBase<t_mode>::ExchangeableBinaryStreamView;

		template <typename TType> requires
			CategoryConstraint<IsPureInstance<TType>>
		using ExchangeableValue = CommonBinaryStreamExchangeBase<t_mode>::template ExchangeableValue<TType>;

		template <typename TType> requires
			CategoryConstraint<IsPureInstance<TType>>
		using ExchangeableConstantValue = CommonBinaryStreamExchangeBase<t_mode>::template ExchangeableConstantValue<TType>;

		#pragma endregion

		#pragma region base

		template <typename TValue, typename TExchanger> requires
			CategoryConstraint<IsPureInstance<TValue> && IsPureInstance<TExchanger>>
			&& (IsGenericCallable<TExchanger>)
		inline static auto exchange(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<TValue> &    value,
			TExchanger const &             exchanger
		) -> Void {
			exchanger(data, value);
			return;
		}

		// ----------------

		template <typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_boolean(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Boolean> &   value,
			TValueExchanger const &        value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_integer(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Integer> &   value,
			TValueExchanger const &        value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_floater(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Floater> &   value,
			TValueExchanger const &        value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_size(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Size> &      value,
			TValueExchanger const &        value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_character(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Character> & value,
			TValueExchanger const &        value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_unicode(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Unicode> &   value,
			TValueExchanger const &        value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_enumerated(
			ExchangeableBinaryStreamView &  data,
			ExchangeableValue<Enumerated> & value,
			TValueExchanger const &         value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename TSizeExchanger, typename TContentExchanger> requires
			CategoryConstraint<IsPureInstance<TSizeExchanger> && IsPureInstance<TContentExchanger>>
			&& (IsGenericCallable<TSizeExchanger>)
			&& (IsGenericCallable<TContentExchanger>)
		inline static auto exchange_string(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<String> &    value,
			TSizeExchanger const &         size_exchanger,
			TContentExchanger const &      content_exchanger
		) -> Void {
			auto value_size = Size{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				size_exchanger(data, value_size);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				value_size = value.size();
				size_exchanger(data, value_size);
			}
			content_exchanger(data, value, value_size);
			return;
		}

		// ----------------

		template <typename TValue, typename TStateExchanger> requires
			CategoryConstraint<IsPureInstance<TValue> && IsPureInstance<TStateExchanger>>
			&& (IsGenericCallable<TStateExchanger>)
		inline static auto exchange_optional_state(
			ExchangeableBinaryStreamView &        data,
			ExchangeableValue<Optional<TValue>> & value,
			TStateExchanger const &               state_exchanger
		) -> Void {
			auto value_state = Boolean{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				state_exchanger(data, value_state);
				if (value_state) {
					value.set();
				}
				else {
					value.reset();
				}
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				value_state = value.has();
				state_exchanger(data, value_state);
			}
			return;
		}

		template <typename TValue, typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValue> && IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_optional_value(
			ExchangeableBinaryStreamView &        data,
			ExchangeableValue<Optional<TValue>> & value,
			TValueExchanger const &               value_exchanger
		) -> Void {
			if (value.has()) {
				value_exchanger(data, value.get());
			}
			return;
		}

		template <typename TValue, typename TStateExchanger, typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValue> && IsPureInstance<TStateExchanger> && IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TStateExchanger>)
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_optional(
			ExchangeableBinaryStreamView &        data,
			ExchangeableValue<Optional<TValue>> & value,
			TStateExchanger const &               state_exchanger,
			TValueExchanger const &               value_exchanger
		) -> Void {
			exchange_optional_state(data, value, state_exchanger);
			exchange_optional_value(data, value, value_exchanger);
			return;
		}

		// ----------------

		template <typename ... TValue, typename TIndexExchanger> requires
			CategoryConstraint<IsPureInstance<TValue ...> && IsPureInstance<TIndexExchanger>>
			&& (IsGenericCallable<TIndexExchanger>)
		inline static auto exchange_variant_index(
			ExchangeableBinaryStreamView &           data,
			ExchangeableValue<Variant<TValue ...>> & value,
			TIndexExchanger const &                  index_exchanger
		) -> Void {
			auto value_index = Size{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				index_exchanger(data, value_index);
				value.template set_of_index<value_index>();
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				value_index = value.index();
				index_exchanger(data, value_index);
			}
			return;
		}

		template <typename ... TValue, typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValue ...> && IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_variant_value(
			ExchangeableBinaryStreamView &           data,
			ExchangeableValue<Variant<TValue ...>> & value,
			TValueExchanger const &                  value_exchanger
		) -> Void {
			Generalization::match<AsValuePackageOfIndex<sizeof...(TValue)>>(
				value.index().value,
				[&]<auto t_index, auto t_value_index>(ValuePackage<t_index>, ValuePackage<t_value_index>) {
					value_exchanger(data, value.template get_of_index<make_box<Size>(t_value_index)>());
				}
			);
			return;
		}

		template <typename ... TValue, typename TIndexExchanger, typename TValueExchanger> requires
			CategoryConstraint<IsPureInstance<TValue ...> && IsPureInstance<TIndexExchanger> && IsPureInstance<TValueExchanger>>
			&& (IsGenericCallable<TIndexExchanger>)
			&& (IsGenericCallable<TValueExchanger>)
		inline static auto exchange_variant(
			ExchangeableBinaryStreamView &           data,
			ExchangeableValue<Variant<TValue ...>> & value,
			TIndexExchanger const &                  index_exchanger,
			TValueExchanger const &                  value_exchanger
		) -> Void {
			exchange_variant_index(data, value, index_exchanger);
			exchange_variant_value(data, value, value_exchanger);
			return;
		}

		// ----------------

		template <typename TElement, typename TSizeExchanger> requires
			CategoryConstraint<IsPureInstance<TElement> && IsPureInstance<TSizeExchanger>>
			&& (IsGenericCallable<TSizeExchanger>)
		inline static auto exchange_list_size(
			ExchangeableBinaryStreamView &      data,
			ExchangeableValue<List<TElement>> & value,
			TSizeExchanger const &              size_exchanger
		) -> Void {
			auto value_size = Size{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				size_exchanger(data, value_size);
				value.allocate_full(value_size);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				value_size = value.size();
				size_exchanger(data, value_size);
			}
			return;
		}

		template <typename TElement, typename ... TElementExchanger> requires
			CategoryConstraint<IsPureInstance<TElement> && IsPureInstance<TElementExchanger ...>>
			&& (IsGenericCallable<TElementExchanger> && ...)
		inline static auto exchange_list_element(
			ExchangeableBinaryStreamView &      data,
			ExchangeableValue<List<TElement>> & value,
			TElementExchanger const & ...       element_exchanger
		) -> Void {
			Generalization::each_with<>(
				[&](auto, auto & current_element_parser) {
					for (auto & value_element : value) {
						current_element_parser(data, value_element);
					}
				},
				element_exchanger ...
			);
			return;
		}

		template <typename TElement, typename TSizeExchanger, typename ... TElementExchanger> requires
			CategoryConstraint<IsPureInstance<TElement> && IsPureInstance<TSizeExchanger> && IsPureInstance<TElementExchanger ...>>
			&& (IsGenericCallable<TSizeExchanger>)
			&& (IsGenericCallable<TElementExchanger> && ...)
		inline static auto exchange_list(
			ExchangeableBinaryStreamView &      data,
			ExchangeableValue<List<TElement>> & value,
			TSizeExchanger const &              size_exchanger,
			TElementExchanger const & ...       element_exchanger
		) -> Void {
			exchange_list_size(data, value, size_exchanger);
			exchange_list_element(data, value, element_exchanger ...);
			return;
		}

		// ----------------

		template <typename TKey, typename TValue, typename TSizeExchanger> requires
			CategoryConstraint<IsPureInstance<TKey> && IsPureInstance<TValue> && IsPureInstance<TSizeExchanger>>
			&& (IsGenericCallable<TSizeExchanger>)
		inline static auto exchange_map_size(
			ExchangeableBinaryStreamView &         data,
			ExchangeableValue<Map<TKey, TValue>> & value,
			TSizeExchanger const &                 size_exchanger
		) -> Void {
			auto value_size = Size{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				size_exchanger(data, value_size);
				value.allocate_full(value_size);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				value_size = value.size();
				size_exchanger(data, value_size);
			}
			return;
		}

		template <typename TKey, typename TValue, typename ... TElementExchanger> requires
			CategoryConstraint<IsPureInstance<TKey> && IsPureInstance<TValue> && IsPureInstance<TElementExchanger ...>>
			&& (IsGenericCallable<TElementExchanger> && ...)
		inline static auto exchange_map_element(
			ExchangeableBinaryStreamView &         data,
			ExchangeableValue<Map<TKey, TValue>> & value,
			TElementExchanger const & ...          element_exchanger
		) -> Void {
			Generalization::each_with<>(
				[&](auto, auto & current_element_parser) {
					for (auto & value_element : value) {
						current_element_parser(data, value_element);
					}
				},
				element_exchanger ...
			);
			return;
		}

		template <typename TKey, typename TValue, typename TSizeExchanger, typename ... TElementExchanger> requires
			CategoryConstraint<IsPureInstance<TKey> && IsPureInstance<TValue> && IsPureInstance<TSizeExchanger> && IsPureInstance<TElementExchanger ...>>
			&& (IsGenericCallable<TSizeExchanger>)
			&& (IsGenericCallable<TElementExchanger> && ...)
		inline static auto exchange_map(
			ExchangeableBinaryStreamView &         data,
			ExchangeableValue<Map<TKey, TValue>> & value,
			TSizeExchanger const &                 size_exchanger,
			TElementExchanger const & ...          element_exchanger
		) -> Void {
			exchange_map_size(data, value, size_exchanger);
			exchange_map_element(data, value, element_exchanger ...);
			return;
		}

		// ----------------

		template <typename TSizeExchanger> requires
			CategoryConstraint<IsPureInstance<TSizeExchanger>>
			&& (IsGenericCallable<TSizeExchanger>)
		inline static auto exchange_data(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Binary> &    value,
			TSizeExchanger const &         size_exchanger
		) -> Void {
			auto value_size = Size{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				size_exchanger(data, value_size);
				value.allocate(value_size);
				data.read(value);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				value_size = value.size();
				size_exchanger(data, value_size);
				data.write(value);
			}
			return;
		}

		#pragma endregion

		#pragma region special

		template <typename TRawValue> requires
			CategoryConstraint<IsPureInstance<TRawValue>>
		inline static auto exchange_raw(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<TRawValue> & value
		) -> Void {
			if constexpr (t_mode == StreamMode::Constant::input()) {
				data.read(value);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				data.write(value);
			}
			return;
		}

		template <typename TRawValue> requires
			CategoryConstraint<IsPureInstance<TRawValue>>
		inline static auto exchange_raw_constant(
			ExchangeableBinaryStreamView &         data,
			ExchangeableConstantValue<TRawValue> & value
		) -> Void {
			if constexpr (t_mode == StreamMode::Constant::input()) {
				data.read_constant(value);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				data.write_constant(value);
			}
			return;
		}

		// ----------------

		template <typename TRawValue> requires
			CategoryConstraint<IsPureInstance<TRawValue>>
			&& (IsBooleanBox<TRawValue>)
		inline static auto exchange_boolean_fixed(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Boolean> &   value
		) -> Void {
			auto raw_value = TRawValue{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				exchange_raw(data, raw_value);
				value = cast_box<Boolean>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				raw_value = cast_box<TRawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename TRawValue> requires
			CategoryConstraint<IsPureInstance<TRawValue>>
			&& (IsIntegerBox<TRawValue>)
		inline static auto exchange_integer_fixed(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Integer> &   value
		) -> Void {
			auto raw_value = TRawValue{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				exchange_raw(data, raw_value);
				value = cast_box<Integer>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				raw_value = cast_box<TRawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename TRawValue> requires
			CategoryConstraint<IsPureInstance<TRawValue>>
			&& (IsFloaterBox<TRawValue>)
		inline static auto exchange_floater_fixed(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Floater> &   value
		) -> Void {
			auto raw_value = TRawValue{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				exchange_raw(data, raw_value);
				value = cast_box<Floater>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				raw_value = cast_box<TRawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename TRawValue> requires
			CategoryConstraint<IsPureInstance<TRawValue>>
			&& (IsIntegerBox<TRawValue>)
		inline static auto exchange_size_fixed(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Size> &      value
		) -> Void {
			auto raw_value = TRawValue{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				exchange_raw(data, raw_value);
				value = cast_box<Size>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				raw_value = cast_box<TRawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename TRawValue> requires
			CategoryConstraint<IsPureInstance<TRawValue>>
			&& (IsCharacterBox<TRawValue>)
		inline static auto exchange_character_fixed(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Character> & value
		) -> Void {
			auto raw_value = TRawValue{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				exchange_raw(data, raw_value);
				value = cast_box<Character>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				raw_value = cast_box<TRawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename TRawValue> requires
			CategoryConstraint<IsPureInstance<TRawValue>>
			&& (IsCharacterBox<TRawValue>)
		inline static auto exchange_unicode_fixed(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<Unicode> &   value
		) -> Void {
			auto raw_value = TRawValue{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				exchange_raw(data, raw_value);
				value = cast_box<Unicode>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				raw_value = cast_box<TRawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename TRawValue> requires
			CategoryConstraint<IsPureInstance<TRawValue>>
			&& (IsIntegerBox<TRawValue>)
		inline static auto exchange_enumerated_fixed(
			ExchangeableBinaryStreamView &  data,
			ExchangeableValue<Enumerated> & value
		) -> Void {
			auto raw_value = TRawValue{};
			if constexpr (t_mode == StreamMode::Constant::input()) {
				exchange_raw(data, raw_value);
				value = cast_box<Enumerated>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::output()) {
				raw_value = cast_box<TRawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename TRawSizeValue> requires
			CategoryConstraint<IsPure<TRawSizeValue>>
			&& (IsVoid<TRawSizeValue> || IsIntegerBox<TRawSizeValue>)
		inline static auto exchange_string_block(
			ExchangeableBinaryStreamView & data,
			ExchangeableValue<String> &    value
		) -> Void {
			exchange_raw(data, unsafe_cast<StringBlock<TRawSizeValue>>(value));
			return;
		}

		#pragma endregion

	};

	using CommonBinaryStreamExchangeForInput = CommonBinaryStreamExchange<StreamMode::Constant::input()>;

	using CommonBinaryStreamExchangeForOutput = CommonBinaryStreamExchange<StreamMode::Constant::output()>;

	#pragma endregion

}
