//

#include <string>
#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <ShlObj_core.h>

auto wmain (
	int       argc,
	wchar_t * argv[]
) -> int {
	auto error = false;
	try {
		auto app_data_directory_data = LPWSTR{};
		if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &app_data_directory_data) != S_OK) {
			throw std::runtime_error{""};
		}
		auto app_data_directory = std::filesystem::path{app_data_directory_data};
		CoTaskMemFree(app_data_directory_data);
		auto data_directory = app_data_directory / "TwinStar.ToolKit.ForwarderForWindows";
		auto script_file = data_directory / "forward.cmd";
		if (std::filesystem::exists(data_directory)) {
			if (!std::filesystem::is_directory(data_directory)) {
				std::filesystem::remove_all(data_directory);
			}
		}
		if (!std::filesystem::exists(data_directory)) {
			std::filesystem::create_directories(data_directory);
		}
		if (std::filesystem::exists(script_file)) {
			if (!std::filesystem::is_regular_file(script_file)) {
				std::filesystem::remove_all(script_file);
			}
		}
		if (!std::filesystem::exists(script_file)) {
			auto file = _wfopen(script_file.wstring().data(), L"wb");
			std::fclose(file);
		}
		ShellExecuteW(nullptr, L"open", data_directory.wstring().data(), nullptr, nullptr, SW_NORMAL);
	} catch (...) {
		error = true;
	}
	std::println(std::cout, "{}", !error ? "succeeded" : "failed");
	std::system("pause");
	return 0;
}
