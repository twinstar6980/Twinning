#pragma once

#include "core/utility/exception/simple_exception.hpp"

namespace TwinStar::Core {

	#pragma region type

	class NeverException :
		public SimpleException {

	public:

		#pragma region structor

		NeverException (
		) :
			SimpleException{"Never"} {
		}

		#pragma endregion

	};

	class ToDoException :
		public SimpleException {

	public:

		#pragma region structor

		ToDoException (
		) :
			SimpleException{"ToDo"} {
		}

		#pragma endregion

	};

	#pragma endregion

}

#define assert_test(...) \
	if (!(__VA_ARGS__)) {\
		throw SimpleException{SourceLocation::make(M_current_source_location).to_string() + "\nassertion failed : " #__VA_ARGS__};\
	}\
	static_assert(true)

#define assert_fail(message) \
	throw SimpleException{SourceLocation::make(M_current_source_location).to_string() + "\nassertion failed : " message}
