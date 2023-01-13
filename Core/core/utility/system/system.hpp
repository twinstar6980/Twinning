#pragma once

#include "core/utility/system/windows.hpp"
#include "core/utility/system/posix.hpp"
#include "core/utility/string/string.hpp"
#include "core/utility/file_system/path.hpp"

namespace TwinStar::Core::System {

	#pragma region namespace alias

	#if defined M_system_windows
	namespace Current = Windows;
	#endif
	#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
	namespace Current = POSIX;
	#endif

	#pragma endregion

	#pragma region process

	inline auto exit (
		IntegerU32 const & code
	) -> Void {
		return Current::exit(code);
	}

	// NOTE
	// the return value is program's exit code, see the following webpage to understand
	// Windows - https://learn.microsoft.com/windows/win32/api/processthreadsapi/nf-processthreadsapi-getexitcodeprocess
	// POSIX   - https://pubs.opengroup.org/onlinepubs/9699919799/functions/waitid.html
	// in Windows, the complete 32-bit exit code can always be obtained
	// for POSIX, although the standard states that the waitid function should obtain the full exit code, this is not actually the case in Linux and Macintosh
	// the following are the number of return bits that can be obtained in each system
	// Windows   : all 32 bit
	// Linux     : low 08 bit
	// Macintosh : low 24 bit
	inline auto execute (
		Path const &           program,
		List<String> const &   argument,
		Optional<Path> const & redirect_input,
		Optional<Path> const & redirect_output,
		Optional<Path> const & redirect_error
	) -> IntegerU32 {
		return Current::execute(program, argument, redirect_input, redirect_output, redirect_error);
	}

	// NOTE
	// implement defined
	// on iphone, std::system is not available, this function always return 0x00000000
	inline auto system (
		String const & command
	) -> IntegerU32 {
		return Current::system(command);
	}

	#pragma endregion

}
