module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.std;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.box;
import twinning.kernel.utility.string.string;

export {

	#pragma region hash

	template <>
	struct std::hash<Twinning::Kernel::ConstantStringView> {
		auto operator ()(
			Twinning::Kernel::ConstantStringView const & it
		) const noexcept -> std::size_t {
			return unmake_box_unsafe<std::size_t>(it.hash());
		}
	};

	template <>
	struct std::hash<Twinning::Kernel::String> {
		auto operator ()(
			Twinning::Kernel::String const & it
		) const noexcept -> std::size_t {
			return unmake_box_unsafe<std::size_t>(it.hash());
		}
	};

	#pragma endregion

}
