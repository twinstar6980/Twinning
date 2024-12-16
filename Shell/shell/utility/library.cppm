module;

#include "shell/common.hpp"

export module twinning.shell.utility.library;
import twinning.shell.utility.string;
import twinning.shell.third.system.posix;
import twinning.shell.third.system.windows;

export namespace Twinning::Shell {

	#pragma region type

	class LibraryLoader {

	protected:

		void * m_handle;

	public:

		#pragma region structor

		~LibraryLoader (
		) {
			if (thiz.state()) {
				thiz.close();
			}
		}

		// ----------------

		LibraryLoader (
		) :
			m_handle{} {
		}

		LibraryLoader (
			LibraryLoader const & that
		) = delete;

		LibraryLoader (
			LibraryLoader && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator = (
			LibraryLoader const & that
		) -> LibraryLoader & = delete;

		auto operator = (
			LibraryLoader && that
		) -> LibraryLoader & = delete;

		#pragma endregion

		#pragma region access

		auto state (
		) -> bool {
			return thiz.m_handle != nullptr;
		}

		// ----------------

		auto open (
			std::string_view const & path
		) -> void {
			assert_test(!thiz.state());
			#if defined M_system_windows
			auto path_absolute = std::string{path} + "."s;
			auto path_absolute_16 = utf8_to_utf16(reinterpret_cast<std::u8string const &>(path_absolute));
			thiz.m_handle = static_cast<void *>(Third::system::windows::$LoadLibraryW(reinterpret_cast<Third::system::windows::$LPCWSTR>(path_absolute_16.data())));
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			thiz.m_handle = Third::system::posix::$dlopen(path.data(), Third::system::posix::$RTLD_LAZY | Third::system::posix::$RTLD_LOCAL);
			#endif
			if (thiz.m_handle == nullptr) {
				throw std::runtime_error{"library can not open : "s + path.data()};
			}
			return;
		}

		auto close (
		) -> void {
			assert_test(thiz.state());
			auto state = bool{};
			#if defined M_system_windows
			state = Third::system::windows::$FreeLibrary(static_cast<Third::system::windows::$HMODULE>(thiz.m_handle)) != Third::system::windows::$FALSE;
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			state = Third::system::posix::$dlclose(thiz.m_handle) == 0;
			#endif
			if (state == false) {
				throw std::runtime_error{"library can not close"s};
			}
			thiz.m_handle = nullptr;
			return;
		}

		// ----------------

		template <typename Symbol>
		auto lookup (
			std::string_view const & name
		) -> Symbol * {
			assert_test(thiz.state());
			auto address = std::add_pointer_t<void>{};
			#if defined M_system_windows
			address = reinterpret_cast<void *>(Third::system::windows::$GetProcAddress(static_cast<Third::system::windows::$HMODULE>(thiz.m_handle), name.data()));
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			address = Third::system::posix::$dlsym(thiz.m_handle, name.data());
			#endif
			if (address == nullptr) {
				throw std::runtime_error{"can not get symbol : "s + name.data()};
			}
			return static_cast<Symbol *>(address);
		}

		#pragma endregion

	};

	#pragma endregion

}
