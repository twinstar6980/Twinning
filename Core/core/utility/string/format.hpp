#pragma once

#include "core/utility/string/string.hpp"

#include "core/third/fmt.hpp"

namespace TwinStar::Core {

	#pragma region function

	template <typename ... Argument> requires
		CategoryConstraint<IsValid<Argument ...>>
	inline auto format_string (
		CStringView const & format,
		Argument && ...     argument
	) -> String {
		return make_string(fmt::format(fmt::runtime(make_std_string_view(format)), as_forward<Argument>(argument) ...));
	}

	#pragma endregion

	#pragma region type

	struct StringFormatter {

		CStringView format;

		// ----------------

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto operator () (
			Argument && ... argument
		) const -> String {
			return format_string(thiz.format, as_forward<Argument>(argument) ...);
		}

	};

	#pragma endregion

	#pragma region literal

	template <StaticString string>
	inline constexpr auto operator ""_sf (
	) -> StringFormatter {
		return StringFormatter{
			.format = string.view(),
		};
	}

	#pragma endregion

}
