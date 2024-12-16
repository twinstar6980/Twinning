module;

#include <typeinfo>
#include "shell/common.hpp"

export module twinning.shell.utility.exception;

export namespace Twinning::Shell {

	#pragma region function

	inline auto parse_current_exception (
	) -> std::string {
		auto destination = std::string{};
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (std::string & source) {
			destination = source;
		}
		catch (std::exception & source) {
			destination = std::string{typeid(source).name()} + " : "s + std::string{source.what()};
		}
		catch (...) {
			destination = "?"s;
		}
		return destination;
	}

	#pragma endregion

}
