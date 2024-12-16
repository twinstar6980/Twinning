module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.format;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_static_string;
import twinning.kernel.utility.string.string;
import twinning.kernel.third.fmt;

export namespace Twinning::Kernel {

	#pragma region function

	template <typename ... Argument> requires
		CategoryConstraint<IsValid<Argument ...>>
	inline auto format_string (
		CStringView const & format,
		Argument && ...     argument
	) -> String {
		return make_string(Third::fmt::format(Third::fmt::runtime(make_std_string_view(format)), as_forward<Argument>(argument) ...));
	}

	#pragma endregion

	#pragma region type

	class StringFormatter {

	protected:

		CStringView m_format;

	public:

		#pragma region structor

		constexpr ~StringFormatter (
		) = default;

		// ----------------

		constexpr StringFormatter (
		) :
			m_format{} {
		}

		constexpr StringFormatter (
			StringFormatter const & that
		) = default;

		constexpr StringFormatter (
			StringFormatter && that
		) = default;

		// ----------------

		explicit constexpr StringFormatter (
			CStringView const & format
		) :
			m_format{format} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			StringFormatter const & that
		) -> StringFormatter & = default;

		constexpr auto operator = (
			StringFormatter && that
		) -> StringFormatter & = default;

		// ----------------

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto operator () (
			Argument && ... argument
		) const -> String {
			return format_string(thiz.m_format, as_forward<Argument>(argument) ...);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region literal

	template <StaticString string> requires
		NoneConstraint
	inline constexpr auto operator ""_sf (
	) -> StringFormatter {
		return StringFormatter{string.view()};
	}

	#pragma endregion

}
