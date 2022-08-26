#pragma once

#include "core/utility/system/windows.hpp"
#include "core/utility/system/posix.hpp"
#include "core/utility/string/string.hpp"
#include "core/utility/file_system/path.hpp"

namespace TwinKleS::Core::System {

	#pragma region namespace alias

	#if defined M_system_windows
	namespace Current = Windows;
	#endif
	#if defined M_system_linux || defined M_system_macos || defined M_system_android
	namespace Current = POSIX;
	#endif

	#pragma endregion

	#pragma region control

	// NOTE : [[noreturn]]
	inline auto exit (
		IntegerS32 const & code
	) -> Void {
		return Current::exit(code);
	}

	inline auto sleep (
		Size const & time
	) -> Void {
		return Current::sleep(time);
	}

	#pragma endregion

	#pragma region command

	inline auto system (
		String const & command
	) -> IntegerS32 {
		return Current::system(command);
	}

	inline auto process (
		Path const &         path,
		List<String> const & argument
	) -> IntegerS32 {
		return Current::process(path, argument);
	}

	#pragma endregion

}
