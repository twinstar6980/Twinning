#pragma once

#include "shell/base.hpp"
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
			Core::String const &        script,
			Core::Boolean const &       script_is_path,
			Core::StringList const &    argument,
			Core::ShellCallback const & shell_callback
		) -> Core::String const* override {
			return thiz.m_symbol.execute(script, script_is_path, argument, shell_callback);
		}

		#pragma endregion

	};

	#pragma endregion

}
