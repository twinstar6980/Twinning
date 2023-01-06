#pragma once

#include "core/utility/exception/source_location.hpp"
#include <string>

namespace TwinStar::Core {

	#pragma region type

	class SimpleException :
		public Exception {

	protected:

		std::string m_message{};

	public:

		#pragma region structor

		virtual ~SimpleException (
		) override = default;

		// ----------------

		SimpleException (
		) = default;

		SimpleException (
			SimpleException const & that
		) = default;

		SimpleException (
			SimpleException && that
		) = default;

		// ----------------

		explicit SimpleException (
			std::string_view const & message
		) :
			m_message{message} {
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			SimpleException const & that
		) -> SimpleException& = default;

		auto operator = (
			SimpleException && that
		) -> SimpleException& = default;

		#pragma endregion

		#pragma region what message

		virtual auto what (
		) const -> std::string_view override {
			return thiz.m_message.operator std::string_view();
		}

		#pragma endregion

	};

	#pragma endregion

}
