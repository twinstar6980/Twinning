module;

#include "shell/common.hpp"

export module twinning.shell.bridge.client;

export namespace Twinning::Shell::Bridge {

	#pragma region type

	class Client {

	public:

		#pragma region structor

		virtual ~Client (
		) = default;

		// ----------------

		Client (
		) = default;

		Client (
			Client const & that
		) = delete;

		Client (
			Client && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Client const & that
		) -> Client & = delete;

		auto operator = (
			Client && that
		) -> Client & = delete;

		#pragma endregion

		#pragma region life

		virtual auto start (
		) -> void = 0;

		virtual auto finish (
		) -> void = 0;

		// ----------------

		virtual auto callback (
			std::vector<std::string> const & argument
		) -> std::vector<std::string> = 0;

		#pragma endregion

	};

	#pragma endregion

}
