module;

#include "shell/common.hpp"

export module twinning.shell.utility.exception;

export namespace Twinning::Shell {

	#pragma region utility

	inline auto parse_current_exception(
		std::source_location const & location = std::source_location::current()
	) -> std::string {
		auto message = std::string{};
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (std::exception & e) {
			message = e.what();
		}
		catch (...) {
			message = "UnknownException"s;
		}
		auto root_path = std::string_view{std::source_location::current().file_name(), std::string_view{std::source_location::current().file_name()}.size() - "shell/utility/exception.cppm"sv.size()};
		auto file_path = std::string{location.file_name()};
		if (file_path.starts_with(root_path)) {
			file_path.erase(0zu, root_path.size());
		}
		std::ranges::replace(file_path, '\\', '/');
		return std::format("{}\n@ {}:{}:{} {}"sv, message, file_path, location.line(), location.column(), location.function_name());
	}

	#pragma endregion

}
