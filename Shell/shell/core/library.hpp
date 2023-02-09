#pragma once

#include "shell/common.hpp"
#include "shell/core/interface.hpp"

namespace TwinStar::Shell::Core {

	#pragma region type

	class Library {

	public:

		#pragma region structor

		virtual ~Library (
		) = default;

		// ----------------

		Library (
		) = default;

		Library (
			Library const & that
		) = delete;

		Library (
			Library && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Library const & that
		) -> Library& = delete;

		auto operator = (
			Library && that
		) -> Library& = delete;

		#pragma endregion

		#pragma region interface

		virtual auto version (
		) -> Interface::Size const* = 0;

		virtual auto execute (
			Interface::Callback const *   callback,
			Interface::String const *     script,
			Interface::StringList const * argument
		) -> Interface::String const* = 0;

		virtual auto prepare (
		) -> Interface::String const* = 0;

		#pragma endregion

	};

	#pragma endregion

}
