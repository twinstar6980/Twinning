#pragma once

#include <string_view>
#include <string>
#include <source_location>

namespace Twinning::Kernel {

	#pragma region function

	inline auto parse_source_location_file_path (
		std::source_location const & location
	) -> std::string {
		auto string = std::string{location.file_name() + (std::string_view{std::source_location::current().file_name()}.size() - std::string_view{"kernel/utility/exception/source_location.hpp"}.size())};
		for (auto & character : string) {
			if (character == '\\') {
				character = '/';
			}
		}
		return string;
	}

	#pragma endregion

}
