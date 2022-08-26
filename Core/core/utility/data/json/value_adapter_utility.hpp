#pragma once

#include "core/utility/data/json/value.hpp"

namespace TwinKleS::Core::JSON {

	#pragma region check

	template <typename It>
	concept IsHasValueAdapterWithRegularFrom =
	CategoryConstraint<IsPureInstance<It>>
	&& (requires { { ValueAdapter<It>::from(declare<Value &>(), declare<It const &>()) } -> IsSame<Void>; })
	;

	template <typename It>
	concept IsHasValueAdapterWithRegularTo =
	CategoryConstraint<IsPureInstance<It>>
	&& (requires { { ValueAdapter<It>::to(declare<Value const &>(), declare<It &>()) } -> IsSame<Void>; })
	;

	#pragma endregion

	#pragma region adapter for aggregate by reflection

	template <typename TType, typename TFieldPackage> requires
		CategoryConstraint<IsPureInstance<TType> && IsPureInstance<TFieldPackage>>
		&& (IsTypePackage<TFieldPackage>)
	struct ValueAdapterForAggregateAsNoneByField {

		using Type = TType;

		using FieldPackage = TFieldPackage;

		// ----------------

		static constexpr auto has_from (
		) -> Boolean {
			auto result = k_true;
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					if constexpr (!IsHasValueAdapterWithRegularFrom<AsPure<decltype(Field::value_of(declare<Type &>()))>>) {
						result = k_false;
					}
					return;
				}
			);
			return result;
		}

		static constexpr auto has_to (
		) -> Boolean {
			auto result = k_true;
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					if constexpr (!IsHasValueAdapterWithRegularTo<AsPure<decltype(Field::value_of(declare<Type &>()))>>) {
						result = k_false;
					}
					return;
				}
			);
			return result;
		}

	};

	// ----------------

	template <typename TType, typename TFieldPackage> requires
		CategoryConstraint<IsPureInstance<TType> && IsPureInstance<TFieldPackage>>
		&& (IsTypePackage<TFieldPackage>)
	struct ValueAdapterForAggregateAsObjectByField :
		ValueAdapterForAggregateAsNoneByField<TType, TFieldPackage> {

		using ValueAdapterForAggregateAsNoneByField = ValueAdapterForAggregateAsNoneByField<TType, TFieldPackage>;

		using typename ValueAdapterForAggregateAsNoneByField::Type;

		using typename ValueAdapterForAggregateAsNoneByField::FieldPackage;

		using ValueAdapterForAggregateAsNoneByField::has_from;

		using ValueAdapterForAggregateAsNoneByField::has_to;

		// ----------------

		using This = Value;

		using That = TType;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void requires
			(has_from().value) {
			auto & thix_object = thix.set_object(mbw<Size>(FieldPackage::size));
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					thix_object(make_string_view(Field::name.view())).from(Field::value_of(that));
					return;
				}
			);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void requires
			(has_to().value) {
			auto & thix_object = thix.get_object();
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					thix_object[make_string_view(Field::name.view())].to(Field::value_of(that));
					return;
				}
			);
			return;
		}

	};

	template <typename TType, typename TFieldPackage> requires
		CategoryConstraint<IsPureInstance<TType> && IsPureInstance<TFieldPackage>>
		&& (IsTypePackage<TFieldPackage>)
	struct ValueAdapterForAggregateAsArrayByField :
		ValueAdapterForAggregateAsNoneByField<TType, TFieldPackage> {

		using ValueAdapterForAggregateAsNoneByField = ValueAdapterForAggregateAsNoneByField<TType, TFieldPackage>;

		using typename ValueAdapterForAggregateAsNoneByField::Type;

		using typename ValueAdapterForAggregateAsNoneByField::FieldPackage;

		using ValueAdapterForAggregateAsNoneByField::has_from;

		using ValueAdapterForAggregateAsNoneByField::has_to;

		// ----------------

		using This = Value;

		using That = TType;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void requires
			(has_from().value) {
			auto & thix_array = thix.set_array(mbw<Size>(FieldPackage::size));
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					thix_array(mbw<Size>(Field::index)).from(Field::value_of(that));
					return;
				}
			);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void requires
			(has_to().value) {
			auto & thix_array = thix.get_array();
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					thix_array[mbw<Size>(Field::index)].to(Field::value_of(that));
					return;
				}
			);
			return;
		}

	};

	// ----------------

	template <typename TType> requires
		CategoryConstraint<IsPureInstance<TType>>
	struct ValueAdapterForAggregateAsObjectByInformation :
		ValueAdapterForAggregateAsObjectByField<TType, typename Reflection::Information<TType>::MemberVariable> {
	};

	template <typename TType> requires
		CategoryConstraint<IsPureInstance<TType>>
	struct ValueAdapterForAggregateAsArrayByInformation :
		ValueAdapterForAggregateAsArrayByField<TType, typename Reflection::Information<TType>::MemberVariable> {
	};

	#pragma endregion

}

#define M_json_value_adapter_for_aggregate_as_array_by_field_of(_type, _member_variable, _)\
	struct ValueAdapter<_type>  {\
		\
		using This = Value;\
		\
		using That = _type;\
		\
		using Type = _type;\
		\
		using FieldPackage = M_reflection_field_member_variable_of_n(M_wrap(_member_variable));\
		\
		static auto from (\
			This &       thix,\
			That const & that\
		) -> Void requires\
			(ValueAdapterForAggregateAsArrayByField<Type, FieldPackage>::has_from().value) {\
			return ValueAdapterForAggregateAsArrayByField<Type, FieldPackage>::from(thix, that);\
		}\
		\
		static auto to (\
			This const & thix,\
			That &       that\
		) -> Void requires\
			(ValueAdapterForAggregateAsArrayByField<Type, FieldPackage>::has_to().value) {\
			return ValueAdapterForAggregateAsArrayByField<Type, FieldPackage>::to(thix, that);\
		}\
		\
	}

#define M_json_value_adapter_for_aggregate_as_array_by_field_of_unique(_type, _member_variable, _)\
	template <typename TType> requires\
		AutoConstraint\
		&& (IsSame<TType, _type>)\
	M_json_value_adapter_for_aggregate_as_array_by_field_of(TType, M_wrap(_member_variable), _)

// ----------------

#define M_json_value_adapter_for_aggregate_as_object_by_field_of(_type, _member_variable, _)\
	struct ValueAdapter<_type>  {\
		\
		using This = Value;\
		\
		using That = _type;\
		\
		using Type = _type;\
		\
		using FieldPackage = M_reflection_field_member_variable_of_n(M_wrap(_member_variable));\
		\
		static auto from (\
			This &       thix,\
			That const & that\
		) -> Void requires\
			(ValueAdapterForAggregateAsObjectByField<Type, FieldPackage>::has_from().value) {\
			return ValueAdapterForAggregateAsObjectByField<Type, FieldPackage>::from(thix, that);\
		}\
		\
		static auto to (\
			This const & thix,\
			That &       that\
		) -> Void requires\
			(ValueAdapterForAggregateAsObjectByField<Type, FieldPackage>::has_to().value) {\
			return ValueAdapterForAggregateAsObjectByField<Type, FieldPackage>::to(thix, that);\
		}\
		\
	}

#define M_json_value_adapter_for_aggregate_as_object_by_field_of_unique(_type, _member_variable, _)\
	template <typename TType> requires\
		AutoConstraint\
		&& (IsSame<TType, _type>)\
	M_json_value_adapter_for_aggregate_as_object_by_field_of(TType, M_wrap(_member_variable), _)

// ----------------

#define M_json_value_adapter_for_aggregate_as_object_by_information_of(...)\
	struct ValueAdapter<__VA_ARGS__> :\
		ValueAdapterForAggregateAsObjectByInformation<__VA_ARGS__> {\
	}

#define M_json_value_adapter_for_aggregate_as_object_by_information_of_unique(...)\
	template <>\
	struct ValueAdapter<__VA_ARGS__> :\
		ValueAdapterForAggregateAsObjectByInformation<__VA_ARGS__> {\
	}

// ----------------

#define M_json_value_adapter_for_aggregate_as_array_by_information_of(...)\
	struct ValueAdapter<__VA_ARGS__> :\
		ValueAdapterForAggregateAsArrayByInformation<__VA_ARGS__> {\
	}

#define M_json_value_adapter_for_aggregate_as_array_by_information_of_unique(...)\
	template <>\
	struct ValueAdapter<__VA_ARGS__> :\
		ValueAdapterForAggregateAsArrayByInformation<__VA_ARGS__> {\
	}
