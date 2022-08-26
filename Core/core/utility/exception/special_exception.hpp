#pragma once

// TODO

#include "core/utility/exception/simple_exception.hpp"

namespace TwinKleS::Core {

	#pragma region type

	class NeverException :
		public SimpleException {

	public: //

		#pragma region structor

		NeverException (
		) :
			SimpleException{"Never"} {
		}

		#pragma endregion

	};

	class ToDoException :
		public SimpleException {

	public: //

		#pragma region structor

		ToDoException (
		) :
			SimpleException{"ToDo"} {
		}

		#pragma endregion

	};

	#pragma endregion

}

#define assert_condition(...) \
	if (!(__VA_ARGS__)) {\
		throw SimpleException{SourceLocation::make(M_current_source_location).to_string() + "\nassert failed : " #__VA_ARGS__};\
	}\
	void()

#define assert_failed(message) \
	throw SimpleException{SourceLocation::make(M_current_source_location).to_string() + "\nassert failed : " message}
