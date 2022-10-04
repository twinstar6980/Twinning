#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#pragma once

#include <optional>
#include <vector>
#include <string>
#include <array>
#include <regex>
#include <filesystem>
#include <Windows.h>
#include <shellapi.h>
#include <wrl/module.h>
#include <wrl/implements.h>
#include <wrl/client.h>
#include <wil/resource.h>
#include <ShObjIdl_core.h>
#include <shellapi.h>

using Microsoft::WRL::RuntimeClass;
using Microsoft::WRL::RuntimeClassFlags;
using Microsoft::WRL::ClassicCom;
using Microsoft::WRL::ComPtr;
using Microsoft::WRL::Make;
using Microsoft::WRL::Module;
using Microsoft::WRL::InProc;

#define thiz (*this)

namespace TwinKleS::WindowsExplorerExtension {

	template <typename T>
	using X = T;

	// ----------------

	inline auto g_dll_handle = HMODULE{};

	inline auto get_dll_path (
		HMODULE const & handle
	) -> std::wstring {
		auto buffer = std::array<WCHAR, 1024>{};
		auto length = GetModuleFileNameW(g_dll_handle, buffer.data(), 1024);
		// todo : check length == 0 ?
		return std::wstring{buffer.begin(), buffer.begin() + length};
	}

}
