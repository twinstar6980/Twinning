#pragma once

#include "shell/base.hpp"
#include "shell/host/host.hpp"
#include <iostream>

namespace TwinKleS::Shell {

	#pragma region type

	class CLIHost :
		public Host {

	protected: //

		bool m_running;

	public: //

		#pragma region structor

		virtual ~CLIHost (
		) override {
		}

		// ----------------

		CLIHost (
		) = delete;

		CLIHost (
			CLIHost const & that
		) = delete;

		CLIHost (
			CLIHost && that
		) = delete;

		// ----------------

		explicit CLIHost (
			std::nullptr_t _
		):
			m_running{false} {
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			CLIHost const & that
		) -> CLIHost& = delete;

		auto operator = (
			CLIHost && that
		) -> CLIHost& = delete;

		#pragma endregion

		#pragma region life-time

		virtual auto start (
		) -> void override {
			assert_condition(!thiz.m_running);
			thiz.m_running = true;
			return;
		}

		virtual auto finish (
		) -> void override {
			assert_condition(thiz.m_running);
			thiz.m_running = false;
			return;
		}

		#pragma endregion

		#pragma region callback

		virtual auto callback (
			std::vector<std::string> const & argument
		) -> std::vector<std::string> override {
			assert_condition(thiz.m_running);
			auto result = std::vector<std::string>{};
			auto method = argument[0];
			switch (hash_string(method)) {
				default : {
					throw std::runtime_error{"invalid method"s};
					break;
				}
				case hash_string("name"sv) : {
					auto name = thiz.name();
					result.emplace_back(std::move(name));
					break;
				}
				case hash_string("output"sv) : {
					auto text = argument[1];
					thiz.output(text);
					break;
				}
				case hash_string("input"sv) : {
					auto text = thiz.input();
					result.emplace_back(std::move(text));
					break;
				}
					#if defined M_system_windows
				case hash_string("open_file_dialog"sv) : {
					auto pick_folder = string_to_boolean(argument[1]);
					auto multiple = string_to_boolean(argument[2]);
					auto selected_path = thiz.open_file_dialog(pick_folder, multiple);
					result.insert(result.end(), std::make_move_iterator(selected_path.begin()), std::make_move_iterator(selected_path.end()));
					break;
				}
					#endif
			}
			return result;
		}

		#pragma endregion

		#pragma region callback implement

		auto name (
		) -> std::string {
			#if defined M_system_windows
			auto name = "windows.cli"s;
			#endif
			#if defined M_system_linux
			auto name = "linux.cli"s;
			#endif
			#if defined M_system_macos
			auto name = "macos.cli"s;
			#endif
			#if defined M_system_android
			auto name = "android.cli"s;
			#endif
			#if defined M_system_ios
			auto name = "ios.cli"s;
			#endif
			return name;
		}

		// ----------------

		auto output (
			std::string const & text
		) -> void {
			#if defined M_system_windows
			auto text_16 = utf8_to_utf16(reinterpret_cast<std::u8string const &>(text));
			auto length = DWORD{};
			auto state = WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), text_16.data(), static_cast<DWORD>(text_16.size()), &length, nullptr);
			assert_condition(state);
			#endif
			#if defined M_system_linux || defined M_system_macos || defined M_system_android || defined M_system_ios
			std::cout << text << std::flush;
			#endif
			return;
		}

		auto input (
		) -> std::string {
			#if defined M_system_windows
			auto text_16 = std::array<char16_t, 0x1000>{};
			auto length = DWORD{};
			auto state = ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), text_16.data(), static_cast<DWORD>(text_16.size()), &length, nullptr);
			assert_condition(state);
			auto text_8 = utf16_to_utf8(std::u16string_view{text_16.data(), length - 2});
			return std::string{std::move(reinterpret_cast<std::string &>(text_8))};
			#endif
			#if defined M_system_linux || defined M_system_macos || defined M_system_android || defined M_system_ios
			auto text = std::string{};
			std::getline(std::cin, text);
			return text;
			#endif
		}

		// ----------------

		#if defined M_system_windows

		auto open_file_dialog (
			bool const & pick_folder,
			bool const & multiple
		) -> std::vector<std::string> {
			return Windows::open_file_dialog(pick_folder, multiple);
		}

		#endif

		#pragma endregion

	};

	#pragma endregion

}
