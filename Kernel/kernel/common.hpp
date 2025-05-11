#pragma once

#pragma region version

#define M_version "77"

#pragma endregion

#pragma region keyword

#define implicit\
	

#define thiz\
	(*this)

#pragma endregion

#pragma region utility

#define M_wrap(...)\
	__VA_ARGS__

#define M_detach(...)\
	

#define M_remain(...)\
	__VA_ARGS__

// ----------------

#define M_stringify_detail(...)\
	#__VA_ARGS__

#define M_stringify(...)\
	M_stringify_detail(__VA_ARGS__)

// ----------------

#define M_catenate_detail(_1, _2)\
	_1##_2

#define M_catenate(_1, _2)\
	M_catenate_detail(_1, _2)

// ----------------

#define M_count_detail(\
	_00, _01, _02, _03, _04, _05, _06, _07, _08, _09,\
	_10, _11, _12, _13, _14, _15, _16, _17, _18, _19,\
	_20, _21, _22, _23, _24, _25, _26, _27, _28, _29,\
	_30, _31, _32, _33, _34, _35, _36, _37, _38, _39,\
	_40, _41, _42, _43, _44, _45, _46, _47, _48, _49,\
	_50, _51, _52, _53, _54, _55, _56, _57, _58, _59,\
	_60, _61, _62, _63, _64, _65, _66, _67, _68, _69,\
	_70, _71, _72, _73, _74, _75, _76, _77, _78, _79,\
	_80, _81, _82, _83, _84, _85, _86, _87, _88, _89,\
	_90, _91, _92, _93, _94, _95, _96, _97, _98, _99,\
	_n, ...\
)\
	_n

#define M_count(...)\
	M_count_detail(\
		_ignored, ##__VA_ARGS__,\
		99, 98, 97, 96, 95, 94, 93, 92, 91, 90,\
		89, 88, 87, 86, 85, 84, 83, 82, 81, 80,\
		79, 78, 77, 76, 75, 74, 73, 72, 71, 70,\
		69, 68, 67, 66, 65, 64, 63, 62, 61, 60,\
		59, 58, 57, 56, 55, 54, 53, 52, 51, 50,\
		49, 48, 47, 46, 45, 44, 43, 42, 41, 40,\
		39, 38, 37, 36, 35, 34, 33, 32, 31, 30,\
		29, 28, 27, 26, 25, 24, 23, 22, 21, 20,\
		19, 18, 17, 16, 15, 14, 13, 12, 11, 10,\
		09, 08, 07, 06, 05, 04, 03, 02, 01, 00\
	)

// ----------------

// NOTE : M_map_0 will be triggered if empty va is passed
// NOTE : ignore the item 1 to support trailing comma style
#define M_map_00(_m,     ...) 
#define M_map_01(_m, _1, ...) 
#define M_map_02(_m, _1, ...) _m(_1) M_map_01(_m, ##__VA_ARGS__)
#define M_map_03(_m, _1, ...) _m(_1) M_map_02(_m, ##__VA_ARGS__)
#define M_map_04(_m, _1, ...) _m(_1) M_map_03(_m, ##__VA_ARGS__)
#define M_map_05(_m, _1, ...) _m(_1) M_map_04(_m, ##__VA_ARGS__)
#define M_map_06(_m, _1, ...) _m(_1) M_map_05(_m, ##__VA_ARGS__)
#define M_map_07(_m, _1, ...) _m(_1) M_map_06(_m, ##__VA_ARGS__)
#define M_map_08(_m, _1, ...) _m(_1) M_map_07(_m, ##__VA_ARGS__)
#define M_map_09(_m, _1, ...) _m(_1) M_map_08(_m, ##__VA_ARGS__)
#define M_map_10(_m, _1, ...) _m(_1) M_map_09(_m, ##__VA_ARGS__)
#define M_map_11(_m, _1, ...) _m(_1) M_map_10(_m, ##__VA_ARGS__)
#define M_map_12(_m, _1, ...) _m(_1) M_map_11(_m, ##__VA_ARGS__)
#define M_map_13(_m, _1, ...) _m(_1) M_map_12(_m, ##__VA_ARGS__)
#define M_map_14(_m, _1, ...) _m(_1) M_map_13(_m, ##__VA_ARGS__)
#define M_map_15(_m, _1, ...) _m(_1) M_map_14(_m, ##__VA_ARGS__)
#define M_map_16(_m, _1, ...) _m(_1) M_map_15(_m, ##__VA_ARGS__)
#define M_map_17(_m, _1, ...) _m(_1) M_map_16(_m, ##__VA_ARGS__)
#define M_map_18(_m, _1, ...) _m(_1) M_map_17(_m, ##__VA_ARGS__)
#define M_map_19(_m, _1, ...) _m(_1) M_map_18(_m, ##__VA_ARGS__)
#define M_map_20(_m, _1, ...) _m(_1) M_map_19(_m, ##__VA_ARGS__)
#define M_map_21(_m, _1, ...) _m(_1) M_map_20(_m, ##__VA_ARGS__)
#define M_map_22(_m, _1, ...) _m(_1) M_map_21(_m, ##__VA_ARGS__)
#define M_map_23(_m, _1, ...) _m(_1) M_map_22(_m, ##__VA_ARGS__)
#define M_map_24(_m, _1, ...) _m(_1) M_map_23(_m, ##__VA_ARGS__)
#define M_map_25(_m, _1, ...) _m(_1) M_map_24(_m, ##__VA_ARGS__)
#define M_map_26(_m, _1, ...) _m(_1) M_map_25(_m, ##__VA_ARGS__)
#define M_map_27(_m, _1, ...) _m(_1) M_map_26(_m, ##__VA_ARGS__)
#define M_map_28(_m, _1, ...) _m(_1) M_map_27(_m, ##__VA_ARGS__)
#define M_map_29(_m, _1, ...) _m(_1) M_map_28(_m, ##__VA_ARGS__)
#define M_map_30(_m, _1, ...) _m(_1) M_map_29(_m, ##__VA_ARGS__)
#define M_map_31(_m, _1, ...) _m(_1) M_map_30(_m, ##__VA_ARGS__)
#define M_map_32(_m, _1, ...) _m(_1) M_map_31(_m, ##__VA_ARGS__)
#define M_map_33(_m, _1, ...) _m(_1) M_map_32(_m, ##__VA_ARGS__)
#define M_map_34(_m, _1, ...) _m(_1) M_map_33(_m, ##__VA_ARGS__)
#define M_map_35(_m, _1, ...) _m(_1) M_map_34(_m, ##__VA_ARGS__)
#define M_map_36(_m, _1, ...) _m(_1) M_map_35(_m, ##__VA_ARGS__)
#define M_map_37(_m, _1, ...) _m(_1) M_map_36(_m, ##__VA_ARGS__)
#define M_map_38(_m, _1, ...) _m(_1) M_map_37(_m, ##__VA_ARGS__)
#define M_map_39(_m, _1, ...) _m(_1) M_map_38(_m, ##__VA_ARGS__)
#define M_map_40(_m, _1, ...) _m(_1) M_map_39(_m, ##__VA_ARGS__)
#define M_map_41(_m, _1, ...) _m(_1) M_map_40(_m, ##__VA_ARGS__)
#define M_map_42(_m, _1, ...) _m(_1) M_map_41(_m, ##__VA_ARGS__)
#define M_map_43(_m, _1, ...) _m(_1) M_map_42(_m, ##__VA_ARGS__)
#define M_map_44(_m, _1, ...) _m(_1) M_map_43(_m, ##__VA_ARGS__)
#define M_map_45(_m, _1, ...) _m(_1) M_map_44(_m, ##__VA_ARGS__)
#define M_map_46(_m, _1, ...) _m(_1) M_map_45(_m, ##__VA_ARGS__)
#define M_map_47(_m, _1, ...) _m(_1) M_map_46(_m, ##__VA_ARGS__)
#define M_map_48(_m, _1, ...) _m(_1) M_map_47(_m, ##__VA_ARGS__)
#define M_map_49(_m, _1, ...) _m(_1) M_map_48(_m, ##__VA_ARGS__)
#define M_map_50(_m, _1, ...) _m(_1) M_map_49(_m, ##__VA_ARGS__)
#define M_map_51(_m, _1, ...) _m(_1) M_map_50(_m, ##__VA_ARGS__)
#define M_map_52(_m, _1, ...) _m(_1) M_map_51(_m, ##__VA_ARGS__)
#define M_map_53(_m, _1, ...) _m(_1) M_map_52(_m, ##__VA_ARGS__)
#define M_map_54(_m, _1, ...) _m(_1) M_map_53(_m, ##__VA_ARGS__)
#define M_map_55(_m, _1, ...) _m(_1) M_map_54(_m, ##__VA_ARGS__)
#define M_map_56(_m, _1, ...) _m(_1) M_map_55(_m, ##__VA_ARGS__)
#define M_map_57(_m, _1, ...) _m(_1) M_map_56(_m, ##__VA_ARGS__)
#define M_map_58(_m, _1, ...) _m(_1) M_map_57(_m, ##__VA_ARGS__)
#define M_map_59(_m, _1, ...) _m(_1) M_map_58(_m, ##__VA_ARGS__)
#define M_map_60(_m, _1, ...) _m(_1) M_map_59(_m, ##__VA_ARGS__)
#define M_map_61(_m, _1, ...) _m(_1) M_map_60(_m, ##__VA_ARGS__)
#define M_map_62(_m, _1, ...) _m(_1) M_map_61(_m, ##__VA_ARGS__)
#define M_map_63(_m, _1, ...) _m(_1) M_map_62(_m, ##__VA_ARGS__)
#define M_map_64(_m, _1, ...) _m(_1) M_map_63(_m, ##__VA_ARGS__)
#define M_map_65(_m, _1, ...) _m(_1) M_map_64(_m, ##__VA_ARGS__)
#define M_map_66(_m, _1, ...) _m(_1) M_map_65(_m, ##__VA_ARGS__)
#define M_map_67(_m, _1, ...) _m(_1) M_map_66(_m, ##__VA_ARGS__)
#define M_map_68(_m, _1, ...) _m(_1) M_map_67(_m, ##__VA_ARGS__)
#define M_map_69(_m, _1, ...) _m(_1) M_map_68(_m, ##__VA_ARGS__)
#define M_map_70(_m, _1, ...) _m(_1) M_map_69(_m, ##__VA_ARGS__)
#define M_map_71(_m, _1, ...) _m(_1) M_map_70(_m, ##__VA_ARGS__)
#define M_map_72(_m, _1, ...) _m(_1) M_map_71(_m, ##__VA_ARGS__)
#define M_map_73(_m, _1, ...) _m(_1) M_map_72(_m, ##__VA_ARGS__)
#define M_map_74(_m, _1, ...) _m(_1) M_map_73(_m, ##__VA_ARGS__)
#define M_map_75(_m, _1, ...) _m(_1) M_map_74(_m, ##__VA_ARGS__)
#define M_map_76(_m, _1, ...) _m(_1) M_map_75(_m, ##__VA_ARGS__)
#define M_map_77(_m, _1, ...) _m(_1) M_map_76(_m, ##__VA_ARGS__)
#define M_map_78(_m, _1, ...) _m(_1) M_map_77(_m, ##__VA_ARGS__)
#define M_map_79(_m, _1, ...) _m(_1) M_map_78(_m, ##__VA_ARGS__)
#define M_map_80(_m, _1, ...) _m(_1) M_map_79(_m, ##__VA_ARGS__)
#define M_map_81(_m, _1, ...) _m(_1) M_map_80(_m, ##__VA_ARGS__)
#define M_map_82(_m, _1, ...) _m(_1) M_map_81(_m, ##__VA_ARGS__)
#define M_map_83(_m, _1, ...) _m(_1) M_map_82(_m, ##__VA_ARGS__)
#define M_map_84(_m, _1, ...) _m(_1) M_map_83(_m, ##__VA_ARGS__)
#define M_map_85(_m, _1, ...) _m(_1) M_map_84(_m, ##__VA_ARGS__)
#define M_map_86(_m, _1, ...) _m(_1) M_map_85(_m, ##__VA_ARGS__)
#define M_map_87(_m, _1, ...) _m(_1) M_map_86(_m, ##__VA_ARGS__)
#define M_map_88(_m, _1, ...) _m(_1) M_map_87(_m, ##__VA_ARGS__)
#define M_map_89(_m, _1, ...) _m(_1) M_map_88(_m, ##__VA_ARGS__)
#define M_map_90(_m, _1, ...) _m(_1) M_map_89(_m, ##__VA_ARGS__)
#define M_map_91(_m, _1, ...) _m(_1) M_map_90(_m, ##__VA_ARGS__)
#define M_map_92(_m, _1, ...) _m(_1) M_map_91(_m, ##__VA_ARGS__)
#define M_map_93(_m, _1, ...) _m(_1) M_map_92(_m, ##__VA_ARGS__)
#define M_map_94(_m, _1, ...) _m(_1) M_map_93(_m, ##__VA_ARGS__)
#define M_map_95(_m, _1, ...) _m(_1) M_map_94(_m, ##__VA_ARGS__)
#define M_map_96(_m, _1, ...) _m(_1) M_map_95(_m, ##__VA_ARGS__)
#define M_map_97(_m, _1, ...) _m(_1) M_map_96(_m, ##__VA_ARGS__)
#define M_map_98(_m, _1, ...) _m(_1) M_map_97(_m, ##__VA_ARGS__)
#define M_map_99(_m, _1, ...) _m(_1) M_map_98(_m, ##__VA_ARGS__)

#define M_map(_m, ...)\
	M_catenate(M_map_, M_count(__VA_ARGS__))(_m, ##__VA_ARGS__)

#pragma endregion

#pragma region advanced

#define M_simple_derived_class(_derived, _base, _simplify_base)\
	class _derived : public _base {\
	public:\
		using _base::_simplify_base;\
	}

#define M_simple_function_alias(_alias, _name)\
	template <typename ... Argument> \
	inline constexpr auto _alias (\
		Argument && ... argument\
	) -> decltype(_name(std::forward<Argument>(argument)...)) {\
		return _name(std::forward<Argument>(argument)...);\
	}

// ----------------

#define M_log(...)\
	std::cerr << (__VA_ARGS__) << "\n" << std::flush;\
	static_assert(true)

#pragma endregion

#pragma region assertion

#define assert_test(...)\
	if (!(__VA_ARGS__)) {\
		throw AssertionException{#__VA_ARGS__};\
	}\
	static_assert(true)

#define assert_fail(_message)\
	throw AssertionException{_message};\
	static_assert(true)

#pragma endregion

#pragma region enumeration

#define M_enumeration_item(_name)\
	_name,

#define M_enumeration_item_reflection(_name)\
	Trait::Reflection::EnumerationValueField<Trait::Reflection::make_string(#_name), Type::_name>,

#define M_enumeration_item_getter(_name)\
	inline static constexpr auto _name (\
	) -> Type {\
		return Type{Type::Value::_name};\
	}

// ----------------

#define M_enumeration(_type, _item, _)\
	struct _type :\
		EnumerationBox<decltype([] {\
			enum class Type : ZIntegerU8 {\
				M_map(M_enumeration_item, _item)\
			};\
			return declare<Type>();\
		}())> {\
		using EnumerationBox<typename _type::Value>::EnumerationBox;\
		struct Reflection {\
			using Type = typename EnumerationBox<typename _type::Value>::Value;\
			using EnumerationValue = AsTypePackageRemoveTail<TypePackage<\
				M_map(M_enumeration_item_reflection, _item)\
				None\
			>, 1_szz>;\
		};\
		struct Constant {\
			using Type = _type;\
			M_map(M_enumeration_item_getter, _item)\
		};\
	}

#pragma endregion

#pragma region record

#define M_record_field(_expression)\
	M_remain _expression;

#define M_record_field_reflection(_expression)\
	Trait::Reflection::MemberVariableField<Trait::Reflection::make_string(M_stringify(M_detach _expression)), &Type:: M_detach _expression>,

// ----------------

#define M_record(_category, _type, _item, _)\
	struct _type :\
		_category {\
		M_map(M_record_field, _item)\
		inline friend constexpr auto operator == (\
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

#pragma endregion

#pragma region stream

#define M_use_zps(_name, _source)\
	auto _name = AsUnmakeReference<decltype(_source)>{_source.reserve_view()};\
	auto finalize_forward_##_name = make_finalizer([&] { _source.forward(_name.position()); });\
	static_assert(true)

#define M_use_zps_of(_name)\
	M_use_zps(_name, _name##_)

#pragma endregion

#pragma region import

import twinning.common.std;

#pragma endregion
