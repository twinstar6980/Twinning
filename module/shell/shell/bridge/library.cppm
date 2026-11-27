module;

#include "shell/common.hpp"

export module twinning.shell.bridge.library;
import twinning.shell.utility.exception;
import twinning.shell.utility.library;
import twinning.shell.bridge.service;

export namespace Twinning::Shell::Bridge {

	#pragma region type

	class Library {

	protected:

		std::intptr_t m_handle;

		std::add_pointer_t<Service> m_symbol;

	public:

		#pragma region constructor

		~Library(
		) {
			if (thiz.state()) {
				thiz.close();
			}
			return;
		}

		// ----------------

		Library(
		) :
			m_handle{-1},
			m_symbol{} {
			return;
		}

		Library(
			Library const & that
		) = delete;

		Library(
			Library && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator =(
			Library const & that
		) -> Library & = delete;

		auto operator =(
			Library && that
		) -> Library & = delete;

		#pragma endregion

		#pragma region access

		auto state(
		) -> bool {
			return thiz.m_symbol != nullptr;
		}

		// ----------------

		auto imbue(
			Service * const & symbol
		) -> void {
			assert_test(!thiz.state());
			assert_test(symbol != nullptr);
			thiz.m_symbol = symbol;
			try {
				thiz.m_symbol->initialize();
			}
			catch (...) {
				// ignored
			}
			return;
		}

		auto open(
			std::string_view const & path
		) -> void {
			assert_test(!thiz.state());
			auto handle = Shell::Library::open(path);
			auto symbol = std::add_pointer_t<Service>{nullptr};
			try {
				symbol = Shell::Library::lookup<Service>(handle, "_ZN8Twinning6Kernel9Interface7serviceE"sv);
			}
			catch (...) {
				Shell::Library::close(handle);
				throw;
			}
			thiz.m_handle = handle;
			thiz.m_symbol = symbol;
			try {
				thiz.m_symbol->initialize();
			}
			catch (...) {
				// ignored
			}
			return;
		}

		auto close(
		) -> void {
			assert_test(thiz.state());
			try {
				thiz.m_symbol->finalize();
			}
			catch (...) {
				// ignored
			}
			thiz.m_symbol = nullptr;
			if (thiz.m_handle != -1) {
				Shell::Library::close(thiz.m_handle);
				thiz.m_handle = -1;
			}
			return;
		}

		// ----------------

		auto symbol(
		) -> Service & {
			assert_test(thiz.state());
			return *thiz.m_symbol;
		}

		#pragma endregion

	};

	#pragma endregion

}
