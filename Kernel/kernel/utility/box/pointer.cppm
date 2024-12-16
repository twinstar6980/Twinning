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

		#pragma region structor

		constexpr ~PointerBox (
		) = default;

		// ----------------

		constexpr PointerBox (
		) = default;

		constexpr PointerBox (
			PointerBox const & that
		) = default;

		constexpr PointerBox (
			PointerBox && that
		) = default;

		// ----------------

		using BaseBox::BaseBox;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			PointerBox const & that
		) -> PointerBox & = default;

		constexpr auto operator = (
			PointerBox && that
		) -> PointerBox & = default;

		// ----------------

		template <typename TargetObject = Target> requires
			CategoryConstraint<IsInstance<TargetObject>>
		constexpr auto operator * (
		) const -> TargetObject & {
			return thiz.template dereference<TargetObject>();
		}

		template <typename TargetObject = Target> requires
			CategoryConstraint<IsInstance<TargetObject>>
		constexpr auto operator [] (
			Size const & index
		) const -> TargetObject & {
			return thiz.template dereference<TargetObject>(index);
		}

		// ----------------

		implicit operator PointerBox<Target const> & () requires
			(IsVInstance<Target>) {
			return self_cast<PointerBox<Target const>>(thiz);
		}

		implicit operator PointerBox<Target const> const & () const requires
			(IsVInstance<Target>) {
			return self_cast<PointerBox<Target const>>(thiz);
		}

		#pragma endregion

		#pragma region dereference

		template <typename TargetObject = Target> requires
			CategoryConstraint<IsInstance<TargetObject>>
		constexpr auto dereference (
		) const -> TargetObject & {
			return *static_cast<ZPointer<TargetObject>>(thiz.value);
		}

		template <typename TargetObject = Target> requires
			CategoryConstraint<IsInstance<TargetObject>>
		constexpr auto dereference (
			Size const & index
		) const -> TargetObject & {
			return *static_cast<ZPointer<TargetObject>>(thiz.value + index.value);
		}

		#pragma endregion

	};

	// ----------------

	template <typename It>
	concept IsPointerBox = IsTemplateInstanceOfT<It, PointerBox>;

	#pragma endregion

	#pragma region operator

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator == (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value == that.value;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator < (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value < that.value;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator > (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value > that.value;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator <= (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value <= that.value;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator >= (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value >= that.value;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator + (
		It const &   thix,
		Size const & size
	) -> It {
		return It{thix.value + size.value};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator - (
		It const &   thix,
		Size const & size
	) -> It {
		return It{thix.value - size.value};
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator += (
		It &         thix,
		Size const & size
	) -> It & {
		thix.value += size;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator -= (
		It &         thix,
		Size const & size
	) -> It & {
		thix.value -= size;
		return thix;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator ++ (
		It & thix
	) -> It & {
		++thix.value;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator -- (
		It & thix
	) -> It & {
		--thix.value;
		return thix;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator ++ (
		It & thix,
		int
	) -> It {
		return It{thix.value++};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator -- (
		It & thix,
		int
	) -> It {
		return It{thix.value--};
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerBox<It>)
	inline constexpr auto operator - (
		It const & thix,
		It const & that
	) -> SSize {
		return mbox<SSize>(thix.value - that.value);
	}

	#pragma endregion

	#pragma region utility

	template <typename Target> requires
		CategoryConstraint<IsPointable<Target>>
	inline constexpr auto make_pointer (
		ZPointer<Target> const & value
	) -> PointerBox<Target> {
		return PointerBox<Target>{value};
	}

	// ----------------

	template <typename DestinationTarget, typename SourceTarget> requires
		CategoryConstraint<IsPure<DestinationTarget> && IsPointable<SourceTarget>>
	inline auto cast_pointer (
		PointerBox<SourceTarget> const & source
	) -> PointerBox<AsConstantIf<DestinationTarget, IsCInstance<SourceTarget>>> {
		return self_cast<PointerBox<AsConstantIf<DestinationTarget, IsCInstance<SourceTarget>>>>(source);
	}

	// ----------------

	template <typename Target> requires
		CategoryConstraint<IsPointable<Target>>
	inline auto as_variable_pointer (
		PointerBox<Target> & it
	) -> PointerBox<AsUnmakeConstant<Target>> & {
		return self_cast<PointerBox<AsUnmakeConstant<Target>>>(it);
	}

	template <typename Target> requires
		CategoryConstraint<IsPointable<Target>>
	inline auto as_variable_pointer (
		PointerBox<Target> const & it
	) -> PointerBox<AsUnmakeConstant<Target>> const & {
		return self_cast<PointerBox<AsUnmakeConstant<Target>>>(it);
	}

	// ----------------

	template <typename Target> requires
		CategoryConstraint<IsPointable<Target>>
	inline auto as_constant_pointer (
		PointerBox<Target> & it
	) -> PointerBox<AsMakeConstant<Target>> & {
		return self_cast<PointerBox<AsMakeConstant<Target>>>(it);
	}

	template <typename Target> requires
		CategoryConstraint<IsPointable<Target>>
	inline auto as_constant_pointer (
		PointerBox<Target> const & it
	) -> PointerBox<AsMakeConstant<Target>> const & {
		return self_cast<PointerBox<AsMakeConstant<Target>>>(it);
	}

	#pragma endregion

	#pragma region alias

	template <typename Target> requires
		AutoConstraint
	using Pointer = PointerBox<Target>;

	#pragma endregion

}
