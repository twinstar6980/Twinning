#pragma once

#include "shell_cli/common.hpp"
#include "shell_cli/utility/macro.hpp"
#include "shell_cli/utility/library.hpp"
#include "shell_cli/bridge/library.hpp"
#include "shell_cli/bridge/symbol.hpp"

namespace TwinStar::ShellCLI::Bridge {

	#pragma region type

	class DynamicLibrary :
		public Library {

	protected:

		SymbolTable   m_symbol;
		LibraryLoader m_loader;

	public:

		#pragma region structor

		virtual ~DynamicLibrary (
		) override {
			thiz.m_loader.close();
		}

		// ----------------

		DynamicLibrary (
		) = delete;

		DynamicLibrary (
			DynamicLibrary const & that
		) = delete;

		DynamicLibrary (
			DynamicLibrary && that
		) = delete;

		// ----------------

		explicit DynamicLibrary (
			std::nullptr_t           _,
			std::string_view const & path
		) :
			m_symbol{},
			m_loader{} {
			thiz.m_loader.open(path);
			thiz.m_symbol = {
				.execute = thiz.m_loader.lookup<decltype(Interface::execute) *>(SymbolNameTable::execute),
			};
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			DynamicLibrary const & that
		) -> DynamicLibrary & = delete;

		auto operator = (
			DynamicLibrary && that
		) -> DynamicLibrary & = delete;

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
