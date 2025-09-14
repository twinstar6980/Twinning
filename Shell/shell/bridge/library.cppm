module;

#include "shell/common.hpp"

export module twinning.shell.bridge.library;
import twinning.shell.utility.library;
import twinning.shell.bridge.service;

export namespace Twinning::Shell::Bridge {

	#pragma region type

	class Library {

	protected:

		LibraryLoader m_handle;

		Service * m_symbol;

	public:

		#pragma region constructor

		~Library (
		) {
			thiz.m_symbol->finalize();
			if (thiz.m_handle.state()) {
				thiz.m_handle.close();
			}
			return;
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
			m_handle{},
			m_symbol{} {
			thiz.m_handle.open(path);
			thiz.m_symbol = thiz.m_handle.lookup<Service>("_ZN8Twinning6Kernel9Interface7serviceE");
			thiz.m_symbol->initialize();
			return;
		}

		explicit Library (
			Service * const & symbol
		) :
			m_handle{},
			m_symbol{} {
			thiz.m_symbol = symbol;
			thiz.m_symbol->initialize();
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Library const & that
		) -> Library & = delete;

		auto operator = (
			Library && that
		) -> Library & = delete;

		#pragma endregion

		#pragma region access

		auto symbol (
		) -> Service & {
			return *thiz.m_symbol;
		}

		#pragma endregion

	};

	#pragma endregion

}
