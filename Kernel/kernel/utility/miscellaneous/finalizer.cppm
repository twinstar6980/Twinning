module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.finalizer;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TFunction> requires
		CategoryConstraint<IsPureInstance<TFunction>>
		&& (IsInvocable<TFunction>)
	class Finalizer {

	public:

		using Function = TFunction;

	protected:

		Function m_function;

	public:

		#pragma region structor

		constexpr ~Finalizer (
		) {
			thiz.m_function();
		}

		// ----------------

		constexpr Finalizer (
		) = delete;

		constexpr Finalizer (
			Finalizer const & that
		) = delete;

		constexpr Finalizer (
			Finalizer && that
		) = delete;

		// ----------------

		explicit constexpr Finalizer (
			Function const & function
		) :
			m_function{function} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			Finalizer const & that
		) -> Finalizer & = delete;

		constexpr auto operator = (
			Finalizer && that
		) -> Finalizer & = delete;

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename FinalizerFunction> requires
		CategoryConstraint<IsPureInstance<FinalizerFunction>>
		&& (IsInvocable<FinalizerFunction>)
	inline constexpr auto make_finalizer (
		FinalizerFunction const & finalizer
	) -> Finalizer<FinalizerFunction> {
		return Finalizer<FinalizerFunction>{finalizer};
	}

	template <typename InitializerFunction, typename FinalizerFunction> requires
		CategoryConstraint<IsPureInstance<InitializerFunction> && IsPureInstance<FinalizerFunction>>
		&& (IsInvocable<InitializerFunction> && IsInvocable<FinalizerFunction>)
	inline constexpr auto make_finalizer (
		InitializerFunction const & initializer,
		FinalizerFunction const &   finalizer
	) -> Finalizer<FinalizerFunction> {
		initializer();
		return Finalizer<FinalizerFunction>{finalizer};
	}

	#pragma endregion

}
