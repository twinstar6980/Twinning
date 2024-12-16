module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.data.json.value;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.null;
import twinning.kernel.utility.container.variant.enumerable_variant;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.container.map.map;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.miscellaneous.number_variant;
import twinning.kernel.utility.data.json.value_adapter;

export namespace Twinning::Kernel::JSON {

	#pragma region declaration

	class Value;

	#pragma endregion

	#pragma region sub type

	using Null = Null;

	using Boolean = Boolean;

	using Number = NumberVariant;

	using String = String;

	using Array = List<Value>;

	using Object = Map<String, Value>;

	// ----------------

	M_enumeration(
		M_wrap(ValueType),
		M_wrap(
			null,
			boolean,
			number,
			string,
			array,
			object,
		),
	);

	#pragma endregion

	#pragma region type

	class Value :
		public EnumerableVariant<ValueType, Null, Boolean, Number, String, Array, Object> {

	public:

		#pragma region structor

		~Value (
		) = default;

		// ----------------

		Value (
		) = default;

		Value (
			Value const & that
		) = default;

		Value (
			Value && that
		) = default;

		// ----------------

		using EnumerableVariant::EnumerableVariant;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Value const & that
		) -> Value & = default;

		auto operator = (
			Value && that
		) -> Value & = default;

		// ----------------

		auto operator [] (
			Size const & index
		) -> Value & {
			return thiz.get_array()[index];
		}

		auto operator [] (
			Size const & index
		) const -> Value const & {
			return thiz.get_array()[index];
		}

		auto operator () (
			Size const & index
		) -> Value & {
			return thiz.get_array()(index);
		}

		// ----------------

		auto operator [] (
			String const & key
		) -> Value & {
			return thiz.get_object()[key];
		}

		auto operator [] (
			String const & key
		) const -> Value const & {
			return thiz.get_object()[key];
		}

		auto operator () (
			String const & key
		) -> Value & {
			return thiz.get_object()(key);
		}

		// ----------------
		// TODO : sv remove

		auto operator [] (
			CStringView const & key
		) -> Value & {
			return thiz.get_object()[key];
		}

		auto operator [] (
			CStringView const & key
		) const -> Value const & {
			return thiz.get_object()[key];
		}

		auto operator () (
			CStringView const & key
		) -> Value & {
			return thiz.get_object()(key);
		}

		#pragma endregion

		#pragma region value

		auto is_null (
		) const -> Boolean {
			return thiz.is<Null>();
		}

		auto is_boolean (
		) const -> Boolean {
			return thiz.is<Boolean>();
		}

		auto is_number (
		) const -> Boolean {
			return thiz.is<Number>();
		}

		auto is_string (
		) const -> Boolean {
			return thiz.is<String>();
		}

		auto is_array (
		) const -> Boolean {
			return thiz.is<Array>();
		}

		auto is_object (
		) const -> Boolean {
			return thiz.is<Object>();
		}

		// ----------------

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto set_null (
			Argument && ... argument
		) -> Null & {
			return thiz.set<Null>(as_forward<Argument>(argument) ...);
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto set_boolean (
			Argument && ... argument
		) -> Boolean & {
			return thiz.set<Boolean>(as_forward<Argument>(argument) ...);
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto set_number (
			Argument && ... argument
		) -> Number & {
			return thiz.set<Number>(as_forward<Argument>(argument) ...);
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto set_string (
			Argument && ... argument
		) -> String & {
			return thiz.set<String>(as_forward<Argument>(argument) ...);
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto set_array (
			Argument && ... argument
		) -> Array & {
			return thiz.set<Array>(as_forward<Argument>(argument) ...);
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto set_object (
			Argument && ... argument
		) -> Object & {
			return thiz.set<Object>(as_forward<Argument>(argument) ...);
		}

		// ----------------

		auto get_null (
		) -> Null & {
			return thiz.get<Null>();
		}

		auto get_boolean (
		) -> Boolean & {
			return thiz.get<Boolean>();
		}

		auto get_number (
		) -> Number & {
			return thiz.get<Number>();
		}

		auto get_string (
		) -> String & {
			return thiz.get<String>();
		}

		auto get_array (
		) -> Array & {
			return thiz.get<Array>();
		}

		auto get_object (
		) -> Object & {
			return thiz.get<Object>();
		}

		// ----------------

		auto get_null (
		) const -> Null const & {
			return thiz.get<Null>();
		}

		auto get_boolean (
		) const -> Boolean const & {
			return thiz.get<Boolean>();
		}

		auto get_number (
		) const -> Number const & {
			return thiz.get<Number>();
		}

		auto get_string (
		) const -> String const & {
			return thiz.get<String>();
		}

		auto get_array (
		) const -> Array const & {
			return thiz.get<Array>();
		}

		auto get_object (
		) const -> Object const & {
			return thiz.get<Object>();
		}

		#pragma endregion

		#pragma region from & to by adapter

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto from (
			That &&       that,
			Option && ... option
		) -> Void {
			ValueAdapter<AsPure<That>>::from(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto to (
			That &&       that,
			Option && ... option
		) const -> Void {
			ValueAdapter<AsPure<That>>::to(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		// ----------------

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsPureInstance<That> && IsValid<Option ...>>
		auto to_of (
			Option && ... option
		) const -> That {
			auto that = That{};
			thiz.to(that, as_forward<Option>(option) ...);
			return that;
		}

		#pragma endregion

	};

	#pragma endregion

}
