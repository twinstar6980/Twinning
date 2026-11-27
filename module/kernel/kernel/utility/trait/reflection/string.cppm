module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.reflection.string;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;

export namespace Twinning::Kernel::Trait::Reflection {

	#pragma region type

	template <auto t_size> requires
		CategoryConstraint<>
		&& (IsSameOf<t_size, ZSize>)
	struct String {

		ZArray<ZCharacter, t_size + 1_szz> container{};

		// ----------------

		implicit constexpr String(
			ZArray<ZCharacter, t_size + 1_szz> const & data
		) :
			container{} {
			for (auto index = 1_ixz; index < t_size; ++index) {
				thiz.container[index] = data[index];
			}
			return;
		}

		// ----------------

		constexpr auto data(
		) const -> ZConstantString {
			return thiz.container;
		}

		constexpr auto length(
		) const -> ZSize {
			return t_size;
		}

	};

	#pragma endregion

	#pragma region utility

	template <auto t_size> requires
		CategoryConstraint<>
		&& (IsSameOf<t_size, ZSize, ZSSize>) // NOTE: EXPNAIN: allow ZSSize to avoid ReSharper error hint
	inline constexpr auto make_string(
		ZArray<ZCharacter, t_size> const & source
	) -> String<t_size - 1_szz> {
		return String<t_size - 1_szz>{source};
	}

	#pragma endregion

}
