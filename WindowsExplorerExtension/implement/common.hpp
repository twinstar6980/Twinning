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

#include "implement/version.hpp"
#include <string>
#include <regex>
#include <optional>
#include <vector>
#include <array>
#include <filesystem>
#include <Windows.h>
#include <ShObjIdl_core.h>
#include <shellapi.h>
#include <wrl/module.h>
#include <wrl/implements.h>
#include <wrl/client.h>
#include <wil/resource.h>

#pragma warning(pop)

using Microsoft::WRL::RuntimeClass;
using Microsoft::WRL::RuntimeClassFlags;
using Microsoft::WRL::ClassicCom;
using Microsoft::WRL::ComPtr;
using Microsoft::WRL::Make;
using Microsoft::WRL::Module;
using Microsoft::WRL::InProc;

#define thiz (*this)

namespace TwinStar::WindowsExplorerExtension {

	template <typename T>
	using X = T;

	// ----------------

	inline auto g_dll_handle = HMODULE{};

	// ----------------

	inline auto get_module_file_name (
		HMODULE const & handle
	) -> std::wstring {
		auto state_d = DWORD{};
		auto buffer = std::array<wchar_t, 1024>{};
		state_d = GetModuleFileNameW(handle, buffer.data(), static_cast<DWORD>(buffer.size()));
		if (state_d == 0) {
			throw std::runtime_error{std::format("^ GetModuleFileNameW : {}", GetLastError())};
		}
		// TODO : maybe ERROR_INSUFFICIENT_BUFFER even if function succeeded
		return std::wstring{buffer.data(), state_d};
	}

	// ----------------

	// TODO : error 14007 ?
	inline auto get_environment_variable (
		std::wstring const & name
	) -> std::wstring {
		auto state_d = DWORD{};
		auto buffer = std::vector<wchar_t>{};
		state_d = GetEnvironmentVariableW(name.data(), nullptr, 0);
		if (state_d == 0) {
			throw std::runtime_error{std::format("^ GetEnvironmentVariableW : {}", GetLastError())};
		}
		buffer.reserve(state_d);
		state_d = GetEnvironmentVariableW(name.data(), buffer.data(), static_cast<DWORD>(buffer.size()));
		if (state_d != buffer.size() - 1) {
			throw std::runtime_error{std::format("^ GetEnvironmentVariableW : {}", GetLastError())};
		}
		return std::wstring{buffer.data(), state_d};
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
		auto state_d = DWORD{};
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
		if (!state_b) {
			throw std::runtime_error{std::format("^ CreateProcessW : {}", GetLastError())};
		}
		return;
	}

	// ----------------

	inline auto get_shell_item_file_path (
		IShellItemArray * const & selection
	) -> std::vector<std::wstring> {
		auto state_h = HRESULT{};
		auto result = std::vector<std::wstring>{};
		if (selection == nullptr) {
			throw std::runtime_error{std::format("selection is null")};
		}
		auto count = DWORD{};
		state_h = selection->GetCount(&count);
		if (state_h != S_OK) {
			throw std::runtime_error{std::format("^ IShellItemArray::GetCount : {}", state_h)};
		}
		result.reserve(count);
		for (auto i = DWORD{0}; i < count; ++i) {
			auto item = X<IShellItem *>{};
			state_h = selection->GetItemAt(i, &item);
			if (state_h != S_OK) {
				throw std::runtime_error{std::format("^ IShellItemArray::GetItemAt : {}", state_h)};
			}
			auto name = LPWSTR{};
			state_h = item->GetDisplayName(SIGDN_FILESYSPATH, &name);
			if (state_h != S_OK) {
				throw std::runtime_error{std::format("^ IShellItemArray::GetDisplayName : {}", state_h)};
			}
			item->Release();
			result.emplace_back(name);
			CoTaskMemFree(name);
		}
		return result;
	}

}
