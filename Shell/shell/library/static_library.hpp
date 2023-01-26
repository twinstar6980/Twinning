#pragma once

#include "shell/common.hpp"
#include "shell/library/library.hpp"

namespace TwinStar::Shell {

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
				.version = &Core::version,
				.execute = &Core::execute,
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
		) -> Core::Size const* override {
			return thiz.m_symbol.version();
		}

		virtual auto execute (
			Core::Callback const &   callback,
			Core::String const &     script,
			Core::StringList const & argument
		) -> Core::String const* override {
			return thiz.m_symbol.execute(callback, script, argument);
		}

		#pragma endregion

	};

	#pragma endregion

}
