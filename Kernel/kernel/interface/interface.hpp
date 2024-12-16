#pragma once

namespace Twinning::Kernel::Interface {

	extern "C++" struct Service;

	#if defined M_system_windows
	__declspec(dllimport)
	#endif
	#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
	#endif
	extern "C++" Service service;

}
