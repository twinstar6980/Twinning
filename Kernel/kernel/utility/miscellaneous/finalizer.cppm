module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.finalizer;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TAction> requires
		CategoryConstraint<IsPureInstance<TAction>>
		&& (IsInvocable<TAction>)
	class Finalizer {

	public:

		using Action = TAction;

	protected:

		Action m_action;

	public:

		#pragma region constructor

		constexpr ~Finalizer(
		) {
			thiz.m_action();
			return;
		}

		// ----------------

		constexpr Finalizer(
		) = delete;

		constexpr Finalizer(
			Finalizer const & that
		) = delete;

		constexpr Finalizer(
			Finalizer && that
		) = delete;

		// ----------------

		explicit constexpr Finalizer(
			Action const & action
		) :
			m_action{action} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			Finalizer const & that
		) -> Finalizer & = delete;

		constexpr auto operator =(
			Finalizer && that
		) -> Finalizer & = delete;

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename TFinalizer> requires
		CategoryConstraint<IsPureInstance<TFinalizer>>
		&& (IsInvocable<TFinalizer>)
	inline constexpr auto make_finalizer(
		TFinalizer const & finalizer
	) -> Finalizer<TFinalizer> {
		return Finalizer<TFinalizer>{finalizer};
	}

	template <typename TInitializer, typename TFinalizer> requires
		CategoryConstraint<IsPureInstance<TInitializer> && IsPureInstance<TFinalizer>>
		&& (IsInvocable<TInitializer> && IsInvocable<TFinalizer>)
	inline constexpr auto make_finalizer(
		TInitializer const & initializer,
		TFinalizer const &   finalizer
	) -> Finalizer<TFinalizer> {
		initializer();
		return Finalizer<TFinalizer>{finalizer};
	}

	#pragma endregion

}
