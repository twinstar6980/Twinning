module;

#if defined M_system_windows

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <Windows.h>
#include <ShlObj.h>
#include <corecrt_wstdio.h>

#pragma clang diagnostic pop

#endif

export module twinning.kernel.third.system.windows;

#if defined M_system_windows

export namespace Twinning::Kernel::Third::system::windows {

	using $BOOL = ::BOOL;

	using $DWORD = ::DWORD;

	using $WCHAR = ::WCHAR;

	using $SECURITY_ATTRIBUTES = ::SECURITY_ATTRIBUTES;

	using $STARTUPINFOW = ::STARTUPINFOW;

	using $PROCESS_INFORMATION = ::PROCESS_INFORMATION;

	inline auto const $_wgetenv = ::_wgetenv;

	inline auto const $_wfopen = ::_wfopen;

	inline auto const $_wsystem = ::_wsystem;

	inline auto const $GetLastError = ::GetLastError;

	inline auto const $GetEnvironmentVariableW = ::GetEnvironmentVariableW;

	inline auto const $SetEnvironmentVariableW = ::SetEnvironmentVariableW;

	inline auto const $MultiByteToWideChar = ::MultiByteToWideChar;

	inline auto const $CloseHandle = ::CloseHandle;

	inline auto const $CreateFileW = ::CreateFileW;

	inline auto const $CreateProcessW = ::CreateProcessW;

	inline auto const $GetExitCodeProcess = ::GetExitCodeProcess;

	inline auto const $WaitForSingleObject = ::WaitForSingleObject;

	inline constexpr auto $FALSE = FALSE;

	inline constexpr auto $TRUE = TRUE;

	inline constexpr auto $INFINITE = INFINITE;

	inline constexpr auto $CP_ACP = CP_ACP;

	inline constexpr auto $MB_ERR_INVALID_CHARS = MB_ERR_INVALID_CHARS;

	inline constexpr auto $ERROR_ENVVAR_NOT_FOUND = ERROR_ENVVAR_NOT_FOUND;

	inline constexpr auto $GENERIC_READ = GENERIC_READ;

	inline constexpr auto $GENERIC_WRITE = GENERIC_WRITE;

	inline constexpr auto $FILE_SHARE_READ = FILE_SHARE_READ;

	inline constexpr auto $FILE_SHARE_WRITE = FILE_SHARE_WRITE;

	inline constexpr auto $FILE_SHARE_DELETE = FILE_SHARE_DELETE;

	inline constexpr auto $OPEN_EXISTING = OPEN_EXISTING;

	inline constexpr auto $FILE_ATTRIBUTE_NORMAL = FILE_ATTRIBUTE_NORMAL;

	inline constexpr auto $STARTF_USESTDHANDLES = STARTF_USESTDHANDLES;

	inline constexpr auto $CREATE_UNICODE_ENVIRONMENT = CREATE_UNICODE_ENVIRONMENT;

	inline constexpr auto $DETACHED_PROCESS = DETACHED_PROCESS;

	inline constexpr auto $WAIT_OBJECT_0 = WAIT_OBJECT_0;

	inline auto const $INVALID_HANDLE_VALUE = INVALID_HANDLE_VALUE;

	inline auto $_wenviron (
	) -> wchar_t * * {
		return _wenviron;
	}

}

#endif
