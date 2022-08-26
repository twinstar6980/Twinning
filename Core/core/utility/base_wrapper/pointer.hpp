#pragma once

#include "core/utility/base_wrapper/base.hpp"
#include "core/utility/base_wrapper/boolean.hpp"
#include "core/utility/base_wrapper/number.hpp"

namespace TwinKleS::Core {

	#pragma region type

	template <typename TTarget> requires
		CategoryConstraint<IsPointable<TTarget>>
	class PointerWrapper :
		public BaseWrapper<ZPointer<TTarget>> {

	private: //

		using BaseWrapper = BaseWrapper<ZPointer<TTarget>>;

	public: //

		using Target = TTarget;

	public: //

		#pragma region structor

		constexpr ~PointerWrapper (
		) = default;

		// ----------------

		constexpr PointerWrapper (
		) = default;

		constexpr PointerWrapper (
			PointerWrapper const & that
		) = default;

		constexpr PointerWrapper (
			PointerWrapper && that
		) = default;

		// ----------------

		using BaseWrapper::BaseWrapper;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			PointerWrapper const & that
		) -> PointerWrapper& = default;

		constexpr auto operator = (
			PointerWrapper && that
		) -> PointerWrapper& = default;

		// ----------------

		template <typename TargetObject = Target> requires
			CategoryConstraint<IsInstance<TargetObject>>
		constexpr auto operator * (
		) const -> TargetObject& {
			return thiz.template dereference<TargetObject>();
		}

		template <typename TargetObject = Target> requires
			CategoryConstraint<IsInstance<TargetObject>>
		constexpr auto operator [] (
			Size const & index
		) const -> TargetObject& {
			return thiz.template dereference<TargetObject>(index);
		}

		// ----------------

		implicit operator PointerWrapper<Target const> & () requires
			(IsVInstance<Target>) {
			return self_cast<PointerWrapper<Target const>>(thiz);
		}

		implicit operator PointerWrapper<Target const> const & () const requires
			(IsVInstance<Target>) {
			return self_cast<PointerWrapper<Target const>>(thiz);
		}

		#pragma endregion

		#pragma region dereference

		template <typename TargetObject = Target> requires
			CategoryConstraint<IsInstance<TargetObject>>
		constexpr auto dereference (
		) const -> TargetObject& {
			return *static_cast<ZPointer<TargetObject>>(thiz.value);
		}

		template <typename TargetObject = Target> requires
			CategoryConstraint<IsInstance<TargetObject>>
		constexpr auto dereference (
			Size const & index
		) const -> TargetObject& {
			return *static_cast<ZPointer<TargetObject>>(thiz.value + index.value);
		}

		#pragma endregion

	};

	// ----------------

	template <typename It>
	concept IsPointerWrapper = IsTemplateInstanceOfT<It, PointerWrapper>;

	#pragma endregion

	#pragma region operator

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator == (
		It const & thix,
		It const & that
	) -> Boolean {
		return mbw<Boolean>(thix.value == that.value);
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator != (
		It const & thix,
		It const & that
	) -> Boolean {
		return !(thix == that);
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator < (
		It const & thix,
		It const & that
	) -> Boolean {
		return mbw<Boolean>(thix.value < that.value);
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator > (
		It const & thix,
		It const & that
	) -> Boolean {
		return mbw<Boolean>(thix.value > that.value);
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator <= (
		It const & thix,
		It const & that
	) -> Boolean {
		return mbw<Boolean>(thix.value <= that.value);
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator >= (
		It const & thix,
		It const & that
	) -> Boolean {
		return mbw<Boolean>(thix.value >= that.value);
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator + (
		It const &   thix,
		Size const & size
	) -> It {
		return It{thix.value + size.value};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator - (
		It const &   thix,
		Size const & size
	) -> It {
		return It{thix.value - size.value};
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator += (
		It &         thix,
		Size const & size
	) -> It& {
		thix.value += size;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator -= (
		It &         thix,
		Size const & size
	) -> It& {
		thix.value -= size;
		return thix;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator ++ (
		It & thix
	) -> It& {
		++thix.value;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator -- (
		It & thix
	) -> It& {
		--thix.value;
		return thix;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator ++ (
		It & thix,
		int  
	) -> It {
		return It{thix.value++};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator -- (
		It & thix,
		int  
	) -> It {
		return It{thix.value--};
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsPointerWrapper<It>)
	inline constexpr auto operator - (
		It const & thix,
		It const & that
	) -> SSize {
		return mbw<SSize>(thix.value - that.value);
	}

	#pragma endregion

	#pragma region utility

	template <typename Target> requires
		CategoryConstraint<IsPointable<Target>>
	inline constexpr auto make_pointer (
		ZPointer<Target> const & value
	) -> PointerWrapper<Target> {
		return PointerWrapper<Target>{value};
	}

	// ----------------

	template <typename DestTarget, typename SourceTarget> requires
		CategoryConstraint<IsPure<DestTarget> && IsPointable<SourceTarget>>
	inline auto cast_pointer (
		PointerWrapper<SourceTarget> const & source
	) -> PointerWrapper<AsConstantIf<DestTarget, IsCInstance<SourceTarget>>> {
		return self_cast<PointerWrapper<AsConstantIf<DestTarget, IsCInstance<SourceTarget>>>>(source);
	}

	// ----------------

	template <typename Target> requires
		CategoryConstraint<IsPointable<Target>>
	inline auto as_variable_pointer (
		PointerWrapper<Target> & it
	) -> PointerWrapper<AsUnmakeConstant<Target>>& {
		return self_cast<PointerWrapper<AsUnmakeConstant<Target>>>(it);
	}

	template <typename Target> requires
		CategoryConstraint<IsPointable<Target>>
	inline auto as_variable_pointer (
		PointerWrapper<Target> const & it
	) -> PointerWrapper<AsUnmakeConstant<Target>> const& {
		return self_cast<PointerWrapper<AsUnmakeConstant<Target>>>(it);
	}

	// ----------------

	template <typename Target> requires
		CategoryConstraint<IsPointable<Target>>
	inline auto as_constant_pointer (
		PointerWrapper<Target> & it
	) -> PointerWrapper<AsMakeConstant<Target>>& {
		return self_cast<PointerWrapper<AsMakeConstant<Target>>>(it);
	}

	template <typename Target> requires
		CategoryConstraint<IsPointable<Target>>
	inline auto as_constant_pointer (
		PointerWrapper<Target> const & it
	) -> PointerWrapper<AsMakeConstant<Target>> const& {
		return self_cast<PointerWrapper<AsMakeConstant<Target>>>(it);
	}

	#pragma endregion

	#pragma region alias

	template <typename Target> requires
		AutoConstraint
	using Pointer = PointerWrapper<Target>;

	#pragma endregion

}
