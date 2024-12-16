module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.package.value_package;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;
import twinning.kernel.utility.trait.template_instance;

export namespace Twinning::Kernel::Trait {

	#pragma region type

	template <auto ... t_element> requires
		CategoryConstraint<>
	struct ValuePackage {

		inline static constexpr auto size = ZSize{sizeof...(t_element)};

		template <auto index> requires
			CategoryConstraint<>
			&& (IsSameV<index, ZSize>)
			&& (index < size)
		using Element = AsSelect<index, decltype(t_element) ...>;

		template <auto index> requires
			CategoryConstraint<>
			&& (IsSameV<index, ZSize>)
			&& (index < size)
		inline static constexpr auto element =
			[] <auto ... element_index> (
			std::index_sequence<element_index ...>,
			auto ... element_value
		) -> auto {
				auto result = ValuePackage::Element<index>{};
				auto iterate =
					[&] (
					auto current_index,
					auto current_value
				) -> auto {
					if (current_index == index) {
						result = current_value;
					}
					return;
				};
				(iterate(element_index, element_value), ...);
				return result;
			}(std::make_index_sequence<size>{}, t_element ...);

		template <typename Value> requires
			CategoryConstraint<>
		inline static constexpr auto has (
			Value const & value
		) -> ZBoolean {
			auto result = false;
			auto iterate =
				[&] <typename CurrentValue> (
				CurrentValue current_value
			) {
				if constexpr (IsSame<CurrentValue, Value>) {
					if (current_value == value) {
						result = true;
					}
				}
			};
			(iterate(t_element), ...);
			return result;
		}

	};

	// ----------------

	template <typename It>
	concept IsValuePackage = IsTemplateInstanceOfNV<It, ValuePackage>;

	#pragma endregion

	#pragma region utility

	template <typename Package, auto begin, auto size> requires
		CategoryConstraint<IsPureInstance<Package>>
		&& (IsValuePackage<Package> && IsSameV<begin, ZSize> && IsSameV<size, ZSize>)
		&& (begin + size <= Package::size)
	using AsValuePackageSub = decltype(
		[] <ZSize ... index> (
		std::index_sequence<index ...>
	) -> ValuePackage<Package::template value<begin + index> ...> {
			return {};
		}(std::make_index_sequence<size>{})
	);

	template <typename Package, auto size> requires
		CategoryConstraint<IsPureInstance<Package>>
		&& (IsValuePackage<Package> && IsSameV<size, ZSize>)
		&& (size <= Package::size)
	using AsValuePackageRemoveHead = AsValuePackageSub<Package, size, Package::size - size>;

	template <typename Package, auto size> requires
		CategoryConstraint<IsPureInstance<Package>>
		&& (IsValuePackage<Package> && IsSameV<size, ZSize>)
		&& (size <= Package::size)
	using AsValuePackageRemoveTail = AsValuePackageSub<Package, 1_ixz, Package::size - size>;

	// ----------------

	template <typename Package1, typename Package2> requires
		CategoryConstraint<IsPureInstance<Package1> && IsPureInstance<Package2>>
		&& (IsValuePackage<Package1> && IsValuePackage<Package2>)
	using AsValuePackageConcat = decltype(
		[] <auto ... index_1, auto ... index_2> (
		std::index_sequence<index_1 ...>,
		std::index_sequence<index_2 ...>
	) -> ValuePackage<Package1::template value<index_1> ..., Package2::template value<index_2> ...> {
			return {};
		}(std::make_index_sequence<Package1::size>{}, std::make_index_sequence<Package2::size>{})
	);

	template <typename Package, auto ... element> requires
		CategoryConstraint<IsPureInstance<Package>>
		&& (IsValuePackage<Package>)
	using AsValuePackagePrepend = AsValuePackageConcat<ValuePackage<element ...>, Package>;

	template <typename Package, auto ... element> requires
		CategoryConstraint<IsPureInstance<Package>>
		&& (IsValuePackage<Package>)
	using AsValuePackageAppend = AsValuePackageConcat<Package, ValuePackage<element ...>>;

	// ----------------

	template <auto size> requires
		CategoryConstraint<>
		&& (IsSameV<size, ZSize>)
	using AsValuePackageOfIndex = decltype(
		[] <auto ... index> (
		std::index_sequence<index ...>
	) -> ValuePackage<index ...> {
			return {};
		}(std::make_index_sequence<size>{})
	);

	#pragma endregion

}
