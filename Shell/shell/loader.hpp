#pragma once

#include "./base.hpp"
#include <optional>

#if defined M_system_windows
#include "Windows.h"
#endif
#if defined M_system_linux || defined M_system_macos || defined M_system_android
#include "dlfcn.h"
#endif

namespace TwinKleS::Shell::Loader {

	#pragma region detail

	namespace Detail {

		using LibraryHandle = void *;

		struct SymbolTable {
			decltype(TwinKleS::Core::Interface::version) * version;
			decltype(TwinKleS::Core::Interface::execute) * execute;
		};

		inline constexpr struct {
			std::string_view version;
			std::string_view execute;
		}
		k_symbol_id = {
			#if defined M_compiler_msvc
			.version = "?version@Interface@Core@TwinKleS@@YAPEBUSize@123@XZ",
			.execute = "?execute@Interface@Core@TwinKleS@@YAPEBUString@123@AEBU4123@AEBUBoolean@123@AEBUStringList@123@AEBQ6AAEBU6123@2@Z@Z",
			#endif
			#if defined M_compiler_clang
			.version = "_ZN8TwinKleS4Core9Interface7versionEv",
			.execute = "_ZN8TwinKleS4Core9Interface7executeERKNS1_6StringERKNS1_7BooleanERKNS1_10StringListERKPFSA_SA_E",
			#endif
		};

		// ----------------

		inline auto open_library (
			std::string_view const & path
		) -> LibraryHandle {
			#if defined M_system_windows
			auto path_w = utf8_to_utf16(reinterpret_cast<std::u8string_view const &>(path));
			auto result = LoadLibraryW(reinterpret_cast<LPCWSTR>(path_w.c_str()));
			#endif
			#if defined M_system_linux || defined M_system_macos || defined M_system_android
			auto result = dlopen(path.data(), RTLD_LAZY | RTLD_LOCAL);
			#endif
			if (!result) {
				throw std::runtime_error{(std::string{"can not open library : "} + std::string{path}).c_str()};
			}
			return result;
		}

		inline auto close_library (
			LibraryHandle const & handle
		) -> void {
			#if defined M_system_windows
			auto result = FreeLibrary(static_cast<HMODULE>(handle));
			if (result != TRUE) {
				throw std::runtime_error{(std::string{"can not close library"}).c_str()};
			}
			#endif
			#if defined M_system_linux || defined M_system_macos || defined M_system_android
			auto result = dlclose(handle);
			if (result != 0) {
				throw std::runtime_error{(std::string{"can not close library"}).c_str()};
			}
			#endif
			return;
		}

		template <typename Symbol>
		inline auto get_symbol (
			LibraryHandle const &    handle,
			std::string_view const & symbol
		) -> Symbol {
			#if defined M_system_windows
			auto result = GetProcAddress(static_cast<HMODULE>(handle), symbol.data());
			if (!result) {
				throw std::runtime_error{(std::string{"can not get symbol : "} + std::string{symbol}).c_str()};
			}
			#endif
			#if defined M_system_linux || defined M_system_macos || defined M_system_android
			auto result = dlsym(handle, symbol.data());
			if (!result) {
				throw std::runtime_error{(std::string{"can not get symbol : "} + std::string{symbol}).c_str()};
			}
			#endif
			return reinterpret_cast<Symbol>(result);
		}

	}

	#pragma endregion

	#pragma region type

	class Library {

	protected: //

		Detail::LibraryHandle m_library;
		Detail::SymbolTable   m_symbol;

	public: //

		#pragma region structor

		~Library (
		) {
			Detail::close_library(thiz.m_library);
		}

		// ----------------

		Library (
		) = delete;

		Library (
			Library const & that
		) = delete;

		Library (
			Library && that
		) = delete;

		// ----------------

		explicit Library (
			std::string_view const & path
		) :
			m_library{},
			m_symbol{} {
			thiz.m_library = Detail::open_library(path);
			thiz.m_symbol = {
				.version = Detail::get_symbol<decltype(TwinKleS::Core::Interface::version) *>(thiz.m_library, Detail::k_symbol_id.version),
				.execute = Detail::get_symbol<decltype(TwinKleS::Core::Interface::execute) *>(thiz.m_library, Detail::k_symbol_id.execute),
			};
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Library const & that
		) -> Library& = delete;

		auto operator = (
			Library && that
		) -> Library& = delete;

		#pragma endregion

		#pragma region interface

		auto version (
		) const -> std::size_t {
			auto result_c = thiz.m_symbol.version();
			return from_size_structure(*result_c);
		}

		auto execute (
			std::string const &              script,
			bool const &                     script_is_file,
			std::vector<std::string> const & argument,
			Core::ShellCallback const &      shell_callback
		) const -> std::optional<std::string> {
			auto script_s = to_string_structure(script);
			auto script_is_file_s = to_boolean_structure(script_is_file);
			auto argument_s = alloc_string_list_structure(argument);
			auto result_s = thiz.m_symbol.execute(script_s, script_is_file_s, argument_s, shell_callback);
			free_string_list_structure(argument_s);
			return result_s ? std::make_optional<std::string>(from_string_structure(*result_s)) : std::nullopt;
		}

		#pragma endregion

	};

	#pragma endregion

}
