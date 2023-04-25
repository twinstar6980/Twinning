#pragma once

#define M_version 29

#pragma warning(push)
#pragma warning(disable:4191)
#pragma warning(disable:4265)
#pragma warning(disable:4267)
#pragma warning(disable:4324)
#pragma warning(disable:4365)
#pragma warning(disable:4625)
#pragma warning(disable:4626)
#pragma warning(disable:5026)
#pragma warning(disable:5027)
#pragma warning(disable:5039)
#pragma warning(disable:5204)
#pragma warning(disable:5220)

#include <string>
#include <regex>
#include <optional>
#include <array>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <Windows.h>
#include <ShObjIdl_core.h>
#include <wrl/module.h>
#include <wrl/implements.h>
#include <wrl/client.h>
#include <wil/resource.h>

#pragma warning(pop)

namespace WRL = Microsoft::WRL;

#define thiz (*this)

#define assert_test(...)\
	if (!(__VA_ARGS__)) {\
		throw std::runtime_error{"assertion failed : " #__VA_ARGS__};\
	}\
	static_assert(true)

namespace TwinStar::ForwarderForWindows {

	inline auto get_register_value_dword (
		HKEY const &         key_parent,
		std::wstring const & key_path,
		std::wstring const & value_name
	) -> std::optional<std::uint32_t> {
		auto state_s = LSTATUS{};
		auto data_size = DWORD{};
		state_s = RegGetValueW(key_parent, key_path.data(), value_name.data(), RRF_RT_REG_DWORD, nullptr, nullptr, &data_size);
		if (state_s != ERROR_SUCCESS) {
			if (state_s != ERROR_FILE_NOT_FOUND) {
				throw std::runtime_error{std::format("^ RegGetValueW : {}", GetLastError())};
			}
		}
		auto value = std::optional<std::uint32_t>{};
		if (data_size != 0) {
			auto & data = value.emplace();
			state_s = RegGetValueW(key_parent, key_path.data(), value_name.data(), RRF_RT_REG_DWORD, nullptr, &data, &data_size);
			assert_test(state_s == ERROR_SUCCESS);
		}
		return value;
	}

	inline auto get_register_value_string (
		HKEY const &         key_parent,
		std::wstring const & key_path,
		std::wstring const & value_name
	) -> std::optional<std::wstring> {
		auto state_s = LSTATUS{};
		auto data_size = DWORD{};
		state_s = RegGetValueW(key_parent, key_path.data(), value_name.data(), RRF_RT_REG_SZ, nullptr, nullptr, &data_size);
		if (state_s != ERROR_SUCCESS) {
			if (state_s != ERROR_FILE_NOT_FOUND) {
				throw std::runtime_error{std::format("^ RegGetValueW : {}", GetLastError())};
			}
		}
		auto value = std::optional<std::wstring>{};
		if (data_size != 0) {
			auto & data = value.emplace();
			data.resize(data_size / sizeof(wchar_t) - 1);
			state_s = RegGetValueW(key_parent, key_path.data(), value_name.data(), RRF_RT_REG_SZ, nullptr, data.data(), &data_size);
			assert_test(state_s == ERROR_SUCCESS);
			data.resize(data.size() - 1);
		}
		return value;
	}

	// ----------------

	// NOTE : the first string of command line is program path, should not encode as argument string, and must use '\' be path separator
	inline auto encode_command_line_program_string (
		std::wstring const & source,
		std::wstring &       destination
	) -> void {
		destination.push_back(L'"');
		for (auto & element : source) {
			if (element == L'/') {
				destination.push_back(L'\\');
			} else {
				destination.push_back(element);
			}
		}
		destination.push_back(L'"');
		return;
	}

	// NOTE : see https://learn.microsoft.com/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170
	inline auto encode_command_line_argument_string (
		std::wstring const & source,
		std::wstring &       destination
	) -> void {
		auto current_backslash_count = 0;
		destination.push_back(L'"');
		for (auto & element : source) {
			if (element == L'"') {
				for (auto index = 0; index < current_backslash_count; ++index) {
					destination.push_back(L'\\');
				}
				destination.push_back(L'\\');
			}
			destination.push_back(element);
			if (element == L'\\') {
				++current_backslash_count;
			} else {
				current_backslash_count = 0;
			}
		}
		for (auto index = 0; index < current_backslash_count; ++index) {
			destination.push_back(L'\\');
		}
		destination.push_back(L'"');
		return;
	}

	inline auto encode_command_line_string (
		std::wstring const &              program,
		std::vector<std::wstring> const & argument
	) -> std::wstring {
		auto destination_size = std::size_t{0};
		destination_size += 256 + 2;
		for (auto & element : argument) {
			destination_size += element.size() * 2 + 2;
		}
		auto destination = std::wstring{};
		destination.reserve(destination_size);
		encode_command_line_program_string(program, destination);
		for (auto & element : argument) {
			destination.push_back(L' ');
			encode_command_line_argument_string(element, destination);
		}
		return destination;
	}

	// ----------------

	inline auto create_process (
		std::wstring const &              program,
		std::vector<std::wstring> const & argument
	) -> void {
		auto state_b = BOOL{};
		auto program_string = program;
		auto argument_string = encode_command_line_string(program, argument);
		auto startup_information = STARTUPINFOW{
			.cb = sizeof(STARTUPINFO),
			.dwFlags = STARTF_USESTDHANDLES,
			.hStdInput = GetStdHandle(STD_INPUT_HANDLE),
			.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE),
			.hStdError = GetStdHandle(STD_ERROR_HANDLE),
		};
		auto process_information = PROCESS_INFORMATION{
		};
		state_b = CreateProcessW(
			program_string.data(),
			argument_string.data(),
			nullptr,
			nullptr,
			TRUE,
			CREATE_UNICODE_ENVIRONMENT | DETACHED_PROCESS,
			nullptr,
			nullptr,
			&startup_information,
			&process_information
		);
		assert_test(state_b);
		return;
	}

	// ----------------

	inline auto get_shell_item_file_path (
		IShellItemArray * const & selection
	) -> std::vector<std::wstring> {
		auto state_h = HRESULT{};
		auto result = std::vector<std::wstring>{};
		assert_test(selection != nullptr);
		auto count = DWORD{};
		state_h = selection->GetCount(&count);
		assert_test(state_h == S_OK);
		result.reserve(count);
		for (auto i = DWORD{0}; i < count; ++i) {
			auto item = std::add_pointer_t<IShellItem>{};
			state_h = selection->GetItemAt(i, &item);
			assert_test(state_h == S_OK);
			auto name = LPWSTR{};
			state_h = item->GetDisplayName(SIGDN_FILESYSPATH, &name);
			assert_test(state_h == S_OK);
			item->Release();
			result.emplace_back(name);
			CoTaskMemFree(name);
		}
		return result;
	}

	// ----------------

	inline auto get_module_file_name (
		HMODULE const & handle
	) -> std::wstring {
		auto state_d = DWORD{};
		auto buffer = std::array<wchar_t, 1024>{};
		state_d = GetModuleFileNameW(handle, buffer.data(), static_cast<DWORD>(buffer.size()));
		assert_test(state_d != 0 && state_d != buffer.size());
		// TODO : maybe ERROR_INSUFFICIENT_BUFFER even if function succeeded
		return std::wstring{buffer.data(), state_d};
	}

	// ----------------

	inline auto const k_register_key_parent = HKEY{HKEY_CURRENT_USER};

	inline auto const k_register_key_path = std::wstring{L"Software\\TwinStar\\ToolKit\\ForwarderForWindows"};

	inline auto g_dll_handle = HMODULE{};

}
