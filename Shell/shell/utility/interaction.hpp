#pragma once

#include "shell/common.hpp"
#include "shell/utility/exception.hpp"
#include "shell/utility/string.hpp"
#include "shell/third/system/windows.hpp"
#include "shell/third/system/posix.hpp"
#include "shell/third/tinyfiledialogs.hpp"

namespace TwinStar::Shell::Interaction {

	#pragma region console

	inline auto input_text (
	) -> std::string {
		auto text = std::string{};
		#if defined M_system_windows
		auto state_b = BOOL{};
		auto handle = GetStdHandle(STD_INPUT_HANDLE);
		auto handle_mode = DWORD{};
		if (GetConsoleMode(handle, &handle_mode)) {
			auto text_16 = std::array<char16_t, 0x1000>{};
			auto length = DWORD{};
			state_b = ReadConsoleW(handle, text_16.data(), static_cast<DWORD>(text_16.size()), &length, nullptr);
			assert_test(state_b);
			auto text_8 = utf16_to_utf8(std::u16string_view{text_16.data(), length - 2});
			text = std::move(reinterpret_cast<std::string &>(text_8));
		}
		else {
			std::getline(std::cin, text);
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		std::getline(std::cin, text);
		#endif
		return text;
	}

	inline auto output_text (
		std::string const & text
	) -> void {
		#if defined M_system_windows
		auto state_b = BOOL{};
		auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
		auto handle_mode = DWORD{};
		if (GetConsoleMode(handle, &handle_mode)) {
			auto text_16 = utf8_to_utf16(reinterpret_cast<std::u8string const &>(text));
			state_b = WriteConsoleW(handle, text_16.data(), static_cast<DWORD>(text_16.size()), nullptr, nullptr);
			assert_test(state_b);
		}
		else {
			std::cout << text << std::flush;
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		std::cout << text << std::flush;
		#endif
		return;
	}

	#pragma endregion

	#pragma region shell

	inline auto pick_path (
		std::string const & type
	) -> std::optional<std::string> {
		auto target = std::optional<std::string>{};
		assert_test(type == "open_file"sv || type == "open_directory"sv || type == "save_file"sv);
		#if defined M_system_windows
		auto state_h = HRESULT{};
		CoInitialize(nullptr);
		auto dialog = std::add_pointer_t<IFileDialog>{nullptr};
		if (type == "open_file"sv || type == "open_directory"sv) {
			state_h = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));
		}
		if (type == "save_file"sv) {
			state_h = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));
		}
		assert_test(state_h == S_OK);
		auto option = FILEOPENDIALOGOPTIONS{};
		option |= FOS_NOCHANGEDIR | FOS_FORCEFILESYSTEM | FOS_NOVALIDATE | FOS_NODEREFERENCELINKS | FOS_DONTADDTORECENT | FOS_FORCESHOWHIDDEN;
		if (type == "open_file"sv || type == "open_directory"sv) {
			option |= FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST;
		}
		if (type == "open_directory"sv) {
			option |= FOS_PICKFOLDERS;
		}
		if (type == "save_file"sv) {
			option |= FOS_PATHMUSTEXIST | FOS_OVERWRITEPROMPT;
		}
		state_h = dialog->SetOptions(option);
		assert_test(state_h == S_OK);
		state_h = dialog->Show(GetForegroundWindow());
		if (state_h != HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
			assert_test(state_h == S_OK);
			auto result_item = std::add_pointer_t<IShellItem>{nullptr};
			state_h = dialog->GetResult(&result_item);
			assert_test(state_h == S_OK);
			auto display_name = LPWSTR{nullptr};
			state_h = result_item->GetDisplayName(SIGDN_FILESYSPATH, &display_name);
			assert_test(state_h == S_OK);
			auto display_name_8 = utf16_to_utf8(std::u16string_view{reinterpret_cast<char16_t const *>(display_name)});
			std::replace(display_name_8.begin(), display_name_8.end(), u8'\\', u8'/');
			target.emplace(std::move(reinterpret_cast<std::string &>(display_name_8)));
			CoTaskMemFree(display_name);
			result_item->Release();
		}
		dialog->Release();
		CoUninitialize();
		#endif
		#if defined M_system_linux || defined M_system_macintosh
		auto target_data = std::add_pointer_t<char>{nullptr};
		if (type == "open_file"sv) {
			target_data = Third::tinyfiledialogs::tinyfd_openFileDialog("", nullptr, 0, nullptr, nullptr, false);
		}
		if (type == "open_directory"sv) {
			target_data = Third::tinyfiledialogs::tinyfd_selectFolderDialog("", nullptr);
		}
		if (type == "save_file"sv) {
			target_data = Third::tinyfiledialogs::tinyfd_saveFileDialog("", nullptr, 0, nullptr, nullptr);
		}
		if (target_data != nullptr) {
			auto target_value = std::string{target_data};
			#if defined M_system_macintosh
			if (target_value.size() > 1 && target_value.back() == '/') {
				target_value.pop_back();
			}
			#endif
			target.emplace(std::move(target_value));
		}
		#endif
		#if defined M_system_android || defined M_system_iphone
		throw std::runtime_error{"unavailable"s};
		#endif
		return target;
	}

	inline auto push_notification (
		std::string const & title,
		std::string const & description
	) -> void {
		#if defined M_system_windows || defined M_system_linux || defined M_system_macintosh
		Third::tinyfiledialogs::tinyfd_notifyPopup(title.data(), description.data(), "info");
		#endif
		#if defined M_system_android || defined M_system_iphone
		throw std::runtime_error{"unavailable"s};
		#endif
		return;
	}

	#pragma endregion

}
