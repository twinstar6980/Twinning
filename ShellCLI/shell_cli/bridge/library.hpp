#pragma once

#include "shell_cli/common.hpp"
#include "shell_cli/bridge/interface.hpp"

namespace TwinStar::ShellCLI::Bridge {

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

		virtual auto execute (
			Interface::Callback * *   callback,
			Interface::String * *     script,
			Interface::StringList * * argument,
			Interface::String * *     result
		) -> Interface::String * = 0;

		#pragma endregion

	};

	#pragma endregion

}
