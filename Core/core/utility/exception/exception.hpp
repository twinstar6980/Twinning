#pragma once

#include "core/utility/exception/source_location.hpp"
#include <string>

namespace TwinKleS::Core {

	#pragma region type

	class Exception {

	public: //

		#pragma region structor

		virtual ~Exception (
		) = default;

		// ----------------

		Exception (
		) = default;

		Exception (
			Exception const & that
		) = default;

		Exception (
			Exception && that
		) = default;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Exception const & that
		) -> Exception& = default;

		auto operator = (
			Exception && that
		) -> Exception& = default;

		#pragma endregion

		#pragma region what message

		virtual auto what (
		) const -> std::string_view = 0;

		#pragma endregion

	};

	#pragma endregion

}
