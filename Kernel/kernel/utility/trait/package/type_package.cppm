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

		template <auto t_index> requires
			CategoryConstraint<>
			&& (IsSameOf<t_index, ZSize>)
			&& (t_index < size)
		using Element = AsSelect<t_index, TElement ...>;

	};

	// ----------------

	template <typename TIt>
	concept IsTypePackage = IsTemplateInstanceOfTnt<TIt, TypePackage>;

	#pragma endregion

	#pragma region utility

	template <typename TPackage, auto t_begin, auto t_size> requires
		CategoryConstraint<IsPureInstance<TPackage>>
		&& (IsTypePackage<TPackage> && IsSameOf<t_begin, ZSize> && IsSameOf<t_size, ZSize>)
		&& (t_begin + t_size <= TPackage::size)
	using AsTypePackageSub = decltype(
		[] <auto ... t_index>(
		std::index_sequence<t_index ...>
	) -> TypePackage<typename TPackage::template Element<t_begin + t_index> ...> {
			return {};
		}(std::make_index_sequence<t_size>{})
	);

	template <typename TPackage, auto t_size> requires
		CategoryConstraint<IsPureInstance<TPackage>>
		&& (IsTypePackage<TPackage> && IsSameOf<t_size, ZSize>)
		&& (t_size <= TPackage::size)
	using AsTypePackageRemoveHead = AsTypePackageSub<TPackage, t_size, TPackage::size - t_size>;

	template <typename TPackage, auto t_size> requires
		CategoryConstraint<IsPureInstance<TPackage>>
		&& (IsTypePackage<TPackage> && IsSameOf<t_size, ZSize>)
		&& (t_size <= TPackage::size)
	using AsTypePackageRemoveTail = AsTypePackageSub<TPackage, 1_ixz, TPackage::size - t_size>;

	// ----------------

	template <typename TPackage1, typename TPackage2> requires
		CategoryConstraint<IsPureInstance<TPackage1> && IsPureInstance<TPackage2>>
		&& (IsTypePackage<TPackage1> && IsTypePackage<TPackage2>)
	using AsTypePackageConcat = decltype(
		[] <auto ... t_index_1, auto ... t_index_2>(
		std::index_sequence<t_index_1 ...>,
		std::index_sequence<t_index_2 ...>
	) -> TypePackage<typename TPackage1::template Element<t_index_1> ..., typename TPackage2::template Element<t_index_2> ...> {
			return {};
		}(std::make_index_sequence<TPackage1::size>{}, std::make_index_sequence<TPackage2::size>{})
	);

	template <typename TPackage, typename ... TElement> requires
		CategoryConstraint<IsPureInstance<TPackage> && IsAnything<TElement ...>>
		&& (IsTypePackage<TPackage>)
	using AsTypePackagePrepend = AsTypePackageConcat<TypePackage<TElement ...>, TPackage>;

	template <typename TPackage, typename ... TElement> requires
		CategoryConstraint<IsPureInstance<TPackage> && IsAnything<TElement ...>>
		&& (IsTypePackage<TPackage>)
	using AsTypePackageAppend = AsTypePackageConcat<TPackage, TypePackage<TElement ...>>;

	// ----------------

	template <typename TPackage, template <typename ...> typename TTransformer> requires
		CategoryConstraint<IsPureInstance<TPackage>>
	using AsTypePackageTransform = decltype([] {
		return [&]<auto ... t_index>(
			std::index_sequence<t_index ...>
		) -> TTransformer<typename TPackage::template Element<t_index> ...> {
				return declare<TTransformer<typename TPackage::template Element<t_index> ...>>();
			}(std::make_index_sequence<TPackage::size>{});
	}());

	#pragma endregion

}
