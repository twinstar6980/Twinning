#pragma once

#include "shell/common.hpp"
#include "shell/core/library.hpp"
#include "shell/core/invoker.hpp"

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
		) -> Host& = delete;

		auto operator = (
			Host && that
		) -> Host& = delete;

		#pragma endregion

		#pragma region life

		virtual auto start (
		) -> void = 0;

		virtual auto finish (
		) -> void = 0;

		#pragma endregion

		#pragma region callback

		virtual auto callback (
			std::vector<std::string> const & argument
		) -> std::vector<std::string> = 0;

		#pragma endregion

	};

	#pragma endregion

	#pragma region function

	inline auto launch (
		Host &                           host,
		Core::Library &                  library,
		std::string const &              script,
		std::vector<std::string> const & argument
	) -> std::optional<std::string> {
		auto prepare_result = Core::Invoker::prepare(library);
		if (prepare_result.has_value()) {
			output("prepare failed : "s + prepare_result.value());
		}
		host.start();
		auto callback = std::bind(&Host::callback, &host, std::placeholders::_1);
		auto result = Core::Invoker::execute(library, callback, script, argument);
		host.finish();
		return result;
	}

	#pragma endregion

}
