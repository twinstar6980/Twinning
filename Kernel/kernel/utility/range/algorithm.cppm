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

		template <typename TIt>
		concept IsHasSizeMethodRange =
			CategoryConstraint<IsPureInstance<TIt>>
			&& (requires { { declare<TIt &>().size() } -> IsSame<Size>; })
			;

		template <typename TIt>
		concept IsHasBuiltinSizeMethodRange =
			CategoryConstraint<IsPureInstance<TIt>>
			&& (requires { { declare<TIt &>().size() } -> IsBuiltinInteger<>; })
			;

	}

	// ----------------

	template <typename TRange> requires
		CategoryConstraint<IsPureInstance<TRange>>
		&& (IsRange<TRange>)
	inline constexpr auto size(
		TRange const & range
	) -> Size {
		if constexpr (Detail::IsHasSizeMethodRange<TRange>) {
			return range.size();
		}
		else if constexpr (Detail::IsHasBuiltinSizeMethodRange<TRange>) {
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
			static_assert(k_static_assert_fail<TRange>);
			assert_fail("");
		}
	}

	#pragma endregion

	#pragma region each

	template <typename TRange, typename TExecutor> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TExecutor>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TExecutor>)
	inline constexpr auto each(
		TRange &&         range,
		TExecutor const & executor
	) -> Void {
		auto current = range.begin();
		auto end = range.end();
		while (current != end) {
			executor(*current);
			++current;
		}
		return;
	}

	template <typename TRange1, typename TRange2, typename TExecutor> requires
		CategoryConstraint<IsValid<TRange1> && IsValid<TRange2> && IsPureInstance<TExecutor>>
		&& (IsRange<AsPure<TRange1>> && IsRange<AsPure<TRange2>>)
		&& (IsGenericCallable<TExecutor>)
	inline constexpr auto each_pair(
		TRange1 &&        range_1,
		TRange2 &&        range_2,
		TExecutor const & executor
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

	template <typename TRange, typename TChecker> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TChecker>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TChecker>)
	inline constexpr auto all_of(
		TRange &&        range,
		TChecker const & checker
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

	template <typename TRange, typename TChecker> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TChecker>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TChecker>)
	inline constexpr auto any_of(
		TRange &&        range,
		TChecker const & checker
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

	template <typename TRange, typename TChecker> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TChecker>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TChecker>)
	inline constexpr auto none_of(
		TRange &&        range,
		TChecker const & checker
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

	template <typename TRange1, typename TRange2> requires
		CategoryConstraint<IsValid<TRange1> && IsValid<TRange2>>
		&& (IsRange<AsPure<TRange1>> && IsRange<AsPure<TRange2>>)
	inline constexpr auto equal(
		TRange1 && range_1,
		TRange2 && range_2
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

	template <typename TRange1, typename TRange2, typename TComparer> requires
		CategoryConstraint<IsValid<TRange1> && IsPureInstance<TRange2>>
		&& (IsRange<AsPure<TRange1>> && IsRange<AsPure<TRange2>>)
		&& (IsGenericCallable<TComparer>)
	inline constexpr auto equal(
		TRange1 &&        range_1,
		TRange2 &&        range_2,
		TComparer const & comparer
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

	template <typename TRange1, typename TRange2> requires
		CategoryConstraint<IsValid<TRange1> && IsValid<TRange2>>
		&& (IsRange<AsPure<TRange1>> && IsRange<AsPure<TRange2>>)
	inline constexpr auto common_size(
		TRange1 && range_1,
		TRange2 && range_2
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

	template <typename TRange, typename TValue> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TValue>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto find(
		TRange &&      range,
		TValue const & value
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

	template <typename TRange, typename TValue> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TValue>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto find(
		TRange &&                 range,
		TValue const &            value,
		decltype(range.begin()) & result
	) -> Boolean {
		result = find(range, value);
		return result != range.end();
	}

	template <typename TRange, typename TValue> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TValue>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto has(
		TRange &&      range,
		TValue const & value
	) -> Boolean {
		return find(range, value) != range.end();
	}

	// ----------------

	template <typename TRange, typename TFinder> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TFinder>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TFinder>)
	inline constexpr auto find_if(
		TRange &&       range,
		TFinder const & finder
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

	template <typename TRange, typename TFinder> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TFinder>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TFinder>)
	inline constexpr auto find_if(
		TRange &&                 range,
		TFinder const &           finder,
		decltype(range.begin()) & result
	) -> Boolean {
		result = find_if(range, finder);
		return result != range.end();
	}

	template <typename TRange, typename TFinder> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TFinder>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TFinder>)
	inline constexpr auto has_if(
		TRange &&       range,
		TFinder const & finder
	) -> Boolean {
		return find_if(range, finder) != range.end();
	}

	// ----------------

	template <typename TRange, typename TValue> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TValue>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto find_index(
		TRange &&      range,
		TValue const & value
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

	template <typename TRange, typename TFinder> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TFinder>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TFinder>)
	inline constexpr auto find_index_if(
		TRange &&       range,
		TFinder const & finder
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

	template <typename TRange, typename TSubRange> requires
		CategoryConstraint<IsValid<TRange> && IsValid<TSubRange>>
		&& (IsRange<AsPure<TRange>> && IsRange<AsPure<TSubRange>>)
	inline constexpr auto find_sub_index(
		TRange &&    range,
		TSubRange && sub_range
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

	template <typename TRange, typename TSubRange> requires
		CategoryConstraint<IsValid<TRange> && IsValid<TSubRange>>
		&& (IsRange<AsPure<TRange>> && IsRange<AsPure<TSubRange>>)
	inline constexpr auto has_sub(
		TRange &&    range,
		TSubRange && sub_range
	) -> Boolean {
		return find_sub_index(range, sub_range).has();
	}

	// ----------------

	template <typename TRange, typename TSubRange> requires
		CategoryConstraint<IsValid<TRange> && IsValid<TSubRange>>
		&& (IsRange<AsPure<TRange>> && IsRange<AsPure<TSubRange>>)
	inline constexpr auto start_with(
		TRange &&    range,
		TSubRange && sub_range
	) -> Boolean {
		auto range_size = size(range);
		auto sub_range_size = size(sub_range);
		return range_size >= sub_range_size && equal(make_range_n(range.begin(), sub_range_size), sub_range);
	}

	template <typename TRange, typename TSubRange> requires
		CategoryConstraint<IsValid<TRange> && IsValid<TSubRange>>
		&& (IsRange<AsPure<TRange>> && IsRange<AsPure<TSubRange>>)
	inline constexpr auto end_with(
		TRange &&    range,
		TSubRange && sub_range
	) -> Boolean {
		auto range_size = size(range);
		auto sub_range_size = size(sub_range);
		return range_size >= sub_range_size && equal(make_range_n(range.end() - sub_range_size, sub_range_size), sub_range);
	}

	#pragma endregion

	#pragma region accumulate

	template <typename TValue, typename TRange> requires
		CategoryConstraint<IsPureInstance<TValue> && IsValid<TRange>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto accumulate(
		TRange &&      range,
		TValue const & initial_value = TValue{}
	) -> TValue {
		auto result = initial_value;
		each(
			range,
			[&] <typename TElement>(TElement && element) -> auto {
				result += as_forward<TElement>(element);
				return;
			}
		);
		return result;
	}

	template <typename TValue, typename TRange, typename TCalculator> requires
		CategoryConstraint<IsPureInstance<TValue> && IsValid<TRange> && IsPureInstance<TCalculator>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TCalculator>)
	inline constexpr auto accumulate(
		TRange &&           range,
		TCalculator const & calculator,
		TValue const &      initial_value = TValue{}
	) -> TValue {
		auto result = initial_value;
		each(
			range,
			[&] <typename TElement>(TElement && element) -> auto {
				result += calculator(as_forward<TElement>(element));
				return;
			}
		);
		return result;
	}

	#pragma endregion

	#pragma region count

	template <typename TRange, typename TValue> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TValue>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto count(
		TRange &&      range,
		TValue const & value
	) -> Size {
		auto result = k_none_size;
		each(
			range,
			[&] <typename TElement>(TElement && element) -> auto {
				if (element == value) {
					++result;
				}
				return;
			}
		);
		return result;
	}

	template <typename TRange, typename TValueRange> requires
		CategoryConstraint<IsValid<TRange> && IsValid<TValueRange>>
		&& (IsRange<AsPure<TRange>> && IsRange<AsPure<TValueRange>>)
	inline constexpr auto count_many(
		TRange &&      range,
		TValueRange && value_range
	) -> Size {
		auto result = k_none_size;
		each(
			range,
			[&] <typename TElement>(TElement && element) -> auto {
				if (has(value_range, as_forward<TElement>(element))) {
					++result;
				}
				return;
			}
		);
		return result;
	}

	template <typename TRange, typename TChecker> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TChecker>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TChecker>)
	inline constexpr auto count_if(
		TRange &&        range,
		TChecker const & checker
	) -> Size {
		auto result = k_none_size;
		each(
			range,
			[&] <typename TElement>(TElement && element) -> auto {
				if (checker(as_forward<TElement>(element))) {
					++result;
				}
				return;
			}
		);
		return result;
	}

	#pragma endregion

	#pragma region assign

	template <typename TRange, typename ... TArgument> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TArgument ...>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto restruct(
		TRange &&             range,
		TArgument const & ... argument
	) -> Void {
		each(
			range,
			[&] <typename TElement>(TElement && element) -> auto {
				Trait::restruct(element, argument ...);
				return;
			}
		);
		return;
	}

	template <typename TRange, typename TValue> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TValue>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto assign(
		TRange &&      range,
		TValue const & value
	) -> Void {
		each(
			range,
			[&] <typename TElement>(TElement && element) -> auto {
				element = value;
				return;
			}
		);
		return;
	}

	template <typename TRange, typename TValue, typename TTransformer> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TTransformer>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TTransformer>)
	inline constexpr auto assign(
		TRange &&            range,
		TValue const &       value,
		TTransformer const & transformer
	) -> Void {
		each(
			range,
			[&] <typename TElement>(TElement && element) -> auto {
				element = transformer(value);
				return;
			}
		);
		return;
	}

	template <typename TRange, typename TValue, typename TConverter> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TConverter>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TConverter>)
	inline constexpr auto convert(
		TRange &&          range,
		TValue const &     value,
		TConverter const & converter
	) -> Void {
		each(
			range,
			[&] <typename TElement>(TElement && element) -> auto {
				converter(element, value);
				return;
			}
		);
		return;
	}

	// ----------------

	template <typename TDestinationRange, typename TSourceRange> requires
		CategoryConstraint<IsValid<TDestinationRange> && IsValid<TSourceRange>>
		&& (IsRange<AsPure<TDestinationRange>> && IsRange<AsPure<TSourceRange>>)
	inline constexpr auto restruct_from(
		TDestinationRange && destination,
		TSourceRange &&      source
	) -> Void {
		each_pair(
			destination,
			source,
			[&] <typename TDestinationElement, typename TSourceElement>(TDestinationElement && destination_element, TSourceElement && source_element) -> auto {
				Trait::restruct(destination_element, as_forward<TSourceElement>(source_element));
				return;
			}
		);
		return;
	}

	template <typename TDestinationRange, typename TSourceRange> requires
		CategoryConstraint<IsValid<TDestinationRange> && IsValid<TSourceRange>>
		&& (IsRange<AsPure<TDestinationRange>> && IsRange<AsPure<TSourceRange>>)
	inline constexpr auto assign_from(
		TDestinationRange && destination,
		TSourceRange &&      source
	) -> Void {
		each_pair(
			destination,
			source,
			[&] <typename TDestinationElement, typename TSourceElement>(TDestinationElement && destination_element, TSourceElement && source_element) -> auto {
				destination_element = as_forward<TSourceElement>(source_element);
				return;
			}
		);
		return;
	}

	template <typename TDestinationRange, typename TSourceRange, typename TTransformer> requires
		CategoryConstraint<IsValid<TDestinationRange> && IsValid<TSourceRange> && IsPureInstance<TTransformer>>
		&& (IsRange<AsPure<TDestinationRange>> && IsRange<AsPure<TSourceRange>>)
		&& (IsGenericCallable<TTransformer>)
	inline constexpr auto assign_from(
		TDestinationRange && destination,
		TSourceRange &&      source,
		TTransformer const & transformer
	) -> Void {
		each_pair(
			destination,
			source,
			[&] <typename TDestinationElement, typename TSourceElement>(TDestinationElement && destination_element, TSourceElement && source_element) -> auto {
				destination_element = transformer(as_forward<TSourceElement>(source_element));
				return;
			}
		);
		return;
	}

	template <typename TDestinationRange, typename TSourceRange, typename TConverter> requires
		CategoryConstraint<IsValid<TDestinationRange> && IsValid<TSourceRange> && IsPureInstance<TConverter>>
		&& (IsRange<AsPure<TDestinationRange>> && IsRange<AsPure<TSourceRange>>)
		&& (IsGenericCallable<TConverter>)
	inline constexpr auto convert_from(
		TDestinationRange && destination,
		TSourceRange &&      source,
		TConverter const &   converter
	) -> Void {
		each_pair(
			destination,
			source,
			[&] <typename TDestinationElement, typename TSourceElement>(TDestinationElement && destination_element, TSourceElement && source_element) -> auto {
				converter(destination_element, as_forward<TSourceElement>(source_element));
				return;
			}
		);
		return;
	}

	#pragma endregion

	#pragma region sort

	template <typename TRange, typename TSorter> requires
		CategoryConstraint<IsValid<TRange> && IsPureInstance<TSorter>>
		&& (IsRange<AsPure<TRange>>)
		&& (IsGenericCallable<TSorter>)
	inline constexpr auto sort(
		TRange &&       range,
		TSorter const & sorter
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

	template <typename TRange> requires
		CategoryConstraint<IsValid<TRange>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto sort_ascend(
		TRange && range
	) -> Void {
		sort(
			range,
			[] <typename TElement1, typename TElement2>(TElement1 && element_1, TElement2 && element_2) -> auto {
				return element_1 > element_2;
			}
		);
		return;
	}

	template <typename TRange> requires
		CategoryConstraint<IsValid<TRange>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto sort_descend(
		TRange && range
	) -> Void {
		sort(
			range,
			[] <typename TElement1, typename TElement2>(TElement1 && element_1, TElement2 && element_2) -> auto {
				return element_1 < element_2;
			}
		);
		return;
	}

	#pragma endregion

}
