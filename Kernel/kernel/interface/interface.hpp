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

	};

	#pragma endregion

	#pragma region variable

	#if defined M_compiler_msvc
	__declspec(dllimport)
	#endif
	#if defined M_compiler_clang
	#endif
	extern Service service;

	#pragma endregion

}
