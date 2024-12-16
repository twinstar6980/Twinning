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

		template <typename Value> requires
			AutoConstraint
		implicit constexpr operator Optional<Value> () const {
			return Optional<Value>{};
		}

		template <typename Value, auto constant> requires
			AutoConstraint
		implicit constexpr operator OptionalView<Value, constant> () const {
			return OptionalView<Value, constant>{};
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator == (
			NullOptional const & thix,
			NullOptional const & that
		) -> bool = default;

		// ----------------

		template <typename Value> requires
			AutoConstraint
		inline friend constexpr auto operator == (
			Optional<Value> const & thix,
			NullOptional const &    that
		) -> bool {
			return !thix.has();
		}

		template <typename Value, auto constant> requires
			AutoConstraint
		inline friend constexpr auto operator == (
			OptionalView<Value, constant> const & thix,
			NullOptional const &                  that
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
