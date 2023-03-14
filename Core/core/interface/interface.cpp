//

#include "core/interface/implement.hpp"
#include <exception>

#if defined M_vld
#include "vld.h"
#endif

#define M_interface_implement
#include "core/interface/interface.hpp"

namespace TwinStar::Core::Interface {

	#pragma region function

	M_symbol_export
	extern auto version (
	) -> Size const* {
		thread_local auto result = Core::Size{};
		result = Implement::version();
		return &self_cast<Size>(result);
	}

	M_symbol_export
	extern auto execute (
		Callback const *   callback,
		String const *     script,
		StringList const * argument
	) -> String const* {
		thread_local auto result = Core::Optional<Core::String>{};
		#if defined M_build_release
		try {
		#endif
			result.reset();
			result = Implement::execute(
				self_cast<Core::Executor::Callback>(*callback),
				self_cast<Core::String>(*script),
				self_cast<Core::List<Core::String>>(*argument)
			);
		#if defined M_build_release
		} catch (Exception & exception) {
			result.set(make_string_view(exception.what()));
		} catch (std::exception & exception) {
			result.set(make_string_view(exception.what()));
		} catch (...) {
			result.set(make_string_view("unknown exception"));
		}
		#endif
		return !result ? (nullptr) : (&self_cast<String>(result.get()));
	}

	M_symbol_export
	extern auto prepare (
	) -> String const* {
		thread_local auto result = Core::Optional<Core::String>{};
		#if defined M_build_release
		try {
		#endif
			result = Implement::prepare();
		#if defined M_build_release
		} catch (std::exception & exception) {
			result.set(make_string_view(exception.what()));
		} catch (...) {
			result.set(make_string_view("unknown exception"));
		}
		#endif
		return !result ? (nullptr) : (&self_cast<String>(result.get()));
	}

	#pragma endregion

}
