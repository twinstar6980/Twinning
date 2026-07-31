module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#if defined M_system_windows

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <Windows.h>
#include <ShlObj.h>

#pragma clang diagnostic pop

#endif

export module twinning.kernel.third.system.windows;

#if defined M_system_windows

export namespace Twinning::Kernel::Third::system::windows {

	using $BOOL = BOOL;

	using $DWORD = DWORD;

	using $LONGLONG = LONGLONG;

	using $SIZE_T = SIZE_T;

	using $CHAR = CHAR;

	using $WCHAR = WCHAR;

	using $HANDLE = HANDLE;

	using $LARGE_INTEGER = LARGE_INTEGER;

	using $STARTUPINFOW = STARTUPINFOW;

	using $STARTUPINFOEXW = STARTUPINFOEXW;

	using $LPPROC_THREAD_ATTRIBUTE_LIST = LPPROC_THREAD_ATTRIBUTE_LIST;

	using $PROCESS_INFORMATION = PROCESS_INFORMATION;

	inline auto const $WideCharToMultiByte = WideCharToMultiByte;

	inline auto const $MultiByteToWideChar = MultiByteToWideChar;

	inline auto const $CloseHandle = CloseHandle;

	inline auto const $CreateFileW = CreateFileW;

	inline auto const $SetFilePointerEx = SetFilePointerEx;

	inline auto const $GetFileSizeEx = GetFileSizeEx;

	inline auto const $SetEndOfFile = SetEndOfFile;

	inline auto const $ReadFile = ReadFile;

	inline auto const $WriteFile = WriteFile;

	inline auto const $GetFileAttributesW = GetFileAttributesW;

	inline auto const $SetHandleInformation = SetHandleInformation;

	inline auto const $GetCurrentDirectoryW = GetCurrentDirectoryW;

	inline auto const $GetEnvironmentStringsW = GetEnvironmentStringsW;

	inline auto const $FreeEnvironmentStringsW = FreeEnvironmentStringsW;

	inline auto const $InitializeProcThreadAttributeList = InitializeProcThreadAttributeList;

	inline auto const $DeleteProcThreadAttributeList = DeleteProcThreadAttributeList;

	inline auto const $UpdateProcThreadAttribute = UpdateProcThreadAttribute;

	inline auto const $CreateProcessW = CreateProcessW;

	inline auto const $GetExitCodeProcess = GetExitCodeProcess;

	inline auto const $WaitForSingleObject = WaitForSingleObject;

	inline constexpr auto $FALSE = FALSE;

	inline constexpr auto $TRUE = TRUE;

	inline constexpr auto $INFINITE = INFINITE;

	inline constexpr auto $CP_UTF8 = CP_UTF8;

	inline constexpr auto $CP_ACP = CP_ACP;

	inline constexpr auto $WC_ERR_INVALID_CHARS = WC_ERR_INVALID_CHARS;

	inline constexpr auto $MB_ERR_INVALID_CHARS = MB_ERR_INVALID_CHARS;

	inline constexpr auto $CREATE_NEW = CREATE_NEW;

	inline constexpr auto $OPEN_EXISTING = OPEN_EXISTING;

	inline constexpr auto $GENERIC_READ = GENERIC_READ;

	inline constexpr auto $GENERIC_WRITE = GENERIC_WRITE;

	inline constexpr auto $FILE_SHARE_READ = FILE_SHARE_READ;

	inline constexpr auto $FILE_SHARE_WRITE = FILE_SHARE_WRITE;

	inline constexpr auto $FILE_SHARE_DELETE = FILE_SHARE_DELETE;

	inline constexpr auto $FILE_BEGIN = FILE_BEGIN;

	inline constexpr auto $INVALID_FILE_ATTRIBUTES = INVALID_FILE_ATTRIBUTES;

	inline constexpr auto $FILE_ATTRIBUTE_NORMAL = FILE_ATTRIBUTE_NORMAL;

	inline constexpr auto $FILE_ATTRIBUTE_DIRECTORY = FILE_ATTRIBUTE_DIRECTORY;

	inline constexpr auto $STARTF_USESTDHANDLES = STARTF_USESTDHANDLES;

	inline constexpr auto $HANDLE_FLAG_INHERIT = HANDLE_FLAG_INHERIT;

	inline constexpr auto $PROC_THREAD_ATTRIBUTE_HANDLE_LIST = PROC_THREAD_ATTRIBUTE_HANDLE_LIST;

	inline constexpr auto $CREATE_UNICODE_ENVIRONMENT = CREATE_UNICODE_ENVIRONMENT;

	inline constexpr auto $CREATE_NO_WINDOW = CREATE_NO_WINDOW;

	inline constexpr auto $EXTENDED_STARTUPINFO_PRESENT = EXTENDED_STARTUPINFO_PRESENT;

	inline constexpr auto $WAIT_OBJECT_0 = WAIT_OBJECT_0;

	inline auto const $INVALID_HANDLE_VALUE = INVALID_HANDLE_VALUE;

}

#endif
