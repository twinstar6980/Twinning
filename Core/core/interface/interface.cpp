//

#include "core/interface/implement.hpp"
#include <exception>
#include <clocale>

#if defined M_vld
#include "vld.h"
#endif

#define M_interface_implement
#include "core/interface/interface.hpp"

namespace TwinStar::Core::Interface {

	#pragma region initialize

	inline auto initialize (
	) -> Void {
		// TODO : thread unsafe ?
		// set locale
		std::setlocale(LC_ALL, "C");
		try {
			// NOTE : LC_NUMERIC的设置会影响quickjs对字符串的解析，这在一些不以点作为小数点的地区（如俄罗斯）本地化设置下将导致脚本运行出错
			std::setlocale(LC_CTYPE, ".utf-8");
		} catch (std::exception & exception) {
			M_log("set locale failed : {}"_sf(message_of_std_exception(exception)));
		}
		return;
	}

	#pragma endregion

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
		Callback const &   callback,
		String const &     script,
		StringList const & argument
	) -> String const* {
		thread_local auto result = Core::Optional<Core::String>{};
		#if defined M_build_release
		try {
		#endif
		result.reset();
		initialize();
		result = Implement::execute(
			self_cast<Core::Executor::Callback>(callback),
			self_cast<Core::String>(script),
			self_cast<Core::List<Core::String>>(argument)
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
		return result ? &self_cast<String>(result.get()) : nullptr;
	}

	#pragma endregion

}
