#pragma once

#include "shell/common.hpp"
#include "shell/utility/macro.hpp"
#include "shell/core/library.hpp"
#include "shell/core/symbol.hpp"

namespace TwinStar::Shell::Core {

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
				.version = &Interface::version,
				.execute = &Interface::execute,
				.prepare = &Interface::prepare,
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
