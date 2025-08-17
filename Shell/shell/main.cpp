// twinning.shell.main

#include "shell/common.hpp"

import twinning.shell.main_console;

#pragma region main

#if defined M_system_windows
auto wmain (
	int       argc,
	wchar_t * argv[]
)
#endif
#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
auto main (
	int    argc,
	char * argv[]
)
#endif
	-> int {
	return Twinning::Shell::MainConsole::run(
		argc,
		reinterpret_cast<void * *>(argv),
		#if defined M_kernel_library_symbol
		M_kernel_library_symbol
		#else
		nullptr
		#endif
	);
}

#pragma endregion
