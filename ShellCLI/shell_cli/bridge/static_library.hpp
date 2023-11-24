#pragma once

#include "shell_cli/common.hpp"
#include "shell_cli/utility/macro.hpp"
#include "shell_cli/bridge/library.hpp"
#include "shell_cli/bridge/symbol.hpp"

namespace TwinStar::ShellCLI::Bridge {

	#pragma region type

	class StaticLibrary :
		public Library {

	protected:

		SymbolTable m_symbol;

	public:

		#pragma region structor

		virtual ~StaticLibrary (
		) override {
		}

		// ----------------

		StaticLibrary (
		) = delete;

		StaticLibrary (
			StaticLibrary const & that
		) = delete;

		StaticLibrary (
			StaticLibrary && that
		) = delete;

		// ----------------

		explicit StaticLibrary (
			std::nullptr_t _
		) :
			m_symbol{} {
			thiz.m_symbol = {
				.execute = &Interface::execute,
			};
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			StaticLibrary const & that
		) -> StaticLibrary & = delete;

		auto operator = (
			StaticLibrary && that
		) -> StaticLibrary & = delete;

		#pragma endregion

		#pragma region interface

		virtual auto execute (
			Interface::Callback * *   callback,
			Interface::String * *     script,
			Interface::StringList * * argument,
			Interface::String * *     result
		) -> Interface::String * override {
			return thiz.m_symbol.execute(callback, script, argument, result);
		}

		#pragma endregion

	};

	#pragma endregion

}
