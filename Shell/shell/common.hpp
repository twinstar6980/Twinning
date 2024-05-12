#pragma once

#define M_version "36"

#include <cstring>
#include <string_view>
#include <string>
#include <optional>
#include <any>
#include <span>
#include <array>
#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <codecvt>
#include <locale>
#include <iostream>

namespace TwinStar::Shell {

	#pragma region literal

	using namespace std::literals::string_literals;

	using namespace std::literals::string_view_literals;

	#pragma endregion

}
