module;

#if defined M_system_windows

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <Windows.h>

#pragma clang diagnostic pop

#endif

export module twinning.shell.third.system.windows;

#if defined M_system_windows

export namespace Twinning::Shell::Third::system::windows {

	using $BOOL = BOOL;

	using $DWORD = DWORD;

	using $HMODULE = HMODULE;

	inline auto const $WideCharToMultiByte = WideCharToMultiByte;

	inline auto const $MultiByteToWideChar = MultiByteToWideChar;

	inline auto const $GetStdHandle = GetStdHandle;

	inline auto const $GetConsoleMode = GetConsoleMode;

	inline auto const $ReadConsoleW = ReadConsoleW;

	inline auto const $WriteConsoleW = WriteConsoleW;

	inline auto const $LoadLibraryW = LoadLibraryW;

	inline auto const $FreeLibrary = FreeLibrary;

	inline auto const $GetProcAddress = GetProcAddress;

	inline auto const $SetProcessPreferredUILanguages = SetProcessPreferredUILanguages;

	inline constexpr auto $FALSE = FALSE;

	inline constexpr auto $CP_UTF8 = CP_UTF8;

	inline constexpr auto $CP_ACP = CP_ACP;

	inline constexpr auto $WC_ERR_INVALID_CHARS = WC_ERR_INVALID_CHARS;

	inline constexpr auto $MB_ERR_INVALID_CHARS = MB_ERR_INVALID_CHARS;

	inline constexpr auto $STD_INPUT_HANDLE = STD_INPUT_HANDLE;

	inline constexpr auto $STD_OUTPUT_HANDLE = STD_OUTPUT_HANDLE;

	inline constexpr auto $MUI_LANGUAGE_NAME = MUI_LANGUAGE_NAME;

}

#endif
