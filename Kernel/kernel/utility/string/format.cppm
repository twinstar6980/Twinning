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

export namespace Twinning::Kernel {

	#pragma region function

	template <typename ... TArgument> requires
		CategoryConstraint<IsValid<TArgument ...>>
	inline auto format_string(
		ConstantStringView const & format,
		TArgument && ...           argument
	) -> String {
		return make_string(std::format(std::runtime_format(make_std_string_view(format)), as_forward<TArgument>(argument) ...));
	}

	#pragma endregion

	#pragma region type

	class StringFormatter {

	protected:

		ConstantStringView m_format;

	public:

		#pragma region constructor

		constexpr ~StringFormatter(
		) = default;

		// ----------------

		constexpr StringFormatter(
		) :
			m_format{} {
			return;
		}

		constexpr StringFormatter(
			StringFormatter const & that
		) = default;

		constexpr StringFormatter(
			StringFormatter && that
		) = default;

		// ----------------

		explicit constexpr StringFormatter(
			ConstantStringView const & format
		) :
			m_format{format} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			StringFormatter const & that
		) -> StringFormatter & = default;

		constexpr auto operator =(
			StringFormatter && that
		) -> StringFormatter & = default;

		// ----------------

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
		auto operator ()(
			TArgument && ... argument
		) const -> String {
			return format_string(thiz.m_format, as_forward<TArgument>(argument) ...);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region literal

	template <StaticString t_string> requires
		NoneConstraint
	inline constexpr auto operator ""_sf(
	) -> StringFormatter {
		return StringFormatter{t_string.view()};
	}

	#pragma endregion

}
