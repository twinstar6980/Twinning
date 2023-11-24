//

#include "kernel/interface/implement.hpp"

#if defined M_vld
#include "vld.h"
#endif

#define M_interface_implement
#include "kernel/interface/interface.hpp"

namespace TwinStar::Kernel::Interface {

	#pragma region function

	M_symbol_export
	extern auto execute (
		Callback * *   callback,
		String * *     script,
		StringList * * argument,
		String * *     result
	) -> String * {
		thread_local auto result_handler = Kernel::String{};
		thread_local auto exception_handler = Kernel::String{};
		restruct(result_handler);
		restruct(exception_handler);
		*result = &self_cast<String>(result_handler);
		#if defined M_build_release
		try {
		#endif
			Implement::execute(
				self_cast<Kernel::Executor::Callback>(**callback),
				self_cast<Kernel::String>(**script),
				self_cast<Kernel::List<Kernel::String>>(**argument),
				self_cast<Kernel::String>(**result)
			);
			return nullptr;
		#if defined M_build_release
		}
		catch (...) {
			auto exception_value = make_string(parse_current_exception().what());
			exception_handler = as_moveable(exception_value);
			return &self_cast<String>(exception_handler);
		}
		#endif
	}

	#pragma endregion

}
