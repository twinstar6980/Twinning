#pragma once

#include "kernel/utility/trait/base.hpp"
#include "kernel/utility/trait/category.hpp"

namespace Twinning::Kernel::Trait::Reflection {

	#pragma region type

	using StringView = std::string_view;

	template <auto t_size> requires
		CategoryConstraint<>
		&& (IsSameV<t_size, ZSize>)
	struct String {

		inline static constexpr auto size = ZSize{t_size};

		// ----------------

		ZArray<ZCharacter, t_size + 1> data{};

		// ----------------

		constexpr auto view (
		) const -> StringView {
			return StringView{thiz.data, thiz.size};
		}

	};

	#pragma endregion

	#pragma region utility

	template <auto size> requires
		CategoryConstraint<>
		&& (IsSameV<size, ZSize>)
	inline constexpr auto make_string (
		ZArray<ZCharacter, size> const & source
	) -> String<size - 1_szz> {
		auto result = String<size - 1_szz>{};
		for (auto index = 1_ixz; index < size; ++index) {
			result.data[index] = source[index];
		}
		return result;
	}

	#pragma endregion

}
