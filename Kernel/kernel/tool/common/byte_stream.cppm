module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.common.byte_stream;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool {

	#pragma region exchange

	template <auto t_mode>
	struct CommonByteStreamExchangeBase;

	template <>
	struct CommonByteStreamExchangeBase<StreamMode::Constant::i()> {

		using ExchangeableByteStreamView = IByteStreamView;

		template <typename Type> requires
			CategoryConstraint<IsPureInstance<Type>>
		using ExchangeableValue = Type;

		template <typename Type> requires
			CategoryConstraint<IsPureInstance<Type>>
		using ExchangeableConstantValue = Type const;

	};

	template <>
	struct CommonByteStreamExchangeBase<StreamMode::Constant::o()> {

		using ExchangeableByteStreamView = OByteStreamView;

		template <typename Type> requires
			CategoryConstraint<IsPureInstance<Type>>
		using ExchangeableValue = Type const;

		template <typename Type> requires
			CategoryConstraint<IsPureInstance<Type>>
		using ExchangeableConstantValue = Type const;

	};

	// ----------------

	template <auto t_mode>
	struct CommonByteStreamExchange :
		CommonByteStreamExchangeBase<t_mode> {

		#pragma region alias

		using ExchangeableByteStreamView = typename CommonByteStreamExchangeBase<t_mode>::ExchangeableByteStreamView;

		template <typename Type> requires
			CategoryConstraint<IsPureInstance<Type>>
		using ExchangeableValue = typename CommonByteStreamExchangeBase<t_mode>::template ExchangeableValue<Type>;

		template <typename Type> requires
			CategoryConstraint<IsPureInstance<Type>>
		using ExchangeableConstantValue = typename CommonByteStreamExchangeBase<t_mode>::template ExchangeableConstantValue<Type>;

		#pragma endregion

		#pragma region base

		template <typename Value, typename Exchanger> requires
			CategoryConstraint<IsPureInstance<Value> && IsPureInstance<Exchanger>>
			&& (IsGenericCallable<Exchanger>)
		inline static auto exchange (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Value> &   value,
			Exchanger const &            exchanger
		) -> Void {
			exchanger(data, value);
			return;
		}

		// ----------------

		template <typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_boolean (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Boolean> & value,
			ValueExchanger const &       value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_integer (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Integer> & value,
			ValueExchanger const &       value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_floater (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Floater> & value,
			ValueExchanger const &       value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_size (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Size> &    value,
			ValueExchanger const &       value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_character (
			ExchangeableByteStreamView &   data,
			ExchangeableValue<Character> & value,
			ValueExchanger const &         value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_unicode (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Unicode> & value,
			ValueExchanger const &       value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_enumerated (
			ExchangeableByteStreamView &    data,
			ExchangeableValue<Enumerated> & value,
			ValueExchanger const &          value_exchanger
		) -> Void {
			value_exchanger(data, value);
			return;
		}

		template <typename SizeExchanger, typename ContentExchanger> requires
			CategoryConstraint<IsPureInstance<SizeExchanger> && IsPureInstance<ContentExchanger>>
			&& (IsGenericCallable<SizeExchanger>)
			&& (IsGenericCallable<ContentExchanger>)
		inline static auto exchange_string (
			ExchangeableByteStreamView & data,
			ExchangeableValue<String> &  value,
			SizeExchanger const &        size_exchanger,
			ContentExchanger const &     content_exchanger
		) -> Void {
			auto value_size = Size{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				size_exchanger(data, value_size);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				value_size = value.size();
				size_exchanger(data, value_size);
			}
			content_exchanger(data, value, value_size);
			return;
		}

		// ----------------

		template <typename Value, typename StateExchanger> requires
			CategoryConstraint<IsPureInstance<Value> && IsPureInstance<StateExchanger>>
			&& (IsGenericCallable<StateExchanger>)
		inline static auto exchange_optional_state (
			ExchangeableByteStreamView &         data,
			ExchangeableValue<Optional<Value>> & value,
			StateExchanger const &               state_exchanger
		) -> Void {
			auto value_state = Boolean{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				state_exchanger(data, value_state);
				if (value_state) {
					value.set();
				}
				else {
					value.reset();
				}
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				value_state = value.has();
				state_exchanger(data, value_state);
			}
			return;
		}

		template <typename Value, typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<Value> && IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_optional_value (
			ExchangeableByteStreamView &         data,
			ExchangeableValue<Optional<Value>> & value,
			ValueExchanger const &               value_exchanger
		) -> Void {
			if (value.has()) {
				value_exchanger(data, value.get());
			}
			return;
		}

		template <typename Value, typename StateExchanger, typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<Value> && IsPureInstance<StateExchanger> && IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<StateExchanger>)
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_optional (
			ExchangeableByteStreamView &         data,
			ExchangeableValue<Optional<Value>> & value,
			StateExchanger const &               state_exchanger,
			ValueExchanger const &               value_exchanger
		) -> Void {
			exchange_optional_state(data, value, state_exchanger);
			exchange_optional_value(data, value, value_exchanger);
			return;
		}

		// ----------------

		template <typename ... Value, typename IndexExchanger> requires
			CategoryConstraint<IsPureInstance<Value ...> && IsPureInstance<IndexExchanger>>
			&& (IsGenericCallable<IndexExchanger>)
		inline static auto exchange_variant_index (
			ExchangeableByteStreamView &            data,
			ExchangeableValue<Variant<Value ...>> & value,
			IndexExchanger const &                  index_exchanger
		) -> Void {
			auto value_index = Size{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				index_exchanger(data, value_index);
				value.template set_of_index<value_index>();
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				value_index = value.index();
				index_exchanger(data, value_index);
			}
			return;
		}

		template <typename ... Value, typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<Value ...> && IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_variant_value (
			ExchangeableByteStreamView &            data,
			ExchangeableValue<Variant<Value ...>> & value,
			ValueExchanger const &                  value_exchanger
		) -> Void {
			Generalization::match<AsValuePackageOfIndex<sizeof...(Value)>>(
				value.index().value,
				[&] <auto index, auto value_index> (ValuePackage<index>, ValuePackage<value_index>) {
					value_exchanger(data, value.template get_of_index<mbox<Size>(value_index)>());
				}
			);
			return;
		}

		template <typename ... Value, typename IndexExchanger, typename ValueExchanger> requires
			CategoryConstraint<IsPureInstance<Value ...> && IsPureInstance<IndexExchanger> && IsPureInstance<ValueExchanger>>
			&& (IsGenericCallable<IndexExchanger>)
			&& (IsGenericCallable<ValueExchanger>)
		inline static auto exchange_variant (
			ExchangeableByteStreamView &            data,
			ExchangeableValue<Variant<Value ...>> & value,
			IndexExchanger const &                  index_exchanger,
			ValueExchanger const &                  value_exchanger
		) -> Void {
			exchange_variant_index(data, value, index_exchanger);
			exchange_variant_value(data, value, value_exchanger);
			return;
		}

		// ----------------

		template <typename Element, typename SizeExchanger> requires
			CategoryConstraint<IsPureInstance<Element> && IsPureInstance<SizeExchanger>>
			&& (IsGenericCallable<SizeExchanger>)
		inline static auto exchange_list_size (
			ExchangeableByteStreamView &       data,
			ExchangeableValue<List<Element>> & value,
			SizeExchanger const &              size_exchanger
		) -> Void {
			auto value_size = Size{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				size_exchanger(data, value_size);
				value.allocate_full(value_size);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				value_size = value.size();
				size_exchanger(data, value_size);
			}
			return;
		}

		template <typename Element, typename ... ElementExchanger> requires
			CategoryConstraint<IsPureInstance<Element> && IsPureInstance<ElementExchanger ...>>
			&& (IsGenericCallable<ElementExchanger> && ...)
		inline static auto exchange_list_element (
			ExchangeableByteStreamView &       data,
			ExchangeableValue<List<Element>> & value,
			ElementExchanger const & ...       element_exchanger
		) -> Void {
			Generalization::each_with<>(
				[&] (auto, auto & current_element_parser) {
					for (auto & value_element : value) {
						current_element_parser(data, value_element);
					}
				},
				element_exchanger ...
			);
			return;
		}

		template <typename Element, typename SizeExchanger, typename ... ElementExchanger> requires
			CategoryConstraint<IsPureInstance<Element> && IsPureInstance<SizeExchanger> && IsPureInstance<ElementExchanger ...>>
			&& (IsGenericCallable<SizeExchanger>)
			&& (IsGenericCallable<ElementExchanger> && ...)
		inline static auto exchange_list (
			ExchangeableByteStreamView &       data,
			ExchangeableValue<List<Element>> & value,
			SizeExchanger const &              size_exchanger,
			ElementExchanger const & ...       element_exchanger
		) -> Void {
			exchange_list_size(data, value, size_exchanger);
			exchange_list_element(data, value, element_exchanger ...);
			return;
		}

		// ----------------

		template <typename SizeExchanger> requires
			CategoryConstraint<IsPureInstance<SizeExchanger>>
			&& (IsGenericCallable<SizeExchanger>)
		inline static auto exchange_data (
			ExchangeableByteStreamView &  data,
			ExchangeableValue<ByteList> & value,
			SizeExchanger const &         size_exchanger
		) -> Void {
			auto value_size = Size{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				size_exchanger(data, value_size);
				value.allocate_full(value_size);
				data.read(value);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				value_size = value.size();
				size_exchanger(data, value_size);
				data.write(value);
			}
			return;
		}

		#pragma endregion

		#pragma region special

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
		inline static auto exchange_raw (
			ExchangeableByteStreamView &  data,
			ExchangeableValue<RawValue> & value
		) -> Void {
			if constexpr (t_mode == StreamMode::Constant::i()) {
				data.read(value);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				data.write(value);
			}
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
		inline static auto exchange_raw_constant (
			ExchangeableByteStreamView &          data,
			ExchangeableConstantValue<RawValue> & value
		) -> Void {
			if constexpr (t_mode == StreamMode::Constant::i()) {
				data.read_constant(value);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				data.write_constant(value);
			}
			return;
		}

		// ----------------

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBooleanBox<RawValue>)
		inline static auto exchange_boolean_fixed (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Boolean> & value
		) -> Void {
			auto raw_value = RawValue{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				exchange_raw(data, raw_value);
				value = cbox<Boolean>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				raw_value = cbox<RawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerBox<RawValue>)
		inline static auto exchange_integer_fixed (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Integer> & value
		) -> Void {
			auto raw_value = RawValue{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				exchange_raw(data, raw_value);
				value = cbox<Integer>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				raw_value = cbox<RawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsFloaterBox<RawValue>)
		inline static auto exchange_floater_fixed (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Floater> & value
		) -> Void {
			auto raw_value = RawValue{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				exchange_raw(data, raw_value);
				value = cbox<Floater>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				raw_value = cbox<RawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerBox<RawValue>)
		inline static auto exchange_size_fixed (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Size> &    value
		) -> Void {
			auto raw_value = RawValue{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				exchange_raw(data, raw_value);
				value = cbox<Size>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				raw_value = cbox<RawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsCharacterBox<RawValue>)
		inline static auto exchange_character_fixed (
			ExchangeableByteStreamView &   data,
			ExchangeableValue<Character> & value
		) -> Void {
			auto raw_value = RawValue{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				exchange_raw(data, raw_value);
				value = cbox<Character>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				raw_value = cbox<RawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsCharacterBox<RawValue>)
		inline static auto exchange_unicode_fixed (
			ExchangeableByteStreamView & data,
			ExchangeableValue<Unicode> & value
		) -> Void {
			auto raw_value = RawValue{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				exchange_raw(data, raw_value);
				value = cbox<Unicode>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				raw_value = cbox<RawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerBox<RawValue>)
		inline static auto exchange_enumerated_fixed (
			ExchangeableByteStreamView &    data,
			ExchangeableValue<Enumerated> & value
		) -> Void {
			auto raw_value = RawValue{};
			if constexpr (t_mode == StreamMode::Constant::i()) {
				exchange_raw(data, raw_value);
				value = cbox<Enumerated>(raw_value);
			}
			if constexpr (t_mode == StreamMode::Constant::o()) {
				raw_value = cbox<RawValue>(value);
				exchange_raw(data, raw_value);
			}
			return;
		}

		template <typename RawSizeValue> requires
			CategoryConstraint<IsPure<RawSizeValue>>
			&& (IsVoid<RawSizeValue> || IsIntegerBox<RawSizeValue>)
		inline static auto exchange_string_block (
			ExchangeableByteStreamView & data,
			ExchangeableValue<String> &  value
		) -> Void {
			exchange_raw(data, self_cast<StringBlock<RawSizeValue>>(value));
			return;
		}

		#pragma endregion

	};

	using CommonByteStreamExchangeForInput = CommonByteStreamExchange<StreamMode::Constant::i()>;

	using CommonByteStreamExchangeForOutput = CommonByteStreamExchange<StreamMode::Constant::o()>;

	#pragma endregion

}
