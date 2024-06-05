//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <ShlObj_core.h>

// ----------------

auto wmain (
	int       argc,
	wchar_t * argv[]
) -> int {
	try {
		auto roaming_directory = LPWSTR{};
		if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &roaming_directory) != S_OK) {
			throw std::runtime_error{"failed to get roaming directory"};
		}
		auto data_directory = std::wstring{roaming_directory} + L"\\TwinStar.Twinning.Forwarder";
		CoTaskMemFree(roaming_directory);
		auto script_file = data_directory + L"\\script.cmd";
		if (std::filesystem::is_regular_file(script_file)) {
			std::wcout << L"current mode is script" << std::endl;
		}
		else {
			std::wcout << L"current mode is link" << std::endl;
		}
		std::wcout << L"please select new mode" << std::endl;
		std::wcout << L"    1. link" << std::endl;
		std::wcout << L"    2. script" << std::endl;
		auto new_mode = std::wstring{};
		std::getline(std::wcin, new_mode);
		if (new_mode != L"1" && new_mode != L"2") {
			std::wcout << "current mode was unchanged" << std::endl;
		}
		else {
			if (std::filesystem::exists(data_directory)) {
				std::filesystem::remove_all(data_directory);
			}
			{
				std::filesystem::create_directories(data_directory);
			}
			if (new_mode == L"2") {
				std::fstream{script_file, std::ios_base::binary | std::ios_base::out}.close();
				if (reinterpret_cast<INT_PTR>(ShellExecuteW(nullptr, L"edit", script_file.data(), nullptr, nullptr, SW_SHOW)) <= 32) {
					throw std::runtime_error{"failed to reveal file"};
				}
			}
			std::wcout << "current mode was changed" << std::endl;
		}
	}
	catch (std::exception & e) {
		std::cout << std::format("{} : {}", typeid(e).name(), e.what()) << std::endl;
	}
	catch (...) {
		std::cout << "?" << std::endl;
	}
	std::system("pause");
	return 0;
}
