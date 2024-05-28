#pragma once

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

#include <stdexcept>
#include <string>
#include <optional>
#include <array>
#include <vector>
#include <filesystem>
#include <Windows.h>
#include <Shlwapi.h>
#include <ShlObj_core.h>
#include <ShObjIdl_core.h>
#include <wrl/module.h>
#include <wrl/client.h>

#pragma warning(pop)

namespace WRL = Microsoft::WRL;

#define thiz (*this)

#define assert_test(...)\
	if (!(__VA_ARGS__)) {\
		throw std::runtime_error{"assertion failed : " #__VA_ARGS__};\
	}\
	static_assert(true)

namespace Twinning::Forwarder {

	inline auto g_application_name = std::wstring{L"Twinning Forwarder"};

	inline auto g_application_logo = std::wstring{};

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
			}
			else {
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
				current_backslash_count += 1;
			}
			else {
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

	inline auto get_file_long_path (
		std::wstring const & short_path
	) -> std::wstring {
		auto state_d = DWORD{};
		state_d = GetLongPathNameW(short_path.data(), nullptr, 0);
		assert_test(state_d != 0);
		auto result = std::wstring{};
		result.resize(state_d);
		state_d = GetLongPathNameW(short_path.data(), result.data(), static_cast<DWORD>(result.size()));
		assert_test(state_d == result.size() - 1);
		result.resize(result.size() - 1);
		return result;
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
		for (auto index = DWORD{0}; index < count; ++index) {
			auto item = std::add_pointer_t<IShellItem>{};
			state_h = selection->GetItemAt(index, &item);
			assert_test(state_h == S_OK);
			auto name = LPWSTR{};
			state_h = item->GetDisplayName(SIGDN_FILESYSPATH, &name);
			assert_test(state_h == S_OK);
			item->Release();
			result.emplace_back(get_file_long_path(std::wstring{name}));
			CoTaskMemFree(name);
		}
		return result;
	}

	// ----------------

	inline auto get_module_file_path (
		HMODULE const & handle
	) -> std::wstring {
		auto state_d = DWORD{};
		auto buffer = std::array<wchar_t, 1024>{};
		state_d = GetModuleFileNameW(handle, buffer.data(), static_cast<DWORD>(buffer.size()));
		assert_test(state_d != 0 && state_d != buffer.size());
		// TODO : maybe ERROR_INSUFFICIENT_BUFFER even if function succeeded
		return std::wstring{buffer.data(), state_d};
	}

	inline auto get_roaming_app_data_directory_path (
	) -> std::wstring {
		auto state_h = DWORD{};
		auto buffer = LPWSTR{};
		state_h = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &buffer);
		assert_test(state_h == S_OK);
		auto data = std::wstring{buffer};
		CoTaskMemFree(buffer);
		return data;
	}

}
