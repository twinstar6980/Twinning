#pragma once

#include "./base.hpp"

#include <cstdlib>
#include <array>
#include <iostream>

#if defined M_system_windows
#include "Windows.h"
#include "ShObjIdl_core.h"
#endif

namespace TwinKleS::Shell::Callback::CLI {

	#pragma region detail

	namespace Detail {

		#pragma region basic interaction

		inline auto input (
		) -> std::string {
			#if defined M_system_windows
			thread_local auto text_16 = std::array<char16_t, 0x1000>{};
			auto              length = DWORD{};
			auto              state = ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), text_16.data(), static_cast<DWORD>(text_16.size()), &length, nullptr);
			assert_condition(state);
			auto text_8 = utf16_to_utf8(std::u16string_view{text_16.data(), length - 2});
			return std::string{std::move(reinterpret_cast<std::string &>(text_8))};
			#endif
			#if defined M_system_linux || defined M_system_macos || defined M_system_android
			auto text = std::string{};
			std::getline(std::cin, text);
			return text;
			#endif
		}

		inline auto output (
			std::string_view const & text
		) -> void {
			#if defined M_system_windows
			auto text_16 = utf8_to_utf16(reinterpret_cast<std::u8string_view const &>(text));
			auto length = DWORD{};
			auto state = WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), text_16.data(), static_cast<DWORD>(text_16.size()), &length, nullptr);
			assert_condition(state);
			#endif
			#if defined M_system_linux || defined M_system_macos || defined M_system_android
			std::cout << text;
			#endif
			return;
		}

		inline auto pause (
			std::string_view const & message
		) -> void {
			#if defined M_system_windows
			std::system("pause");
			#endif
			#if defined M_system_linux || defined M_system_macos || defined M_system_android
			std::system("echo pause... && read _");
			#endif
			return;
		}

		#pragma endregion

		#pragma region windows only

		#if defined M_system_windows

		// NOTE : if arch == arm_32 : compile failed : undefined symbol "CoInitialize"...
		inline auto select_file_by_dialog (
			bool const & pick_folder,
			bool const & multiple
		) -> std::vector<std::string> {
			auto result = std::vector<std::string>{};
			CoInitialize(nullptr);
			auto dialog = X<IFileOpenDialog *>{nullptr};
			auto h_result = CoCreateInstance(__uuidof(FileOpenDialog), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));
			assert_condition(SUCCEEDED(h_result));
			auto option = FILEOPENDIALOGOPTIONS{};
			h_result = dialog->GetOptions(&option);
			assert_condition(SUCCEEDED(h_result));
			option |= FOS_NOCHANGEDIR | FOS_FORCEFILESYSTEM | FOS_NODEREFERENCELINKS | FOS_DONTADDTORECENT | FOS_FORCESHOWHIDDEN;
			if (pick_folder) {
				option |= FOS_PICKFOLDERS;
			}
			if (multiple) {
				option |= FOS_ALLOWMULTISELECT;
			}
			h_result = dialog->SetOptions(option);
			assert_condition(SUCCEEDED(h_result));
			h_result = dialog->Show(nullptr);
			if (h_result != HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
				assert_condition(SUCCEEDED(h_result));
				auto selected_item_list = X<IShellItemArray *>{nullptr};
				h_result = dialog->GetResults(&selected_item_list);
				assert_condition(SUCCEEDED(h_result));
				auto item_count = DWORD{0};
				h_result = selected_item_list->GetCount(&item_count);
				assert_condition(SUCCEEDED(h_result));
				result.reserve(item_count);
				for (auto index = DWORD{0}; index < item_count; index++) {
					auto item = X<IShellItem *>{nullptr};
					auto display_name = LPWSTR{nullptr};
					h_result = selected_item_list->GetItemAt(index, &item);
					assert_condition(SUCCEEDED(h_result));
					h_result = item->GetDisplayName(SIGDN_FILESYSPATH, &display_name);
					assert_condition(SUCCEEDED(h_result));
					auto display_name_8 = utf16_to_utf8(std::u16string_view{reinterpret_cast<char16_t const *>(display_name)});
					result.emplace_back(std::move(reinterpret_cast<std::string &>(display_name_8)));
					CoTaskMemFree(display_name);
					item->Release();
				}
				selected_item_list->Release();
			}
			dialog->Release();
			return result;
		}

		// ----------------

		inline auto get_standard_handle_by_name (
			std::string_view const & name
		) -> HANDLE {
			auto id = DWORD{};
			switch (hash_string(name)) {
				default : {
					throw std::runtime_error{"invalid handle name"};
					break;
				}
				case hash_string("input") : {
					id = STD_INPUT_HANDLE;
					break;
				}
				case hash_string("output") : {
					id = STD_OUTPUT_HANDLE;
					break;
				}
				case hash_string("error") : {
					id = STD_ERROR_HANDLE;
					break;
				}
			}
			auto handle = GetStdHandle(id);
			assert_condition(handle != INVALID_HANDLE_VALUE);
			return handle;
		}

		inline auto get_standard_console_mode (
			std::string_view const & handle_name
		) -> std::uint32_t {
			auto handle = get_standard_handle_by_name(handle_name);
			auto mode = DWORD{};
			auto state = GetConsoleMode(handle, &mode);
			assert_condition(state);
			return mode;
		}

		inline auto set_standard_console_mode (
			std::string_view const & handle_name,
			std::uint32_t const &    mode
		) -> void {
			auto handle = get_standard_handle_by_name(handle_name);
			auto state = SetConsoleMode(handle, mode);
			assert_condition(state);
			return;
		}

		#endif

		#pragma endregion

	}

	#pragma endregion

	#pragma region implement

	class ArgumentStream {

	protected: //

		std::vector<std::string_view> const & m_view;
		std::size_t                           m_position;

	public: //

		#pragma region structor

		~ArgumentStream (
		) = default;

		// ----------------

		ArgumentStream (
		) = delete;

		ArgumentStream (
			ArgumentStream const & that
		) = delete;

		ArgumentStream (
			ArgumentStream && that
		) = delete;

		// ----------------

		explicit ArgumentStream (
			std::vector<std::string_view> const & view
		):
			m_view{view},
			m_position{} {
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			ArgumentStream const & that
		) -> ArgumentStream& = delete;

		auto operator = (
			ArgumentStream && that
		) -> ArgumentStream& = delete;

		#pragma endregion

		#pragma region state

		auto full (
		) -> bool {
			return thiz.m_position == thiz.m_view.size();
		}

		#pragma endregion

		#pragma region query

		auto current (
		) -> std::string_view const& {
			assert_condition(!thiz.full());
			return thiz.m_view.at(thiz.m_position);
		}

		auto next (
		) -> std::string_view const& {
			auto & item = thiz.current();
			++thiz.m_position;
			return item;
		}

		auto next_is (
			std::string_view const & value
		) -> bool {
			if (thiz.full()) {
				return false;
			}
			auto & item = thiz.current();
			if (item == value) {
				++thiz.m_position;
				return true;
			}
			return false;
		}

		#pragma endregion

	};

	namespace Method {

		#pragma region shell name

		inline auto name (
			ArgumentStream &           argument,
			std::vector<std::string> & result
		) -> void {
			#if defined M_system_windows
			auto name = "windows.cli";
			#endif
			#if defined M_system_linux
			auto name = "linux.cli";
			#endif
			#if defined M_system_macos
			auto name = "macos.cli";
			#endif
			#if defined M_system_android
			auto name = "android.cli";
			#endif
			result.emplace_back(name);
			return;
		}

		#pragma endregion

		#pragma region basic interaction

		inline auto input (
			ArgumentStream &           argument,
			std::vector<std::string> & result
		) -> void {
			auto text = Detail::input();
			result.emplace_back(std::move(text));
			return;
		}

		inline auto output (
			ArgumentStream &           argument,
			std::vector<std::string> & result
		) -> void {
			auto text = argument.next();
			Detail::output(text);
			return;
		}

		inline auto pause (
			ArgumentStream &           argument,
			std::vector<std::string> & result
		) -> void {
			auto message = argument.next();
			Detail::pause(message);
			return;
		}

		#pragma endregion

		#pragma region windows only

		#if defined M_system_windows

		inline auto select_file_by_dialog (
			ArgumentStream &           argument,
			std::vector<std::string> & result
		) -> void {
			auto pick_folder = argument.next();
			auto multiple = argument.next();
			auto path_list = Detail::select_file_by_dialog(pick_folder == "true", multiple == "true");
			result.insert(result.end(), std::make_move_iterator(path_list.begin()), std::make_move_iterator(path_list.end()));
			return;
		}

		// ----------------

		inline auto get_standard_console_mode (
			ArgumentStream &           argument,
			std::vector<std::string> & result
		) -> void {
			auto handle_name = argument.next();
			auto mode = Detail::get_standard_console_mode(handle_name);
			result.emplace_back(std::to_string(mode));
			return;
		}

		inline auto set_standard_console_mode (
			ArgumentStream &           argument,
			std::vector<std::string> & result
		) -> void {
			auto handle_name = argument.next();
			auto mode = argument.next();
			Detail::set_standard_console_mode(handle_name, static_cast<std::uint32_t>(std::atoll(mode.data())));
			return;
		}

		#endif

		#pragma endregion

	}

	// ----------------

	inline auto execute (
		ArgumentStream &           argument,
		std::vector<std::string> & result
	) noexcept -> void {
		try {
			result.clear();
			result.emplace_back();
			auto method = argument.next();
			switch (hash_string(method)) {
				default : {
					throw std::runtime_error{"invalid method"};
					break;
				}
				case hash_string("name") : {
					Method::name(argument, result);
					break;
				}
				case hash_string("input") : {
					Method::input(argument, result);
					break;
				}
				case hash_string("output") : {
					Method::output(argument, result);
					break;
				}
				case hash_string("pause") : {
					Method::pause(argument, result);
					break;
				}
					#if defined M_system_windows
				case hash_string("select_file_by_dialog") : {
					Method::select_file_by_dialog(argument, result);
					break;
				}
				case hash_string("get_standard_console_mode") : {
					Method::get_standard_console_mode(argument, result);
					break;
				}
				case hash_string("set_standard_console_mode") : {
					Method::set_standard_console_mode(argument, result);
					break;
				}
					#endif
			}
		} catch (std::exception & exception) {
			result.clear();
			result.emplace_back(std::string{} + typeid(exception).name() + " : " + exception.what());
		} catch (...) {
			result.clear();
			result.emplace_back("unknown exception");
		}
		return;
	}

	#pragma endregion

	#pragma region interface

	inline auto shell_callback (
		Core::StringList const & argument
	) noexcept -> Core::StringList const& {
		thread_local auto result_handler = CTypeStringListHandler{};
		auto argument_value = from_string_list_structure(argument);
		auto argument_stream = ArgumentStream{argument_value};
		auto result_value = std::vector<std::string>{};
		execute(argument_stream, result_value);
		result_handler.imbue(alloc_string_list_structure(result_value));
		return result_handler.value();
	}

	#pragma endregion

}
