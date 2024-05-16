#pragma once

#include "shell/common.hpp"
#include "shell/utility/library.hpp"
#include "shell/bridge/service.hpp"

namespace TwinStar::Shell::Bridge {

	#pragma region type

	class Library {

	protected:

		LibraryLoader m_handle;

		Service * m_symbol;

	public:

		#pragma region structor

		~Library (
		) {
			if (thiz.m_handle.state()) {
				thiz.m_handle.close();
			}
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
			thiz.m_symbol = thiz.m_handle.lookup<Service>(
				#if defined M_system_windows
				"?service@Interface@Kernel@TwinStar@@3UService@123@A"
				#endif
				#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
				"_ZN8TwinStar6Kernel9Interface7serviceE"
				#endif
			);
		}

		explicit Library (
			void * const & symbol
		) :
			m_handle{},
			m_symbol{} {
			thiz.m_symbol = static_cast<Service *>(symbol);
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
		) const -> Service const & {
			return *thiz.m_symbol;
		}

		#pragma endregion

	};

	#pragma endregion

}
