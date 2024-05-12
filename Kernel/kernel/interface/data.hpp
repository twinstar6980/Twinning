#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace TwinStar::Kernel::Interface {

	#pragma region type

	struct Message {
		std::uint8_t * data{nullptr};
		std::size_t    size{0};
	};

	struct Executor {
		std::add_pointer_t<void  (Executor *, Executor *, Message *, Message *, Message *)> invoke{nullptr};
		std::add_pointer_t<void  (Executor *, Executor *, Message *, Message *, Message *)> clear{nullptr};
	};

	#pragma endregion

}
