module;

#include <cxxabi.h>
#include "kernel/common.hpp"
#include "kernel/utility/miscellaneous/low_level/common.hpp"

export module twinning.kernel.utility.miscellaneous.low_level.compiler;

export namespace Twinning::Kernel::LowLevel::Compiler {

	#pragma region name

	inline auto parse_mangled_name(
		std::string_view const & source
	) -> std::string {
		auto status = int{};
		auto data = abi::__cxa_demangle(source.data(), nullptr, nullptr, &status);
		assert_test(status == 0);
		auto result = std::string{data};
		std::free(data);
		return result;
	}

	#pragma endregion

}
