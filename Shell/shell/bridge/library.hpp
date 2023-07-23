#pragma once

#include "shell/common.hpp"
#include "shell/bridge/interface.hpp"

namespace TwinStar::Shell::Bridge {

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
		) -> Library & = delete;

		auto operator = (
			Library && that
		) -> Library & = delete;

		#pragma endregion

		#pragma region interface

		virtual auto version (
			Interface::Size * * number
		) -> Interface::String * = 0;

		virtual auto execute (
			Interface::Callback * *   callback,
			Interface::String * *     script,
			Interface::StringList * * argument,
			Interface::String * *     result
		) -> Interface::String * = 0;

		virtual auto prepare (
		) -> Interface::String * = 0;

		#pragma endregion

	};

	#pragma endregion

}
