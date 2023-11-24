#pragma once

#define M_version 35

#include <cstring>
#include <string_view>
#include <string>
#include <optional>
#include <span>
#include <array>
#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <codecvt>
#include <locale>
#include <iostream>

namespace TwinStar::ShellCLI {

	#pragma region literal

	using namespace std::literals::string_literals;

	using namespace std::literals::string_view_literals;

	#pragma endregion

}
