#pragma once

#include "shell/common.hpp"
#include "shell/core/library.hpp"
#include "shell/core/symbol.hpp"

namespace TwinStar::Shell::Core {

	#pragma region detail

	namespace Detail {

		using DynamicLibraryHandle = void *;

		// ----------------

		inline auto open_library (
			std::string_view const & path
		) -> DynamicLibraryHandle {
			#if defined M_system_windows
			auto path_absolute = std::string{path} + "."s;
			auto path_w = utf8_to_utf16(reinterpret_cast<std::u8string const &>(path_absolute));
			auto result = LoadLibraryW(reinterpret_cast<LPCWSTR>(path_w.c_str()));
			if (!result) {
				throw std::runtime_error{"can not open library : "s + std::string{path}};
			}
			return result;
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			auto result = dlopen(path.data(), RTLD_LAZY | RTLD_LOCAL);
			if (!result) {
				throw std::runtime_error{"can not open library : "s + std::string{path}};
			}
			return result;
			#endif
		}

		inline auto close_library (
			DynamicLibraryHandle const & handle
		) -> void {
			#if defined M_system_windows
			auto state = FreeLibrary(static_cast<HMODULE>(handle));
			if (state != TRUE) {
				throw std::runtime_error{"can not close library"s};
			}
			return;
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			auto state = dlclose(handle);
			if (state != 0) {
				throw std::runtime_error{"can not close library"s};
			}
			return;
			#endif
		}

		template <typename Symbol>
		inline auto get_symbol (
			DynamicLibraryHandle const & handle,
			std::string_view const &     symbol
		) -> Symbol {
			#if defined M_system_windows
			auto result = GetProcAddress(static_cast<HMODULE>(handle), symbol.data());
			if (!result) {
				throw std::runtime_error{"can not get symbol : "s + std::string{symbol}};
			}
			return reinterpret_cast<Symbol>(result);
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			auto result = dlsym(handle, symbol.data());
			if (!result) {
				throw std::runtime_error{"can not get symbol : "s + std::string{symbol}};
			}
			return reinterpret_cast<Symbol>(result);
			#endif
		}

	}

	#pragma endregion

	#pragma region type

	class DynamicLibrary :
		public Library {

	protected:

		SymbolTable                  m_symbol;
		Detail::DynamicLibraryHandle m_handle;

	public:

		#pragma region structor

		virtual ~DynamicLibrary (
		) override {
			Detail::close_library(thiz.m_handle);
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
			m_handle{} {
			thiz.m_handle = Detail::open_library(path);
			thiz.m_symbol = {
				.version = Detail::get_symbol<decltype(Interface::version) *>(thiz.m_handle, SymbolNameTable::version),
				.execute = Detail::get_symbol<decltype(Interface::execute) *>(thiz.m_handle, SymbolNameTable::execute),
				.prepare = Detail::get_symbol<decltype(Interface::prepare) *>(thiz.m_handle, SymbolNameTable::prepare),
			};
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			DynamicLibrary const & that
		) -> DynamicLibrary& = delete;

		auto operator = (
			DynamicLibrary && that
		) -> DynamicLibrary& = delete;

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
