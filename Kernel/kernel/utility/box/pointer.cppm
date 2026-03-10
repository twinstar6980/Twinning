module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.pointer;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box.base;
import twinning.kernel.utility.box.number;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	class PointerBox :
		public BaseBox<ZPointer<TTarget>> {

	private:

		using BaseBox = BaseBox<ZPointer<TTarget>>;

	public:

		using Target = TTarget;

	public:

		#pragma region constructor

		constexpr ~PointerBox(
		) = default;

		// ----------------

		constexpr PointerBox(
		) = default;

		constexpr PointerBox(
			PointerBox const & that
		) = default;

		constexpr PointerBox(
			PointerBox && that
		) = default;

		// ----------------

		using BaseBox::BaseBox;

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			PointerBox const & that
		) -> PointerBox & = default;

		constexpr auto operator =(
			PointerBox && that
		) -> PointerBox & = default;

		// ----------------

		template <typename TTargetObject = Target> requires
			CategoryConstraint<IsInstance<TTargetObject>>
		constexpr auto operator *(
		) const -> TTargetObject & {
			return thiz.template dereference<TTargetObject>();
		}

		template <typename TTargetObject = Target> requires
			CategoryConstraint<IsInstance<TTargetObject>>
		constexpr auto operator [](
			Size const & index
		) const -> TTargetObject & {
			return thiz.template dereference<TTargetObject>(index);
		}

		// ----------------

		implicit operator PointerBox<Target const> &() requires
			(IsVariableInstance<Target>) {
			return self_cast<PointerBox<Target const>>(thiz);
		}

		implicit operator PointerBox<Target const> const &() const requires
			(IsVariableInstance<Target>) {
			return self_cast<PointerBox<Target const>>(thiz);
		}

		#pragma endregion

		#pragma region dereference

		template <typename TTargetObject = Target> requires
			CategoryConstraint<IsInstance<TTargetObject>>
		constexpr auto dereference(
		) const -> TTargetObject & {
			return *static_cast<ZPointer<TTargetObject>>(thiz.value);
		}

		template <typename TTargetObject = Target> requires
			CategoryConstraint<IsInstance<TTargetObject>>
		constexpr auto dereference(
			Size const & index
		) const -> TTargetObject & {
			return *static_cast<ZPointer<TTargetObject>>(thiz.value + index.value);
		}

		#pragma endregion

	};

	// ----------------

	template <typename TIt>
	concept IsPointerBox = IsTemplateInstanceOfTt<TIt, PointerBox>;

	#pragma endregion

	#pragma region operator

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator ==(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value == that.value;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator <(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value < that.value;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator >(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value > that.value;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator <=(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value <= that.value;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator >=(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value >= that.value;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator +(
		TIt const &  thix,
		Size const & size
	) -> TIt {
		return TIt{thix.value + size.value};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator -(
		TIt const &  thix,
		Size const & size
	) -> TIt {
		return TIt{thix.value - size.value};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator +=(
		TIt &        thix,
		Size const & size
	) -> TIt & {
		thix.value += size;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator -=(
		TIt &        thix,
		Size const & size
	) -> TIt & {
		thix.value -= size;
		return thix;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator ++(
		TIt & thix
	) -> TIt & {
		++thix.value;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator --(
		TIt & thix
	) -> TIt & {
		--thix.value;
		return thix;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator ++(
		TIt & thix,
		int
	) -> TIt {
		return TIt{thix.value++};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator --(
		TIt & thix,
		int
	) -> TIt {
		return TIt{thix.value--};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsPointerBox<TIt>)
	inline constexpr auto operator -(
		TIt const & thix,
		TIt const & that
	) -> SSize {
		return mbox<SSize>(thix.value - that.value);
	}

	#pragma endregion

	#pragma region utility

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline constexpr auto make_pointer(
		ZPointer<TTarget> const & value
	) -> PointerBox<TTarget> {
		return PointerBox<TTarget>{value};
	}

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline constexpr auto make_pointer_of(
		TTarget & value
	) -> PointerBox<TTarget> {
		return PointerBox<TTarget>{&value};
	}

	// ----------------

	template <typename TDestinationTarget, typename TSourceTarget> requires
		CategoryConstraint<IsPure<TDestinationTarget> && IsPointable<TSourceTarget>>
	inline auto cast_pointer(
		PointerBox<TSourceTarget> const & source
	) -> PointerBox<AsConstantIf<TDestinationTarget, IsConstantInstance<TSourceTarget>>> {
		return self_cast<PointerBox<AsConstantIf<TDestinationTarget, IsConstantInstance<TSourceTarget>>>>(source);
	}

	// ----------------

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline auto as_variable_pointer(
		PointerBox<TTarget> & it
	) -> PointerBox<AsUnmakeConstant<TTarget>> & {
		return self_cast<PointerBox<AsUnmakeConstant<TTarget>>>(it);
	}

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline auto as_variable_pointer(
		PointerBox<TTarget> const & it
	) -> PointerBox<AsUnmakeConstant<TTarget>> const & {
		return self_cast<PointerBox<AsUnmakeConstant<TTarget>>>(it);
	}

	// ----------------

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline auto as_constant_pointer(
		PointerBox<TTarget> & it
	) -> PointerBox<AsMakeConstant<TTarget>> & {
		return self_cast<PointerBox<AsMakeConstant<TTarget>>>(it);
	}

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline auto as_constant_pointer(
		PointerBox<TTarget> const & it
	) -> PointerBox<AsMakeConstant<TTarget>> const & {
		return self_cast<PointerBox<AsMakeConstant<TTarget>>>(it);
	}

	#pragma endregion

	#pragma region alias

	template <typename TTarget> requires
		AutomaticConstraint
	using Pointer = PointerBox<TTarget>;

	#pragma endregion

}
