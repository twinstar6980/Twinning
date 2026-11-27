module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.basic_string_formatter;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_string_stream_view;
import twinning.kernel.utility.string.basic_static_string;
import twinning.kernel.utility.container.list;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsCharacterBox<TElement>)
	class BasicStringFormatter {

	public:

		using Element = TElement;

	protected:

		ConstantBasicStringView<Element> m_format;

	public:

		#pragma region constructor

		constexpr ~BasicStringFormatter(
		) = default;

		// ----------------

		constexpr BasicStringFormatter(
		) :
			m_format{} {
			return;
		}

		constexpr BasicStringFormatter(
			BasicStringFormatter const & that
		) = default;

		constexpr BasicStringFormatter(
			BasicStringFormatter && that
		) = default;

		// ----------------

		explicit constexpr BasicStringFormatter(
			ConstantBasicStringView<Element> const & format
		) :
			m_format{format} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			BasicStringFormatter const & that
		) -> BasicStringFormatter & = default;

		constexpr auto operator =(
			BasicStringFormatter && that
		) -> BasicStringFormatter & = default;

		// ----------------

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
		auto operator ()(
			TArgument && ... argument
		) const -> BasicString<Element> {
			return thiz.generate(as_forward<TArgument>(argument) ...);
		}

		#pragma endregion

		#pragma region usage

		auto format(
		) const -> ConstantBasicStringView<Element> const & {
			return thiz.m_format;
		}

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
		auto generate(
			TArgument && ... argument
		) const -> BasicString<Element> {
			auto format_stream = InputBasicStringStreamView<Element>{thiz.m_format};
			auto segment = List<BasicString<Element>>{};
			segment.allocate(thiz.m_format.size() / 2_sz);
			segment.append();
			segment.last().allocate(thiz.m_format.size());
			auto argument_index = 0_sz;
			while (!format_stream.full()) {
				auto first_character = format_stream.template read_of<Element>();
				if (first_character != cast_box<Element>('{'_c) && first_character != cast_box<Element>('}'_c)) {
					segment.last().append(first_character);
					continue;
				}
				auto second_character = format_stream.template read_of<Element>();
				if (second_character == first_character) {
					segment.last().append(first_character);
					continue;
				}
				if (first_character != cast_box<Element>('{'_c) || second_character != cast_box<Element>('}'_c)) {
					throw SyntaxException{format_stream.position().value, "invalid format string"s};
				}
				assert_test(argument_index < make_box<Size>(sizeof...(TArgument)));
				Generalization::each_with<>(
					[&]<auto t_index, typename TCurrentArgument>(
					ValuePackage<t_index>,
					TCurrentArgument && current_argument
				) {
						if (make_box<Size>(t_index) == argument_index) {
							segment.append(as_forward<TCurrentArgument>(current_argument));
						}
					},
					as_forward<TArgument>(argument) ...
				);
				++argument_index;
				segment.append();
				segment.last().allocate(thiz.m_format.size());
			}
			auto size = 0_sz;
			for (auto & element : segment) {
				size += element.size();
			}
			auto result = BasicString<Element>{};
			result.allocate(size);
			for (auto & element : segment) {
				result.append_range(element);
			}
			return result;
		}

		#pragma endregion

	};

	#pragma endregion

}
