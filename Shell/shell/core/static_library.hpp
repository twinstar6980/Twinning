#pragma once

#include "shell/common.hpp"
#include "shell/core/library.hpp"

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
		):
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
		) -> StaticLibrary& = delete;

		auto operator = (
			StaticLibrary && that
		) -> StaticLibrary& = delete;

		#pragma endregion

		#pragma region interface

		virtual auto version (
		) -> Interface::Size const* override {
			return thiz.m_symbol.version();
		}

		virtual auto execute (
			Interface::Callback const *   callback,
			Interface::String const *     script,
			Interface::StringList const * argument
		) -> Interface::String const* override {
			return thiz.m_symbol.execute(callback, script, argument);
		}

		virtual auto prepare (
		) -> Interface::String const* override {
			return thiz.m_symbol.prepare();
		}

		#pragma endregion

	};

	#pragma endregion

}
