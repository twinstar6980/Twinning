module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.basic_static_string;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.range.integer_range;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_size> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsCharacterBox<TElement>)
		&& (IsSameOf<t_size, ZSize>)
		&& (t_size > 0_szz)
	struct BasicStaticString {

		using Element = TElement;

		// ----------------

		ZArray<Element, t_size> container{};

		// ----------------

		implicit constexpr BasicStaticString(
			ZArray<typename Element::Value, t_size> const & data
		) :
			container{} {
			for (auto & index : SizeRange{make_box<Size>(t_size - 1_szz)}) {
				thiz.container[index.value].value = data[index.value];
			}
			return;
		}

		// ----------------

		constexpr auto view(
		) const -> ConstantBasicStringView<Element> {
			return ConstantBasicStringView<Element>{make_pointer(static_cast<decltype(&*thiz.container)>(thiz.container)), make_box<Size>(t_size - 1_szz)};
		}

	};

	#pragma endregion

}
