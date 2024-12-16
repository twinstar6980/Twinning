module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.package.type_package;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;
import twinning.kernel.utility.trait.template_instance;

export namespace Twinning::Kernel::Trait {

	#pragma region type

	template <typename ... TElement> requires
		CategoryConstraint<IsAnything<TElement ...>>
	struct TypePackage {

		inline static constexpr auto size = ZSize{sizeof...(TElement)};

		template <auto index> requires
			CategoryConstraint<>
			&& (IsSameV<index, ZSize>)
			&& (index < size)
		using Element = AsSelect<index, TElement ...>;

	};

	// ----------------

	template <typename It>
	concept IsTypePackage = IsTemplateInstanceOfNT<It, TypePackage>;

	#pragma endregion

	#pragma region utility

	template <typename Package, auto begin, auto size> requires
		CategoryConstraint<IsPureInstance<Package>>
		&& (IsTypePackage<Package> && IsSameV<begin, ZSize> && IsSameV<size, ZSize>)
		&& (begin + size <= Package::size)
	using AsTypePackageSub = decltype(
		[] <auto ... index> (
		std::index_sequence<index ...>
	) -> TypePackage<typename Package::template Element<begin + index> ...> {
			return {};
		}(std::make_index_sequence<size>{})
	);

	template <typename Package, auto size> requires
		CategoryConstraint<IsPureInstance<Package>>
		&& (IsTypePackage<Package> && IsSameV<size, ZSize>)
		&& (size <= Package::size)
	using AsTypePackageRemoveHead = AsTypePackageSub<Package, size, Package::size - size>;

	template <typename Package, auto size> requires
		CategoryConstraint<IsPureInstance<Package>>
		&& (IsTypePackage<Package> && IsSameV<size, ZSize>)
		&& (size <= Package::size)
	using AsTypePackageRemoveTail = AsTypePackageSub<Package, 1_ixz, Package::size - size>;

	// ----------------

	template <typename Package1, typename Package2> requires
		CategoryConstraint<IsPureInstance<Package1> && IsPureInstance<Package2>>
		&& (IsTypePackage<Package1> && IsTypePackage<Package2>)
	using AsTypePackageConcat = decltype(
		[] <auto ... index_1, auto ... index_2> (
		std::index_sequence<index_1 ...>,
		std::index_sequence<index_2 ...>
	) -> TypePackage<typename Package1::template Element<index_1> ..., typename Package2::template Element<index_2> ...> {
			return {};
		}(std::make_index_sequence<Package1::size>{}, std::make_index_sequence<Package2::size>{})
	);

	template <typename Package, typename ... Element> requires
		CategoryConstraint<IsPureInstance<Package> && IsAnything<Element ...>>
		&& (IsTypePackage<Package>)
	using AsTypePackagePrepend = AsTypePackageConcat<TypePackage<Element ...>, Package>;

	template <typename Package, typename ... Element> requires
		CategoryConstraint<IsPureInstance<Package> && IsAnything<Element ...>>
		&& (IsTypePackage<Package>)
	using AsTypePackageAppend = AsTypePackageConcat<Package, TypePackage<Element ...>>;

	// ----------------

	template <typename Package, template <typename ...> typename Transformer> requires
		CategoryConstraint<IsPureInstance<Package>>
	using AsTypePackageTransform = decltype([] {
		return [&] <auto ... index> (
			std::index_sequence<index ...>
		) -> Transformer<typename Package::template Element<index> ...> {
				return declare<Transformer<typename Package::template Element<index> ...>>();
			}(std::make_index_sequence<Package::size>{});
	}());

	#pragma endregion

}
