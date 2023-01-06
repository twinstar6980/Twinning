#pragma once

#include "core/utility/string/string.hpp"

#if defined M_system_windows
#include "core/utility/string/encoding.hpp"
#include "core/third_party/system/windows.hpp"
#endif

namespace TwinStar::Core {

	#pragma region utility

	inline auto message_of_std_exception (
		std::exception const & it
	) -> String {
		return make_string(it.what());
	}

	inline auto message_of_std_error_code (
		std::error_code const & it
	) -> String {
		#if defined M_system_windows
		// NOTE : on windows, the error_code message is ANSI encoding
		// NOTE : duplicate of System::Windows::encoding_ansi_to_utf8
		auto encoding_ansi_to_utf8 =
			[] (
			CBasicStringView<Character> const & source
		) -> BasicString<Character8> {
			auto text_16 = Array<Character16>{source.size()};
			auto text_size_16 = mbw<Size>(
				MultiByteToWideChar(
					CP_ACP,
					MB_ERR_INVALID_CHARS,
					cast_pointer<CHAR>(source.begin()).value,
					static_cast<int>(source.size().value),
					cast_pointer<WCHAR>(text_16.begin()).value,
					static_cast<int>(text_16.size().value)
				)
			);
			return StringEncoding::utf16_to_utf8(CBasicStringView<Character16>{text_16.begin(), text_size_16});
		};
		return self_cast<String>(encoding_ansi_to_utf8(make_string_view(it.message())));
		#else
		return make_string(it.message());
		#endif
	}

	#pragma endregion

}
