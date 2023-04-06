#pragma once

#include "shell/common.hpp"

namespace TwinStar::Shell {

	#pragma region type

	class Host {

	public:

		#pragma region structor

		virtual ~Host (
		) = default;

		// ----------------

		Host (
		) = default;

		Host (
			Host const & that
		) = delete;

		Host (
			Host && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Host const & that
		) -> Host & = delete;

		auto operator = (
			Host && that
		) -> Host & = delete;

		#pragma endregion

		#pragma region function

		virtual auto start (
		) -> void = 0;

		virtual auto finish (
		) -> void = 0;

		// ----------------

		virtual auto execute (
			std::vector<std::string> const & argument
		) -> std::vector<std::string> = 0;

		#pragma endregion

	};

	#pragma endregion

}
