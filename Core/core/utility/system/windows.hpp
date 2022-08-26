#pragma once

#if defined M_system_windows

#include "core/utility/string/string.hpp"
#include "core/utility/string/encoding.hpp"
#include "core/utility/file_system/path.hpp"
#include "core/third_party/system/windows.hpp"

namespace TwinKleS::Core::System::Windows {

	#pragma region control

	// NOTE : [[noreturn]]
	inline auto exit (
		IntegerS32 const & code
	) -> Void {
		std::exit(code.value);
		return;
	}

	inline auto sleep (
		Size const & time
	) -> Void {
		Sleep(static_cast<DWORD>(time.value));
		return;
	}

	#pragma endregion

	#pragma region command

	inline auto system (
		String const & command
	) -> IntegerS32 {
		return mbw<IntegerS32>(std::system(cast_pointer<char>(make_null_terminated_string(command).begin()).value));
	}

	// todo : bug if argument value has \" character
	inline auto process (
		Path const &         path,
		List<String> const & argument
	) -> IntegerS32 {
		auto path_string = path.to_string();
		auto argument_string = [&] {
			auto it = String{
				path_string.size() + 2_sz +
				Range::accumulate<Size>(
					argument,
					[] (auto & element) -> auto {
						return element.size() + 3_sz;
					}
				)
			};
			it.append('"'_c);
			it.append_list(path_string);
			it.append('"'_c);
			for (auto & element : argument) {
				it.append(' '_c);
				it.append('"'_c);
				it.append_list(element);
				it.append('"'_c);
			}
			return it;
		}();
		auto state = BOOL{};
		auto startup_info = STARTUPINFOW{};
		auto process_information = PROCESS_INFORMATION{};
		ZeroMemory(&startup_info, sizeof(startup_info));
		ZeroMemory(&process_information, sizeof(process_information));
		state = CreateProcessW(
			cast_pointer<WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>(path.to_string()))).begin()).value,
			cast_pointer<WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<CBasicStringView<Character8>>(argument_string))).begin()).value,
			nullptr,
			nullptr,
			FALSE,
			CREATE_UNICODE_ENVIRONMENT,
			nullptr,
			nullptr,
			&startup_info,
			&process_information
		);
		assert_condition(state);
		auto waiting_state = WaitForSingleObject(process_information.hProcess, INFINITE);
		assert_condition(waiting_state == WAIT_OBJECT_0);
		auto result = DWORD{};
		state = GetExitCodeProcess(process_information.hProcess, &result);
		assert_condition(state);
		return mbw<IntegerS32>(result);
	}

	#pragma endregion

	#pragma region encoding

	// todo : remove ?
	inline auto encoding_ansi_to_utf8 (
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
	}

	#pragma endregion

}

#endif
