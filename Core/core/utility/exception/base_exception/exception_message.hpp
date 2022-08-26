#pragma once

#include "core/utility/exception/exception.hpp"
#include "core/utility/container/list/list.hpp"
#include "core/utility/string/string.hpp"

namespace TwinKleS::Core {

	#pragma region type

	class ExceptionMessage {

	protected: //

		SourceLocation m_location{};
		List<String>   m_message{};

	public: //

		#pragma region structor

		~ExceptionMessage (
		) = default;

		// ----------------

		ExceptionMessage (
		) = default;

		ExceptionMessage (
			ExceptionMessage const & that
		) = default;

		ExceptionMessage (
			ExceptionMessage && that
		) = default;

		// ----------------

		implicit ExceptionMessage (
			std::initializer_list<CStringView> const & message,
			std_source_location const &                location = M_current_source_location_x
		) :
			ExceptionMessage{} {
			thiz.set(SourceLocation::make(location), message);
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			ExceptionMessage const & that
		) -> ExceptionMessage& = default;

		auto operator = (
			ExceptionMessage && that
		) -> ExceptionMessage& = default;

		#pragma endregion

		#pragma region set & append

		auto reset (
		) -> Void {
			thiz.m_location = SourceLocation{};
			thiz.m_message.reset();
			return;
		}

		auto set (
			SourceLocation const &                     location,
			std::initializer_list<CStringView> const & message
		) -> Void {
			thiz.m_location = location;
			thiz.m_message.assign(message);
			return;
		}

		// ----------------

		auto append (
			String const & message
		) -> Void {
			thiz.m_message.append(message);
			return;
		}

		auto append (
			String && message
		) -> Void {
			thiz.m_message.append(as_moveable(message));
			return;
		}

		#pragma endregion

		#pragma region string convert

		auto to_string (
		) const -> String {
			auto result = make_string(thiz.m_location.to_string());
			if (!thiz.m_message.empty()) {
				result.append('\n'_c);
				result.append_list(catenate_string<String>(thiz.m_message, '\n'_c));
			}
			return result;
		}

		#pragma endregion

	};

	#pragma endregion

}
