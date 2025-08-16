module;

#include "shell/common.hpp"

export module twinning.shell.utility.exception;

export namespace Twinning::Shell {

	#pragma region function

	inline auto parse_current_exception (
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
			message = "UnknownException";
		}
		auto file_path = std::string{location.file_name() + (std::string_view{std::source_location::current().file_name()}.size() - std::string_view{"shell/utility/exception.cppm"}.size())};
		std::replace(file_path.begin(), file_path.end(), '\\', '/');
		return std::format("{}\n@ {}:{}:{} {}", message, file_path, location.line(), location.column(), location.function_name());
	}

	#pragma endregion

}
