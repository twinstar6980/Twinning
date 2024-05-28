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
	try {
		auto app_data_directory_raw = LPWSTR{};
		if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &app_data_directory_raw) != S_OK) {
			throw std::runtime_error{"failed to get app data directory"};
		}
		auto app_data_directory = std::filesystem::path{app_data_directory_raw};
		CoTaskMemFree(app_data_directory_raw);
		auto data_directory = app_data_directory / "TwinStar.Twinning.Forwarder";
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
		if (reinterpret_cast<INT_PTR>(ShellExecuteW(nullptr, L"open", data_directory.wstring().data(), nullptr, nullptr, SW_NORMAL)) <= 32) {
			throw std::runtime_error{"failed to reveal data directory"};
		}
		std::exit(0);
	}
	catch (std::exception & e) {
		std::println(std::cout, "{} : {}", typeid(e).name(), e.what());
		std::system("pause");
	}
	catch (...) {
		std::println(std::cout, "?");
		std::system("pause");
	}
	return 0;
}
