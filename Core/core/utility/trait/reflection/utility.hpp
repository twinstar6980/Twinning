#pragma once

#include "core/utility/trait/reflection/field.hpp"
#include "core/utility/trait/reflection/information.hpp"

namespace TwinKleS::Core::Trait::Reflection {

	#pragma region iterate

	template <typename FieldPackage, typename Executor> requires
		CategoryConstraint<IsPureInstance<FieldPackage> && IsPureInstance<Executor>>
		&& (IsTypePackage<FieldPackage>) // TODO : check it is field package
		&& (IsGenericCallable<Executor>)
	inline constexpr auto iterate (
		Executor const & executor
	) -> Void {
		[&] <auto ...index> (
			ValuePackage<index...>
		) -> auto {
				(executor(IndexedField<index, typename FieldPackage::template Element<index>>{}), ...);
				return;
			}(AsValuePackageOfIndex<FieldPackage::size>{});
		return;
	}

	// ----------------

	template <typename Type, typename Executor> requires
		CategoryConstraint<IsPureInstance<Type> && IsPureInstance<Executor>>
		&& (IsRegistedClass<Type>)
		&& (IsGenericCallable<Executor>)
	inline constexpr auto iterate_member_variable (
		Executor const & executor
	) -> Void {
		return iterate<typename Information<Type>::MemberVariable>(executor);
	}

	template <typename Type, typename Executor> requires
		CategoryConstraint<IsPureInstance<Type> && IsPureInstance<Executor>>
		&& (IsRegistedClass<Type>)
		&& (IsGenericCallable<Executor>)
	inline constexpr auto iterate_member_function (
		Executor const & executor
	) -> Void {
		return iterate<typename Information<Type>::MemberFunction>(executor);
	}

	template <typename Type, typename Executor> requires
		CategoryConstraint<IsPureInstance<Type> && IsPureInstance<Executor>>
		&& (IsRegistedClass<Type>)
		&& (IsGenericCallable<Executor>)
	inline constexpr auto iterate_static_variable (
		Executor const & executor
	) -> Void {
		return iterate<typename Information<Type>::StaticVariable>(executor);
	}

	template <typename Type, typename Executor> requires
		CategoryConstraint<IsPureInstance<Type> && IsPureInstance<Executor>>
		&& (IsRegistedClass<Type>)
		&& (IsGenericCallable<Executor>)
	inline constexpr auto iterate_static_function (
		Executor const & executor
	) -> Void {
		return iterate<typename Information<Type>::StaticFunction>(executor);
	}

	// ----------------

	template <typename Type, typename Executor> requires
		CategoryConstraint<IsPureInstance<Type> && IsPureInstance<Executor>>
		&& (IsRegistedEnum<Type>)
		&& (IsGenericCallable<Executor>)
	inline constexpr auto iterate_enum_value (
		Executor const & executor
	) -> Void {
		return iterate<typename Information<Type>::EnumValue>(executor);
	}

	#pragma endregion

}

#define M_reflection_field_member_variable_of(_name)\
	Reflection::MemberVariableField<Reflection::make_string(#_name), &Type::_name>,

#define M_reflection_field_member_function_of(_name)\
	Reflection::MemberFunctionField<Reflection::make_string(#_name), &Type::_name>,

#define M_reflection_field_static_variable_of(_name)\
	Reflection::StaticVariableField<Reflection::make_string(#_name), &Type::_name>,

#define M_reflection_field_static_function_of(_name)\
	Reflection::StaticFunctionField<Reflection::make_string(#_name), &Type::_name>,

#define M_reflection_field_enum_value_of(_name)\
	Reflection::EnumValueField<Reflection::make_string(#_name), Type::_name>,

// ----------------

#define M_reflection_field_member_variable_of_n(_name)\
	AsTypePackageRemoveTail<TypePackage<\
		M_map(M_reflection_field_member_variable_of, _name)\
		None\
	>, 1_szz>

#define M_reflection_field_member_function_of_n(_name)\
	AsTypePackageRemoveTail<TypePackage<\
		M_map(M_reflection_field_member_function_of, _name)\
		None\
	>, 1_szz>

#define M_reflection_field_static_variable_of_n(_name)\
	AsTypePackageRemoveTail<TypePackage<\
		M_map(M_reflection_field_static_variable_of, _name)\
		None\
	>, 1_szz>

#define M_reflection_field_static_function_of_n(_name)\
	AsTypePackageRemoveTail<TypePackage<\
		M_map(M_reflection_field_static_function_of, _name)\
		None\
	>, 1_szz>

#define M_reflection_field_enum_value_of_n(_name)\
	AsTypePackageRemoveTail<TypePackage<\
		M_map(M_reflection_field_enum_value_of, _name)\
		None\
	>, 1_szz>

// ----------------

#define M_reflection_class(_type, _name, _member_variable, _member_function, _static_variable, _static_function, _)\
	template <>\
	struct Information<_type> {\
		using Type = _type;\
		inline static constexpr auto name = String{make_string(M_stringify(_name))};\
		using MemberVariable = M_reflection_field_member_variable_of_n(M_wrap(_member_variable));\
		using MemberFunction = M_reflection_field_member_function_of_n(M_wrap(_member_function));\
		using StaticVariable = M_reflection_field_static_variable_of_n(M_wrap(_static_variable));\
		using StaticFunction = M_reflection_field_static_function_of_n(M_wrap(_static_function));\
	}

#define M_reflection_struct(_type, _name, _member_variable, _)\
	M_reflection_class(M_wrap(_type), M_wrap(_name), M_wrap(_member_variable), M_wrap(), M_wrap(), M_wrap(), _)

// ----------------

#define M_reflection_class_of(_type, _name, _member_variable, _member_function, _static_variable, _static_function, _)\
	struct Information<_type> {\
		using Type = _type;\
		inline static constexpr auto name = String{make_string(M_stringify(_name))};\
		using MemberVariable = M_reflection_field_member_variable_of_n(M_wrap(_member_variable));\
		using MemberFunction = M_reflection_field_member_function_of_n(M_wrap(_member_function));\
		using StaticVariable = M_reflection_field_static_variable_of_n(M_wrap(_static_variable));\
		using StaticFunction = M_reflection_field_static_function_of_n(M_wrap(_static_function));\
	}

#define M_reflection_struct_of(_type, _name, _member_variable, _)\
	M_reflection_class_of(M_wrap(_type), M_wrap(_name), M_wrap(_member_variable), M_wrap(), M_wrap(), M_wrap(), _)

// ----------------

#define M_reflection_enum(_type, _name, _enum_value, _)\
	template <>\
	struct Information<_type> {\
		using Type = _type;\
		inline static constexpr auto name = String{make_string(M_stringify(_name))};\
		using EnumValue = M_reflection_field_enum_value_of_n(M_wrap(_enum_value));\
	}
