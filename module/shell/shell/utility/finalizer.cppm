module;

#include "shell/common.hpp"

export module twinning.shell.utility.finalizer;

export namespace Twinning::Shell {

	#pragma region utility

	template <typename TFinalizer>
	inline constexpr auto make_finalizer(
		TFinalizer const & finalizer
	) -> std::unique_ptr<TFinalizer, void (*)(std::add_pointer_t<TFinalizer>)> {
		return std::unique_ptr<TFinalizer, void (*)(std::add_pointer_t<TFinalizer>)>{new TFinalizer{finalizer}, [](std::add_pointer_t<TFinalizer> it) -> void {
			(*it)();
			delete it;
		}};
	}

	#pragma endregion

}
