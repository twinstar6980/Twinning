#pragma once

#define M_version "45"

#include <type_traits>
#include <exception>
#include <algorithm>
#include <cstddef>
#include <cstdint>
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

namespace Twinning::Shell {

	#pragma region literal

	using namespace std::literals::string_literals;

	using namespace std::literals::string_view_literals;

	#pragma endregion

}
