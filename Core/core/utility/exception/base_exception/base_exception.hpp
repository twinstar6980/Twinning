#pragma once

#include "core/utility/exception/exception.hpp"
#include "core/utility/container/list/list.hpp"
#include "core/utility/string/string.hpp"
#include "core/utility/exception/base_exception/exception_message.hpp"

namespace TwinStar::Core {

	#pragma region type

	class BaseException :
		public Exception {

	protected:

		List<ExceptionMessage> m_message{};

		mutable String m_temporary{};

	public:

		#pragma region structor

		virtual ~BaseException (
		) override = default;

		// ----------------

		BaseException (
		) = default;

		BaseException (
			BaseException const & that
		) = default;

		BaseException (
			BaseException && that
		) = default;

		// ----------------

		explicit BaseException (
			ExceptionMessage && message
		) :
			BaseException{} {
			thiz.m_message.append(as_moveable(message));
		}

		explicit BaseException (
			ExceptionMessage && message,
			ExceptionMessage && sub_message
		) :
			BaseException{} {
			thiz.m_message.append(as_moveable(message));
			thiz.m_message.append(as_moveable(sub_message));
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			BaseException const & that
		) -> BaseException& = default;

		auto operator = (
			BaseException && that
		) -> BaseException& = default;

		#pragma endregion

		#pragma region message

		auto message (
		) const -> List<ExceptionMessage> const& {
			return thiz.m_message;
		}

		// ----------------

		auto reset_message (
		) -> Void {
			thiz.m_message.reset();
			return;
		}

		auto set_message (
			List<ExceptionMessage> && message
		) -> Void {
			thiz.m_message = as_moveable(message);
			return;
		}

		// ----------------

		auto append_message (
			ExceptionMessage && message
		) -> Void {
			thiz.m_message.append(as_moveable(message));
			return;
		}

		#pragma endregion

		#pragma region string convert

		auto to_string (
		) const -> String {
			auto message_string = List<String>{};
			message_string.assign(
				thiz.m_message,
				[] (auto & element) -> auto {
					return element.to_string();
				}
			);
			return catenate_string<String>(message_string, '\n'_c);
		}

		#pragma endregion

		#pragma region what message

		virtual auto what (
		) const -> std::string_view override {
			thiz.m_temporary = thiz.to_string();
			return make_std_string_view(thiz.m_temporary);
		}

		#pragma endregion

	};

	#pragma endregion

}
