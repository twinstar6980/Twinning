#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace Twinning::Kernel::Interface {

	#pragma region type

	struct Message {

		std::uint8_t * data{nullptr};

		std::size_t size{0};

	};

	struct Executor {

		std::add_pointer_t<void  (Executor *, Executor *, Message *, Message *, Message *)> invoke{nullptr};

		std::add_pointer_t<void  (Executor *, Executor *, Message *, Message *, Message *)> clear{nullptr};

	};

	// ----------------

	struct Service {

		Executor * executor{nullptr};

		std::add_pointer_t<void  ()> initialize{nullptr};

		std::add_pointer_t<void  ()> finalize{nullptr};

	};

	#pragma endregion

	#pragma region variable

	#if defined M_system_windows
	__declspec(dllimport)
	#endif
	#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
	#endif
	extern Service service;

	#pragma endregion

}
