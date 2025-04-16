module;

#if defined M_system_windows

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <Windows.h>
#include <ShlObj.h>

#pragma clang diagnostic pop

#endif

export module twinning.shell.third.system.windows;

#if defined M_system_windows

export namespace Twinning::Shell::Third::system::windows {

	using $BOOL = ::BOOL;

	using $DWORD = ::DWORD;

	using $LPWSTR = ::LPWSTR;

	using $LPCWSTR = ::LPCWSTR;

	using $HRESULT = ::HRESULT;

	using $HMODULE = ::HMODULE;

	using $IShellItem = ::IShellItem;

	using $IFileDialog = ::IFileDialog;

	using $FILEOPENDIALOGOPTIONS = ::FILEOPENDIALOGOPTIONS;

	inline constexpr auto $HRESULT_FROM_WIN32 = ::HRESULT_FROM_WIN32;

	inline auto const $LoadLibraryW = ::LoadLibraryW;

	inline auto const $FreeLibrary = ::FreeLibrary;

	inline auto const $GetProcAddress = ::GetProcAddress;

	inline auto const $GetStdHandle = ::GetStdHandle;

	inline auto const $GetConsoleMode = ::GetConsoleMode;

	inline auto const $ReadConsoleW = ::ReadConsoleW;

	inline auto const $WriteConsoleW = ::WriteConsoleW;

	inline auto const $GetForegroundWindow = ::GetForegroundWindow;

	inline auto const $CoInitialize = ::CoInitialize;

	inline auto const $CoUninitialize = ::CoUninitialize;

	inline auto const $CoCreateInstance = ::CoCreateInstance;

	inline auto const $CoTaskMemFree = ::CoTaskMemFree;

	inline constexpr auto $S_OK = S_OK;

	inline constexpr auto $FALSE = FALSE;

	inline constexpr auto $E_INVALIDARG = E_INVALIDARG;

	inline constexpr auto $ERROR_CANCELLED = ERROR_CANCELLED;

	inline constexpr auto $STD_INPUT_HANDLE = STD_INPUT_HANDLE;

	inline constexpr auto $STD_OUTPUT_HANDLE = STD_OUTPUT_HANDLE;

	inline constexpr auto $CLSCTX_INPROC_SERVER = ::CLSCTX_INPROC_SERVER;

	inline constexpr auto $SIGDN_FILESYSPATH = ::SIGDN_FILESYSPATH;

	inline constexpr auto $FOS_NOCHANGEDIR = ::FOS_NOCHANGEDIR;

	inline constexpr auto $FOS_NOVALIDATE = ::FOS_NOVALIDATE;

	inline constexpr auto $FOS_NODEREFERENCELINKS = ::FOS_NODEREFERENCELINKS;

	inline constexpr auto $FOS_DONTADDTORECENT = ::FOS_DONTADDTORECENT;

	inline constexpr auto $FOS_FORCESHOWHIDDEN = ::FOS_FORCESHOWHIDDEN;

	inline constexpr auto $FOS_PATHMUSTEXIST = ::FOS_PATHMUSTEXIST;

	inline constexpr auto $FOS_FILEMUSTEXIST = ::FOS_FILEMUSTEXIST;

	inline constexpr auto $FOS_PICKFOLDERS = ::FOS_PICKFOLDERS;

	inline constexpr auto $FOS_OVERWRITEPROMPT = ::FOS_OVERWRITEPROMPT;

	inline auto const $CLSID_FileOpenDialog = ::CLSID_FileOpenDialog;

	inline auto const $CLSID_FileSaveDialog = ::CLSID_FileSaveDialog;

	template <typename T>
	inline auto $IID_PPV_ARGS_Helper (
		T * * p
	) -> void * * {
		return ::IID_PPV_ARGS_Helper<T>(p);
	}

}

#endif
