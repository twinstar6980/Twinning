// twinning.kernel.test

#include <cfloat>
#include "kernel/test/test.hpp"
#include "kernel/common.hpp"

import twinning.kernel.utility;

namespace Twinning::Kernel::Test {

	#pragma region common

	inline auto print_verbose(
		ConstantStringView const & message
	) -> Void {
		M_log(to_std_string(message));
		return;
	}

	#pragma endregion

	#pragma region str2num

	template <typename X>
	inline auto fx(X x) {
		auto xn = cast_box<Integer>(BasicStringStreamAdapter<Character, X>::capacity(x, k_false));
		auto xs = string_from_of(x, k_false);
		print_verbose("fx: [{}] = {}"_sf(string_from_of(xn, k_false), xs));
		auto v = string_to_of<X>(xs);
		auto vn = cast_box<Integer>(BasicStringStreamAdapter<Character, X>::capacity(v, k_false));
		auto vs = string_from_of(v, k_false);
		print_verbose("fv: [{}] = {}"_sf(string_from_of(vn, k_false), xs));
		if (xs != vs) {
			print_verbose("ng!!"_sf());
		}
		return;
	}

	inline auto str2num(
	) -> Void {
		fx(minimum_number_of<Integer>());
		fx(maximum_number_of<Integer>());
		fx(123.0e+9_f);
		fx(-123.0e-9_f);
		fx(123.0e+59_f);
		fx(123.0e+301_f);
		fx(-123.0e+301_f);
		fx(Floater{maximum_number_of<Floater>()});
		fx(Floater{DBL_MIN});
		fx(Floater{+infinity_number_of<Floater>()});
		fx(Floater{-infinity_number_of<Floater>()});
		fx(Floater{+invalid_number_of<Floater>()});
		fx(Floater{-invalid_number_of<Floater>()});
		return;
	}

	#pragma endregion

	#pragma region main

	// @formatter:off
	__attribute__((visibility("default")))
	// @formatter:on
	extern auto run(
	) -> void {
		#if defined M_build_release
		str2num();
		#endif
		return;
	}

	#pragma endregion

}
