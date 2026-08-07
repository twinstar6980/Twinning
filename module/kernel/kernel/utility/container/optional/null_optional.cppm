module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.optional.null_optional;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.optional.optional_view;

export namespace Twinning::Kernel {

	#pragma region type

	class NullOptional final {

	public:

		#pragma region operator

		template <typename TValue> requires
			AutomaticConstraint
		implicit constexpr operator Optional<TValue>() const {
			return Optional<TValue>{};
		}

		template <typename TValue, auto t_constant> requires
			AutomaticConstraint
		implicit constexpr operator OptionalView<TValue, t_constant>() const {
			return OptionalView<TValue, t_constant>{};
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			NullOptional const & thix,
			NullOptional const & that
		) -> bool = default;

		// ----------------

		template <typename TValue> requires
			AutomaticConstraint
		inline friend constexpr auto operator ==(
			Optional<TValue> const & thix,
			NullOptional const &     that
		) -> bool {
			return !thix.has();
		}

		template <typename TValue, auto t_constant> requires
			AutomaticConstraint
		inline friend constexpr auto operator ==(
			OptionalView<TValue, t_constant> const & thix,
			NullOptional const &                     that
		) -> bool {
			return !thix.has();
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_null_optional = NullOptional{};

	#pragma endregion

}
