module;

#include "shell/common.hpp"

export module twinning.shell.utility.interaction;
import twinning.shell.utility.string;
import twinning.shell.third.system.posix;
import twinning.shell.third.system.windows;
import twinning.shell.third.tinyfiledialogs;

export namespace Twinning::Shell::Interaction {

	#pragma region console

	inline auto input_text (
	) -> std::string {
		auto text = std::string{};
		#if defined M_system_windows
		auto state_b = Third::system::windows::$BOOL{};
		auto handle = Third::system::windows::$GetStdHandle(Third::system::windows::$STD_INPUT_HANDLE);
		auto handle_mode = Third::system::windows::$DWORD{};
		if (Third::system::windows::$GetConsoleMode(handle, &handle_mode)) {
			auto text_16 = std::array<char16_t, 0x1000>{};
			auto length = Third::system::windows::$DWORD{};
			state_b = Third::system::windows::$ReadConsoleW(handle, text_16.data(), static_cast<Third::system::windows::$DWORD>(text_16.size()), &length, nullptr);
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
		auto state_b = Third::system::windows::$BOOL{};
		auto handle = Third::system::windows::$GetStdHandle(Third::system::windows::$STD_OUTPUT_HANDLE);
		auto handle_mode = Third::system::windows::$DWORD{};
		if (Third::system::windows::$GetConsoleMode(handle, &handle_mode)) {
			auto text_16 = utf8_to_utf16(reinterpret_cast<std::u8string const &>(text));
			state_b = Third::system::windows::$WriteConsoleW(handle, text_16.data(), static_cast<Third::system::windows::$DWORD>(text_16.size()), nullptr, nullptr);
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

	inline auto pick_storage_item (
		std::string const & type
	) -> std::optional<std::string> {
		auto target = std::optional<std::string>{};
		assert_test(type == "load_file"sv || type == "load_directory"sv || type == "save_file"sv);
		#if defined M_system_windows
		auto state_h = Third::system::windows::$HRESULT{};
		Third::system::windows::$CoInitialize(nullptr);
		auto dialog = std::add_pointer_t<Third::system::windows::$IFileDialog>{nullptr};
		if (type == "load_file"sv || type == "load_directory"sv) {
			state_h = Third::system::windows::$CoCreateInstance(Third::system::windows::$CLSID_FileOpenDialog, nullptr, Third::system::windows::$CLSCTX_INPROC_SERVER, __uuidof(*dialog), Third::system::windows::$IID_PPV_ARGS_Helper(&dialog));
		}
		if (type == "save_file"sv) {
			state_h = Third::system::windows::$CoCreateInstance(Third::system::windows::$CLSID_FileSaveDialog, nullptr, Third::system::windows::$CLSCTX_INPROC_SERVER, __uuidof(*dialog), Third::system::windows::$IID_PPV_ARGS_Helper(&dialog));
		}
		assert_test(state_h == Third::system::windows::$S_OK);
		auto option = Third::system::windows::$FILEOPENDIALOGOPTIONS{};
		option |= Third::system::windows::$FOS_NOCHANGEDIR | Third::system::windows::$FOS_NOVALIDATE | Third::system::windows::$FOS_NODEREFERENCELINKS | Third::system::windows::$FOS_DONTADDTORECENT | Third::system::windows::$FOS_FORCESHOWHIDDEN;
		if (type == "load_file"sv || type == "load_directory"sv) {
			option |= Third::system::windows::$FOS_PATHMUSTEXIST | Third::system::windows::$FOS_FILEMUSTEXIST;
		}
		if (type == "load_directory"sv) {
			option |= Third::system::windows::$FOS_PICKFOLDERS;
		}
		if (type == "save_file"sv) {
			option |= Third::system::windows::$FOS_PATHMUSTEXIST | Third::system::windows::$FOS_OVERWRITEPROMPT;
		}
		state_h = dialog->SetOptions(option);
		assert_test(state_h == Third::system::windows::$S_OK);
		state_h = dialog->Show(Third::system::windows::$GetForegroundWindow());
		assert_test(state_h == Third::system::windows::$S_OK || state_h == Third::system::windows::$HRESULT_FROM_WIN32(Third::system::windows::$ERROR_CANCELLED));
		if (state_h == Third::system::windows::$S_OK) {
			assert_test(state_h == Third::system::windows::$S_OK);
			auto result_item = std::add_pointer_t<Third::system::windows::$IShellItem>{nullptr};
			state_h = dialog->GetResult(&result_item);
			assert_test(state_h == Third::system::windows::$S_OK);
			auto display_name = Third::system::windows::$LPWSTR{nullptr};
			state_h = result_item->GetDisplayName(Third::system::windows::$SIGDN_FILESYSPATH, &display_name);
			assert_test(state_h == Third::system::windows::$S_OK || state_h == Third::system::windows::$E_INVALIDARG);
			if (state_h == Third::system::windows::$S_OK) {
				auto display_name_8 = utf16_to_utf8(std::u16string_view{reinterpret_cast<char16_t const *>(display_name)});
				std::replace(display_name_8.begin(), display_name_8.end(), u8'\\', u8'/');
				target.emplace(std::move(reinterpret_cast<std::string &>(display_name_8)));
				Third::system::windows::$CoTaskMemFree(display_name);
			}
			result_item->Release();
		}
		dialog->Release();
		Third::system::windows::$CoUninitialize();
		#endif
		#if defined M_system_linux || defined M_system_macintosh
		auto target_data = std::add_pointer_t<char>{nullptr};
		if (type == "load_file"sv) {
			target_data = Third::tinyfiledialogs::tinyfd_openFileDialog("", nullptr, 0, nullptr, nullptr, false);
		}
		if (type == "load_directory"sv) {
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

	inline auto push_system_notification (
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
