#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"

namespace TwinStar::Core {

	#pragma region type

	struct Record {
		friend constexpr auto operator == (
			Record const & thix,
			Record const & that
		) -> bool = default;
	};

	// ----------------

	struct DataRecord :
		Record {
		friend constexpr auto operator == (
			DataRecord const & thix,
			DataRecord const & that
		) -> bool = default;
	};

	struct ListRecord :
		Record {
		friend constexpr auto operator == (
			ListRecord const & thix,
			ListRecord const & that
		) -> bool = default;
	};

	struct MapRecord :
		Record {
		friend constexpr auto operator == (
			MapRecord const & thix,
			MapRecord const & that
		) -> bool = default;
	};

	// ----------------

	template <typename ... TValue> requires
		CategoryConstraint<IsPureInstance<TValue ...>>
		&& (sizeof...(TValue) > 0_szz)
	struct ListRecordVariant :
		Variant<TValue ...> {
		friend constexpr auto operator == (
			ListRecordVariant const & thix,
			ListRecordVariant const & that
		) -> bool = default;
	};

	template <typename ... TValue> requires
		CategoryConstraint<IsPureInstance<TValue ...>>
		&& (sizeof...(TValue) > 0_szz)
	struct MapRecordVariant :
		Variant<TValue ...> {
		friend constexpr auto operator == (
			MapRecordVariant const & thix,
			MapRecordVariant const & that
		) -> bool = default;
	};

	#pragma endregion

}

#define M_record_field(_expression)\
	M_remain _expression;

#define M_record_field_reflection(_expression)\
	Trait::Reflection::MemberVariableField<Trait::Reflection::make_string(M_stringify(M_detach _expression)), &Type:: M_detach _expression>,

// ----------------

#define M_record(_category, _type, _item, _)\
	struct _type :\
		_category {\
		M_map(M_record_field, _item)\
		friend auto operator == (\
			_type const & thix,\
			_type const & that\
		) -> bool = default;\
		struct Reflection {\
			using Type = _type;\
			using MemberVariable = AsTypePackageRemoveTail<TypePackage<\
				M_map(M_record_field_reflection, _item)\
				None\
			>, 1_szz>;\
		};\
	}

#define M_record_of_data(_type, _item, _)\
	M_record(DataRecord, M_wrap(_type), M_wrap(_item), _)

#define M_record_of_list(_type, _item, _)\
	M_record(ListRecord, M_wrap(_type), M_wrap(_item), _)

#define M_record_of_map(_type, _item, _)\
	M_record(MapRecord, M_wrap(_type), M_wrap(_item), _)
