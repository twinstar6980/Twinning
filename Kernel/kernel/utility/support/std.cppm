module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.std;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.string;

export {

	#pragma region hash

	template <>
	struct std::hash<Twinning::Kernel::CStringView> {
		auto operator () (
			Twinning::Kernel::CStringView const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(Twinning::Kernel::make_std_string_view(it));
		}
	};

	template <>
	struct std::hash<Twinning::Kernel::VStringView> {
		auto operator () (
			Twinning::Kernel::VStringView const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(Twinning::Kernel::make_std_string_view(it));
		}
	};

	template <>
	struct std::hash<Twinning::Kernel::String> {
		auto operator () (
			Twinning::Kernel::String const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(Twinning::Kernel::make_std_string_view(it));
		}
	};

	#pragma endregion

}
