#pragma once

// TODO : refactor

#include "core/version.hpp"
#include <version>
#include <iostream>
#include <algorithm>
#include <compare>

// style

/* naming style
 * - namespace : [CamelCase]
 * - type : [@Prefix][CamelCase]
 * - - concept : Is
 * - - decoration alias : As
 * - - template parameter (if for class) : T
 * - value : [@prefix_][under_line]
 * - - constant : k
 * - - global variable : g
 * - - non-public class field : m
 * - - template parameter (if for class) : t
 */

// wrap

#define M_wrap(...)\
	__VA_ARGS__

// detach

#define M_detach(...)\
	

// remain

#define M_remain(...)\
	__VA_ARGS__

// stringify

#define M_stringify_detail(...)\
	#__VA_ARGS__

#define M_stringify(...)\
	M_stringify_detail(__VA_ARGS__)

// catenate

#define M_catenate_detail(_1, _2)\
	_1##_2

#define M_catenate(_1, _2)\
	M_catenate_detail(_1, _2)

// count

#define M_count_detail(\
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,\
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
		9, 8, 7, 6, 5, 4, 3, 2, 1, 0\
	)

// map

// NOTE : M_map_0 with a va, because need support M_xxx(_x) == M_map(_m, _x) if _x is empty va and it passing by M_wrap(...), this case can't use ## to fix dot
// NOTE : use __VA_OPT__(, __VA_ARGS__) replace ##__VA_ARGS__ can fix this error, but resharper will show error highlight
#define M_map_0(_m,     ...) 
#define M_map_1(_m, _1, ...) 
#define M_map_2(_m, _1, ...) _m(_1) M_map_1(_m, ##__VA_ARGS__)
#define M_map_3(_m, _1, ...) _m(_1) M_map_2(_m, ##__VA_ARGS__)
#define M_map_4(_m, _1, ...) _m(_1) M_map_3(_m, ##__VA_ARGS__)
#define M_map_5(_m, _1, ...) _m(_1) M_map_4(_m, ##__VA_ARGS__)
#define M_map_6(_m, _1, ...) _m(_1) M_map_5(_m, ##__VA_ARGS__)
#define M_map_7(_m, _1, ...) _m(_1) M_map_6(_m, ##__VA_ARGS__)
#define M_map_8(_m, _1, ...) _m(_1) M_map_7(_m, ##__VA_ARGS__)
#define M_map_9(_m, _1, ...) _m(_1) M_map_8(_m, ##__VA_ARGS__)
#define M_map_10(_m, _1, ...) _m(_1) M_map_9(_m, ##__VA_ARGS__)
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

#define M_map(_m, ...)\
	M_catenate(M_map_, M_count(__VA_ARGS__))(_m, ##__VA_ARGS__)

// miscellaneous

// implicit specification of constructor and conversion operator
#define implicit\


// dereference this pointer
#define thiz\
	(*this)

// log to cout
#define M_log(...)\
	std::cout << (__VA_ARGS__) << "\n" << std::flush

// log to cout no line
#define M_log_no_line(...)\
	std::cout << (__VA_ARGS__) << std::flush

// TODO

#define M_simple_derived_class(TDerived, TBase, TSimplifyBase)\
	class TDerived : public TBase {\
	public:\
		using TBase::TSimplifyBase;\
	}

// TODO

namespace TwinStar::Core {

	using std::swap;

	using std::min;

	using std::max;

	// ----------------

	using StorageOrdering = std::strong_ordering;

	// ----------------

	template <typename>
	inline constexpr auto k_static_assert_fail = bool{false};

}
