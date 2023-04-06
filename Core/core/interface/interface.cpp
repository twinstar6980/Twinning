//

#include "core/interface/implement.hpp"

#if defined M_vld
#include "vld.h"
#endif

#define M_interface_implement
#include "core/interface/interface.hpp"

namespace TwinStar::Core::Interface {

	#pragma region function

	M_symbol_export
	extern auto version (
		Size * * number
	) -> String * {
		thread_local auto exception_handler = Core::String{};
		thread_local auto number_handler = Core::Size{};
		restruct(exception_handler);
		restruct(number_handler);
		*number = &self_cast<Size>(number_handler);
		#if defined M_build_release
		try {
		#endif
			Implement::version(
				self_cast<Core::Size>(**number)
			);
			return nullptr;
		#if defined M_build_release
		} catch (...) {
			auto exception_value = make_string(parse_current_exception().what());
			exception_handler = as_moveable(exception_value);
			return &self_cast<String>(exception_handler);
		}
		#endif
	}

	M_symbol_export
	extern auto execute (
		Callback * *   callback,
		String * *     script,
		StringList * * argument,
		String * *     result
	) -> String * {
		thread_local auto exception_handler = Core::String{};
		thread_local auto result_handler = Core::String{};
		restruct(exception_handler);
		restruct(result_handler);
		*result = &self_cast<String>(result_handler);
		#if defined M_build_release
		try {
		#endif
			Implement::execute(
				self_cast<Core::Executor::Callback>(**callback),
				self_cast<Core::String>(**script),
				self_cast<Core::List<Core::String>>(**argument),
				self_cast<Core::String>(**result)
			);
			return nullptr;
		#if defined M_build_release
		} catch (...) {
			auto exception_value = make_string(parse_current_exception().what());
			exception_handler = as_moveable(exception_value);
			return &self_cast<String>(exception_handler);
		}
		#endif
	}

	M_symbol_export
	extern auto prepare (
	) -> String * {
		thread_local auto exception_handler = Core::String{};
		restruct(exception_handler);
		#if defined M_build_release
		try {
		#endif
			Implement::prepare(
			);
			return nullptr;
		#if defined M_build_release
		} catch (...) {
			auto exception_value = make_string(parse_current_exception().what());
			exception_handler = as_moveable(exception_value);
			return &self_cast<String>(exception_handler);
		}
		#endif
	}

	#pragma endregion

}
