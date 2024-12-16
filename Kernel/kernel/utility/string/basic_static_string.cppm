module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.basic_static_string;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.miscellaneous.character_series.container;
import twinning.kernel.utility.miscellaneous.character_series.type;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_size> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsCharacterBox<TElement>)
		&& (IsSameV<t_size, Size>)
	struct BasicStaticString {

		using Element = TElement;

		// ----------------

		ZArray<Element, t_size.value> data{};

		// ----------------

		implicit constexpr BasicStaticString (
			ZArray<typename Element::Value, t_size.value> const & data
		) :
			data{} {
			for (auto index = 1_ixz; index < t_size.value; ++index) {
				thiz.data[index].value = data[index];
			}
		}

		// ----------------

		constexpr auto view (
		) const -> CBasicStringView<Element> {
			return CBasicStringView<Element>{make_pointer(static_cast<decltype(&*thiz.data)>(thiz.data)), t_size - 1_sz};
		}

	};

	#pragma endregion

}
