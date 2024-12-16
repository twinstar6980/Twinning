module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.optional.null_optional;
import twinning.kernel.utility.range.number_range;
import twinning.kernel.utility.range.range_wrapper;

export namespace Twinning::Kernel::Range {

	#pragma region size

	namespace Detail {

		template <typename It>
		concept IsHasSizeMethodRange =
			CategoryConstraint<IsPureInstance<It>>
			&& (requires { { declare<It &>().size() } -> IsSame<Size>; })
			;

		template <typename It>
		concept IsHasBuiltinSizeMethodRange =
			CategoryConstraint<IsPureInstance<It>>
			&& (requires { { declare<It &>().size() } -> IsBuiltinInteger<>; })
			;

	}

	// ----------------

	template <typename Range> requires
		CategoryConstraint<IsPureInstance<Range>>
		&& (IsRange<Range>)
	inline constexpr auto size (
		Range const & range
	) -> Size {
		if constexpr (Detail::IsHasSizeMethodRange<Range>) {
			return range.size();
		}
		else if constexpr (Detail::IsHasBuiltinSizeMethodRange<Range>) {
			return mbox<Size>(range.size());
		}
		else if constexpr (IsSame<decltype(range.end() - range.begin()), Size>) {
			return range.end() - range.begin();
		}
		else if constexpr (IsIntegerBox<decltype(range.end() - range.begin())>) {
			return cbox<Size>(range.end() - range.begin());
		}
		else if constexpr (IsBuiltinInteger<decltype(range.end() - range.begin())>) {
			return mbox<Size>(range.end() - range.begin());
		}
		else {
			static_assert(k_static_assert_fail<Range>);
			assert_fail("");
		}
	}

	#pragma endregion

	#pragma region each

	template <typename Range, typename Executor> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Executor>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Executor>)
	inline constexpr auto each (
		Range &&         range,
		Executor const & executor
	) -> Void {
		auto current = range.begin();
		auto end = range.end();
		while (current != end) {
			executor(*current);
			++current;
		}
		return;
	}

	template <typename Range1, typename Range2, typename Executor> requires
		CategoryConstraint<IsValid<Range1> && IsValid<Range2> && IsPureInstance<Executor>>
		&& (IsRange<AsPure<Range1>> && IsRange<AsPure<Range2>>)
		&& (IsGenericCallable<Executor>)
	inline constexpr auto each_pair (
		Range1 &&        range_1,
		Range2 &&        range_2,
		Executor const & executor
	) -> Void {
		assert_test(size(range_1) == size(range_2));
		auto current_1 = range_1.begin();
		auto current_2 = range_2.begin();
		auto end_1 = range_1.end();
		while (current_1 != end_1) {
			executor(*current_1, *current_2);
			++current_1;
			++current_2;
		}
		return;
	}

	#pragma endregion

	#pragma region check

	template <typename Range, typename Checker> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Checker>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Checker>)
	inline constexpr auto all_of (
		Range &&        range,
		Checker const & checker
	) -> Boolean {
		auto current = range.begin();
		auto end = range.end();
		while (current != end) {
			if (!checker(*current)) {
				return k_false;
			}
			++current;
		}
		return k_true;
	}

	template <typename Range, typename Checker> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Checker>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Checker>)
	inline constexpr auto any_of (
		Range &&        range,
		Checker const & checker
	) -> Boolean {
		auto current = range.begin();
		auto end = range.end();
		while (current != end) {
			if (checker(*current)) {
				return k_true;
			}
			++current;
		}
		return k_false;
	}

	template <typename Range, typename Checker> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Checker>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Checker>)
	inline constexpr auto none_of (
		Range &&        range,
		Checker const & checker
	) -> Boolean {
		auto current = range.begin();
		auto end = range.end();
		while (current != end) {
			if (checker(*current)) {
				return k_false;
			}
			++current;
		}
		return k_true;
	}

	#pragma endregion

	#pragma region comparison

	template <typename Range1, typename Range2> requires
		CategoryConstraint<IsValid<Range1> && IsValid<Range2>>
		&& (IsRange<AsPure<Range1>> && IsRange<AsPure<Range2>>)
	inline constexpr auto equal (
		Range1 && range_1,
		Range2 && range_2
	) -> Boolean {
		if (size(range_1) != size(range_2)) {
			return k_false;
		}
		auto current_1 = range_1.begin();
		auto current_2 = range_2.begin();
		auto end_1 = range_1.end();
		while (current_1 != end_1) {
			if (*current_1 != *current_2) {
				return k_false;
			}
			++current_1;
			++current_2;
		}
		return k_true;
	}

	template <typename Range1, typename Range2, typename Comparer> requires
		CategoryConstraint<IsValid<Range1> && IsPureInstance<Range2>>
		&& (IsRange<AsPure<Range1>> && IsRange<AsPure<Range2>>)
		&& (IsGenericCallable<Comparer>)
	inline constexpr auto equal (
		Range1 &&        range_1,
		Range2 &&        range_2,
		Comparer const & comparer
	) -> Boolean {
		if (size(range_1) != size(range_2)) {
			return k_false;
		}
		auto current_1 = range_1.begin();
		auto current_2 = range_2.begin();
		auto end_1 = range_1.end();
		while (current_1 != end_1) {
			if (!comparer(*current_1, *current_2)) {
				return k_false;
			}
			++current_1;
			++current_2;
		}
		return k_true;
	}

	// ----------------

	template <typename Range1, typename Range2> requires
		CategoryConstraint<IsValid<Range1> && IsValid<Range2>>
		&& (IsRange<AsPure<Range1>> && IsRange<AsPure<Range2>>)
	inline constexpr auto common_size (
		Range1 && range_1,
		Range2 && range_2
	) -> Size {
		auto maximum_common_size = minimum(size(range_1), size(range_2));
		auto size = k_none_size;
		auto current_1 = range_1.begin();
		auto current_2 = range_2.begin();
		auto end_1 = range_1.begin() + maximum_common_size;
		while (current_1 != end_1) {
			if (*current_1 != *current_2) {
				return size;
			}
			++current_1;
			++current_2;
			++size;
		}
		return maximum_common_size;
	}

	#pragma endregion

	#pragma region find

	template <typename Range, typename Value> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Value>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto find (
		Range &&      range,
		Value const & value
	) -> decltype(range.begin()) {
		auto current = range.begin();
		auto end = range.end();
		while (current != end) {
			if (*current == value) {
				break;
			}
			++current;
		}
		return current;
	}

	template <typename Range, typename Value> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Value>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto find (
		Range &&                  range,
		Value const &             value,
		decltype(range.begin()) & result
	) -> Boolean {
		result = find(range, value);
		return result != range.end();
	}

	template <typename Range, typename Value> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Value>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto has (
		Range &&      range,
		Value const & value
	) -> Boolean {
		return find(range, value) != range.end();
	}

	// ----------------

	template <typename Range, typename Finder> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Finder>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Finder>)
	inline constexpr auto find_if (
		Range &&       range,
		Finder const & finder
	) -> decltype(range.begin()) {
		auto current = range.begin();
		auto end = range.end();
		while (current != end) {
			if (finder(*current)) {
				break;
			}
			++current;
		}
		return current;
	}

	template <typename Range, typename Finder> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Finder>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Finder>)
	inline constexpr auto find_if (
		Range &&                  range,
		Finder const &            finder,
		decltype(range.begin()) & result
	) -> Boolean {
		result = find_if(range, finder);
		return result != range.end();
	}

	template <typename Range, typename Finder> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Finder>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Finder>)
	inline constexpr auto has_if (
		Range &&       range,
		Finder const & finder
	) -> Boolean {
		return find_if(range, finder) != range.end();
	}

	// ----------------

	template <typename Range, typename Value> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Value>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto find_index (
		Range &&      range,
		Value const & value
	) -> Optional<Size> {
		auto index = k_begin_index;
		auto current = range.begin();
		auto end = range.end();
		while (current != end) {
			if (*current == value) {
				return make_optional_of(index);
			}
			++current;
			++index;
		}
		return k_null_optional;
	}

	template <typename Range, typename Finder> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Finder>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Finder>)
	inline constexpr auto find_index_if (
		Range &&       range,
		Finder const & finder
	) -> Optional<Size> {
		auto index = k_begin_index;
		auto current = range.begin();
		auto end = range.end();
		while (current != end) {
			if (finder(*current)) {
				return make_optional_of(index);
			}
			++current;
			++index;
		}
		return k_null_optional;
	}

	#pragma endregion

	#pragma region find sub

	template <typename Range, typename SubRange> requires
		CategoryConstraint<IsValid<Range> && IsValid<SubRange>>
		&& (IsRange<AsPure<Range>> && IsRange<AsPure<SubRange>>)
	inline constexpr auto find_sub_index (
		Range &&    range,
		SubRange && sub_range
	) -> Optional<Size> {
		auto range_size = size(range);
		auto sub_range_size = size(sub_range);
		if (range_size >= sub_range_size) {
			auto index = k_begin_index;
			auto current = range.begin();
			auto end = range.begin() + (range_size - sub_range_size + k_next_index);
			while (current != end) {
				if (equal(make_range_n(current, sub_range_size), sub_range)) {
					return make_optional_of(index);
				}
				++current;
				++index;
			}
		}
		return k_null_optional;
	}

	template <typename Range, typename SubRange> requires
		CategoryConstraint<IsValid<Range> && IsValid<SubRange>>
		&& (IsRange<AsPure<Range>> && IsRange<AsPure<SubRange>>)
	inline constexpr auto has_sub (
		Range &&    range,
		SubRange && sub_range
	) -> Boolean {
		return find_sub_index(range, sub_range).has();
	}

	// ----------------

	template <typename Range, typename SubRange> requires
		CategoryConstraint<IsValid<Range> && IsValid<SubRange>>
		&& (IsRange<AsPure<Range>> && IsRange<AsPure<SubRange>>)
	inline constexpr auto start_with (
		Range &&    range,
		SubRange && sub_range
	) -> Boolean {
		auto range_size = size(range);
		auto sub_range_size = size(sub_range);
		return range_size >= sub_range_size && equal(make_range_n(range.begin(), sub_range_size), sub_range);
	}

	template <typename Range, typename SubRange> requires
		CategoryConstraint<IsValid<Range> && IsValid<SubRange>>
		&& (IsRange<AsPure<Range>> && IsRange<AsPure<SubRange>>)
	inline constexpr auto end_with (
		Range &&    range,
		SubRange && sub_range
	) -> Boolean {
		auto range_size = size(range);
		auto sub_range_size = size(sub_range);
		return range_size >= sub_range_size && equal(make_range_n(range.end() - sub_range_size, sub_range_size), sub_range);
	}

	#pragma endregion

	#pragma region accumulate

	template <typename Value, typename Range> requires
		CategoryConstraint<IsPureInstance<Value> && IsValid<Range>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto accumulate (
		Range &&      range,
		Value const & initial_value = Value{}
	) -> Value {
		auto result = initial_value;
		each(
			range,
			[&] <typename Element> (Element && element) -> auto {
				result += as_forward<Element>(element);
				return;
			}
		);
		return result;
	}

	template <typename Value, typename Range, typename Calculator> requires
		CategoryConstraint<IsPureInstance<Value> && IsValid<Range> && IsPureInstance<Calculator>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Calculator>)
	inline constexpr auto accumulate (
		Range &&           range,
		Calculator const & calculator,
		Value const &      initial_value = Value{}
	) -> Value {
		auto result = initial_value;
		each(
			range,
			[&] <typename Element> (Element && element) -> auto {
				result += calculator(as_forward<Element>(element));
				return;
			}
		);
		return result;
	}

	#pragma endregion

	#pragma region count

	template <typename Range, typename Value> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Value>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto count (
		Range &&      range,
		Value const & value
	) -> Size {
		auto result = k_none_size;
		each(
			range,
			[&] <typename Element> (Element && element) -> auto {
				if (element == value) {
					++result;
				}
				return;
			}
		);
		return result;
	}

	template <typename Range, typename ValueRange> requires
		CategoryConstraint<IsValid<Range> && IsValid<ValueRange>>
		&& (IsRange<AsPure<Range>> && IsRange<AsPure<ValueRange>>)
	inline constexpr auto count_many (
		Range &&      range,
		ValueRange && value_range
	) -> Size {
		auto result = k_none_size;
		each(
			range,
			[&] <typename Element> (Element && element) -> auto {
				if (has(value_range, as_forward<Element>(element))) {
					++result;
				}
				return;
			}
		);
		return result;
	}

	template <typename Range, typename Checker> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Checker>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Checker>)
	inline constexpr auto count_if (
		Range &&        range,
		Checker const & checker
	) -> Size {
		auto result = k_none_size;
		each(
			range,
			[&] <typename Element> (Element && element) -> auto {
				if (checker(as_forward<Element>(element))) {
					++result;
				}
				return;
			}
		);
		return result;
	}

	#pragma endregion

	#pragma region assign

	template <typename Range, typename ... Argument> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Argument ...>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto restruct (
		Range &&             range,
		Argument const & ... argument
	) -> Void {
		each(
			range,
			[&] <typename Element> (Element && element) -> auto {
				Trait::restruct(element, argument ...);
				return;
			}
		);
		return;
	}

	template <typename Range, typename Value> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Value>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto assign (
		Range &&      range,
		Value const & value
	) -> Void {
		each(
			range,
			[&] <typename Element> (Element && element) -> auto {
				element = value;
				return;
			}
		);
		return;
	}

	template <typename Range, typename Value, typename Transformer> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Transformer>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Transformer>)
	inline constexpr auto assign (
		Range &&            range,
		Value const &       value,
		Transformer const & transformer
	) -> Void {
		each(
			range,
			[&] <typename Element> (Element && element) -> auto {
				element = transformer(value);
				return;
			}
		);
		return;
	}

	template <typename Range, typename Value, typename Converter> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Converter>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Converter>)
	inline constexpr auto convert (
		Range &&          range,
		Value const &     value,
		Converter const & converter
	) -> Void {
		each(
			range,
			[&] <typename Element> (Element && element) -> auto {
				converter(element, value);
				return;
			}
		);
		return;
	}

	// ----------------

	template <typename DestinationRange, typename SourceRange> requires
		CategoryConstraint<IsValid<DestinationRange> && IsValid<SourceRange>>
		&& (IsRange<AsPure<DestinationRange>> && IsRange<AsPure<SourceRange>>)
	inline constexpr auto restruct_from (
		DestinationRange && destination,
		SourceRange &&      source
	) -> Void {
		each_pair(
			destination,
			source,
			[&] <typename DestinationElement, typename SourceElement> (DestinationElement && destination_element, SourceElement && source_element) -> auto {
				Trait::restruct(destination_element, as_forward<SourceElement>(source_element));
				return;
			}
		);
		return;
	}

	template <typename DestinationRange, typename SourceRange> requires
		CategoryConstraint<IsValid<DestinationRange> && IsValid<SourceRange>>
		&& (IsRange<AsPure<DestinationRange>> && IsRange<AsPure<SourceRange>>)
	inline constexpr auto assign_from (
		DestinationRange && destination,
		SourceRange &&      source
	) -> Void {
		each_pair(
			destination,
			source,
			[&] <typename DestinationElement, typename SourceElement> (DestinationElement && destination_element, SourceElement && source_element) -> auto {
				destination_element = as_forward<SourceElement>(source_element);
				return;
			}
		);
		return;
	}

	template <typename DestinationRange, typename SourceRange, typename Transformer> requires
		CategoryConstraint<IsValid<DestinationRange> && IsValid<SourceRange> && IsPureInstance<Transformer>>
		&& (IsRange<AsPure<DestinationRange>> && IsRange<AsPure<SourceRange>>)
		&& (IsGenericCallable<Transformer>)
	inline constexpr auto assign_from (
		DestinationRange && destination,
		SourceRange &&      source,
		Transformer const & transformer
	) -> Void {
		each_pair(
			destination,
			source,
			[&] <typename DestinationElement, typename SourceElement> (DestinationElement && destination_element, SourceElement && source_element) -> auto {
				destination_element = transformer(as_forward<SourceElement>(source_element));
				return;
			}
		);
		return;
	}

	template <typename DestinationRange, typename SourceRange, typename Converter> requires
		CategoryConstraint<IsValid<DestinationRange> && IsValid<SourceRange> && IsPureInstance<Converter>>
		&& (IsRange<AsPure<DestinationRange>> && IsRange<AsPure<SourceRange>>)
		&& (IsGenericCallable<Converter>)
	inline constexpr auto convert_from (
		DestinationRange && destination,
		SourceRange &&      source,
		Converter const &   converter
	) -> Void {
		each_pair(
			destination,
			source,
			[&] <typename DestinationElement, typename SourceElement> (DestinationElement && destination_element, SourceElement && source_element) -> auto {
				converter(destination_element, as_forward<SourceElement>(source_element));
				return;
			}
		);
		return;
	}

	#pragma endregion

	#pragma region sort

	template <typename Range, typename Sorter> requires
		CategoryConstraint<IsValid<Range> && IsPureInstance<Sorter>>
		&& (IsRange<AsPure<Range>>)
		&& (IsGenericCallable<Sorter>)
	inline constexpr auto sort (
		Range &&       range,
		Sorter const & sorter
	) -> Void {
		for (auto current_1 = range.begin(); current_1 != range.end(); ++current_1) {
			for (auto current_2 = current_1 + k_next_index; current_2 != range.end(); ++current_2) {
				if (sorter(*current_1, *current_2)) {
					swap(*current_1, *current_2);
				}
			}
		}
		return;
	}

	// ----------------

	template <typename Range> requires
		CategoryConstraint<IsValid<Range>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto sort_ascend (
		Range && range
	) -> Void {
		return sort(
			range,
			[] <typename Element1, typename Element2> (Element1 && element_1, Element2 && element_2) -> auto {
				return element_1 > element_2;
			}
		);
		return;
	}

	template <typename Range> requires
		CategoryConstraint<IsValid<Range>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto sort_descend (
		Range && range
	) -> Void {
		return sort(
			range,
			[] <typename Element1, typename Element2> (Element1 && element_1, Element2 && element_2) -> auto {
				return element_1 < element_2;
			}
		);
		return;
	}

	#pragma endregion

}
