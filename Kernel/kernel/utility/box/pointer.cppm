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
			(IsVariable<Target>) {
			return unsafe_cast<PointerBox<Target const>>(thiz);
		}

		implicit operator PointerBox<Target const> const &() const requires
			(IsVariable<Target>) {
			return unsafe_cast<PointerBox<Target const>>(thiz);
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
		return make_box<SSize>(thix.value - that.value);
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

	template <typename TTarget, typename TOriginal> requires
		CategoryConstraint<IsPure<TTarget> && IsPointable<TOriginal>>
	inline auto make_pointer_unsafe(
		ZPointer<TOriginal> const & value
	) -> PointerBox<AsConstantIf<TTarget, IsConstant<TOriginal>>> {
		return PointerBox<AsConstantIf<TTarget, IsConstant<TOriginal>>>{reinterpret_cast<AsConstantIf<TTarget, IsConstant<TOriginal>> *>(value)};
	}

	// ----------------

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline constexpr auto unmake_pointer(
		PointerBox<TTarget> const & pointer
	) -> ZPointer<TTarget> {
		return pointer.value;
	}

	template <typename TTarget, typename TOriginal> requires
		CategoryConstraint<IsPure<TTarget> && IsPointable<TOriginal>>
	inline auto unmake_pointer_unsafe(
		PointerBox<TOriginal> const & pointer
	) -> ZPointer<AsConstantIf<TTarget, IsConstant<TOriginal>>> {
		return reinterpret_cast<AsConstantIf<TTarget, IsConstant<TOriginal>> *>(pointer.value);
	}

	// ----------------

	template <typename TDestination, typename TSource> requires
		CategoryConstraint<IsPure<TDestination> && IsPointable<TSource>>
	inline auto cast_pointer(
		PointerBox<TSource> const & source
	) -> PointerBox<AsConstantIf<TDestination, IsConstant<TSource>>> {
		return unsafe_cast<PointerBox<AsConstantIf<TDestination, IsConstant<TSource>>>>(source);
	}

	// ----------------

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline auto as_variable_pointer(
		PointerBox<TTarget> & it
	) -> PointerBox<AsUnmakeConstant<TTarget>> & {
		return unsafe_cast<PointerBox<AsUnmakeConstant<TTarget>>>(it);
	}

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline auto as_variable_pointer(
		PointerBox<TTarget> const & it
	) -> PointerBox<AsUnmakeConstant<TTarget>> const & {
		return unsafe_cast<PointerBox<AsUnmakeConstant<TTarget>>>(it);
	}

	// ----------------

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline auto as_constant_pointer(
		PointerBox<TTarget> & it
	) -> PointerBox<AsMakeConstant<TTarget>> & {
		return unsafe_cast<PointerBox<AsMakeConstant<TTarget>>>(it);
	}

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	inline auto as_constant_pointer(
		PointerBox<TTarget> const & it
	) -> PointerBox<AsMakeConstant<TTarget>> const & {
		return unsafe_cast<PointerBox<AsMakeConstant<TTarget>>>(it);
	}

	#pragma endregion

	#pragma region alias

	template <typename TTarget> requires
		AutomaticConstraint
	using Pointer = PointerBox<TTarget>;

	#pragma endregion

}
