module;

#include "shell/common.hpp"

export module twinning.shell.utility.library;
import twinning.shell.utility.system_native_string;
import twinning.shell.dependency.system.win32;
import twinning.shell.dependency.system.posix;

export namespace Twinning::Shell::Library {

	#pragma region utility

	inline auto open(
		std::string_view const & path
	) -> std::intptr_t {
		auto handle = std::intptr_t{-1};
		#if defined M_system_windows
		auto native_handle = Dependency::system::win32::$HMODULE{};
		native_handle = Dependency::system::win32::$LoadLibraryW(
			SystemNativeString::to_native(std::string{path} + "."s, true).data()
		);
		if (native_handle != nullptr) {
			handle = reinterpret_cast<std::intptr_t>(native_handle);
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto native_handle = std::add_pointer_t<void>{};
		native_handle = Dependency::system::posix::$dlopen(
			SystemNativeString::to_native(path, true).data(),
			Dependency::system::posix::$RTLD_LAZY | Dependency::system::posix::$RTLD_LOCAL
		);
		if (native_handle != nullptr) {
			handle = reinterpret_cast<std::intptr_t>(native_handle);
		}
		#endif
		if (handle == -1) {
			throw std::runtime_error{std::format("Exception: could not open library '{}'"sv, path)};
		}
		return handle;
	}

	inline auto close(
		std::intptr_t const & handle
	) -> void {
		auto state = bool{};
		#if defined M_system_windows
		auto native_handle = reinterpret_cast<Dependency::system::win32::$HMODULE>(handle);
		state = Dependency::system::win32::$FreeLibrary(
			native_handle
		) != Dependency::system::win32::$FALSE;
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto native_handle = reinterpret_cast<std::add_pointer_t<void>>(handle);
		state = Dependency::system::posix::$dlclose(
			native_handle
		) == 0;
		#endif
		if (state == false) {
			throw std::runtime_error{std::format("Exception: could not close library"sv)};
		}
		return;
	}

	// ----------------

	template <typename TSymbol>
	inline auto lookup(
		std::intptr_t const &    handle,
		std::string_view const & name
	) -> TSymbol * {
		auto symbol = std::add_pointer_t<void>{};
		#if defined M_system_windows
		auto native_handle = reinterpret_cast<Dependency::system::win32::$HMODULE>(handle);
		symbol = reinterpret_cast<void *>(Dependency::system::win32::$GetProcAddress(
			native_handle,
			SystemNativeString::to_generic(name, true).data()
		));
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto native_handle = reinterpret_cast<std::add_pointer_t<void>>(handle);
		symbol = Dependency::system::posix::$dlsym(
			native_handle,
			SystemNativeString::to_generic(name, true).data()
		);
		#endif
		if (symbol == nullptr) {
			throw std::runtime_error{std::format("Exception: could not lookup symbol '{}'"sv, name)};
		}
		return static_cast<TSymbol *>(symbol);
	}

	#pragma endregion

}
