module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.function;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TResult, typename ... TArgument> requires
		CategoryConstraint<IsAnything<TResult> && IsValid<TArgument ...>>
	class Function {

	public:

		using Result = TResult;

		using Argument = TypePackage<TArgument ...>;

	protected:

		std::function<TResult (TArgument ...)> m_value;

	public:

		#pragma region constructor

		~Function(
		) = default;

		// ----------------

		Function(
		) :
			m_value{} {
			return;
		}

		Function(
			Function const & that
		) = default;

		Function(
			Function && that
		) = default;

		// ----------------

		template <typename TCallable> requires
			CategoryConstraint<IsValid<TCallable>>
		explicit Function(
			TCallable && callable
		) :
			m_value{as_forward<TCallable>(callable)} {
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator =(
			Function const & that
		) -> Function & = default;

		auto operator =(
			Function && that
		) -> Function & = default;

		#pragma endregion

		#pragma region call

		auto call(
			TArgument ... argument
		) -> TResult {
			assert_test(thiz.m_value != nullptr);
			return thiz.m_value(as_forward<TArgument>(argument) ...);
		}

		#pragma endregion

	};

	#pragma endregion

}
