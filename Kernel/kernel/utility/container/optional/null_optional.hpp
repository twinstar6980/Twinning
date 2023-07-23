#pragma once

#include "kernel/utility/container/optional/optional.hpp"
#include "kernel/utility/container/optional/optional_view.hpp"

namespace TwinStar::Kernel {

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

		friend constexpr auto operator == (
			NullOptional const & thix,
			NullOptional const & that
		) -> bool = default;

		// ----------------

		template <typename Value> requires
			AutoConstraint
		friend constexpr auto operator == (
			Optional<Value> const & thix,
			NullOptional const &    that
		) -> bool {
			return !thix.has();
		}

		template <typename Value, auto constant> requires
			AutoConstraint
		friend constexpr auto operator == (
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
