#pragma once

#include "shell/common.hpp"
#include "shell/utility/macro.hpp"
#include "shell/utility/library.hpp"
#include "shell/core/library.hpp"
#include "shell/core/symbol.hpp"

namespace TwinStar::Shell::Core {

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
				.version = thiz.m_loader.lookup<decltype(Interface::version) *>(SymbolNameTable::version),
				.execute = thiz.m_loader.lookup<decltype(Interface::execute) *>(SymbolNameTable::execute),
				.prepare = thiz.m_loader.lookup<decltype(Interface::prepare) *>(SymbolNameTable::prepare),
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

		virtual auto version (
			Interface::Size * * number
		) -> Interface::String * override {
			return thiz.m_symbol.version(number);
		}

		virtual auto execute (
			Interface::Callback * *   callback,
			Interface::String * *     script,
			Interface::StringList * * argument,
			Interface::String * *     result
		) -> Interface::String * override {
			return thiz.m_symbol.execute(callback, script, argument, result);
		}

		virtual auto prepare (
		) -> Interface::String * override {
			return thiz.m_symbol.prepare();
		}

		#pragma endregion

	};

	#pragma endregion

}
