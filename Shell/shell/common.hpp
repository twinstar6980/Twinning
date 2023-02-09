#pragma once

#include "shell/version.hpp"
#include <cstring>
#include <string>
#include <string_view>
#include <optional>
#include <span>
#include <vector>
#include <array>
#include <unordered_map>
#include <charconv>
#include <codecvt>
#include <functional>
#include <locale>
#include <thread>
#include <iostream>

#if defined M_system_windows
#include "shell/third/system/windows.hpp"
#endif
#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
#include "shell/third/system/posix.hpp"
#endif

#define thiz (*this)

#define assert_test(...)\
	if (!(__VA_ARGS__)) {\
		throw std::runtime_error{"assertion failed : " #__VA_ARGS__};\
	}\
	static_assert(true)

namespace TwinStar::Shell {

	#pragma region using literal

	using namespace std::literals::string_literals;

	using namespace std::literals::string_view_literals;

	#pragma endregion

	#pragma region type alias

	template <typename It>
	using X = It;

	#pragma endregion

	#pragma region string

	inline constexpr auto hash_string (
		std::string_view const & string
	) -> std::uint64_t {
		auto offset = std::uint64_t{14695981039346656037ull};
		auto prime = std::uint64_t{1099511628211ull};
		auto result = offset;
		for (auto & element : string) {
			result *= prime;
			result ^= static_cast<unsigned char>(element);
		}
		return result;
	}

	// ----------------

	inline auto string_to_boolean (
		std::string const & string
	) -> bool {
		auto result = bool{};
		if (string == "false") {
			result = false;
		} else if (string == "true") {
			result = true;
		} else {
			throw std::runtime_error{"invalid string of boolean"};
		}
		return result;
	}

	// ----------------

	inline auto utf8_to_utf16 (
		std::u8string_view const & source
	) -> std::u16string {
		auto converter = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{};
		auto result = converter.from_bytes(
			reinterpret_cast<char const *>(source.data()),
			reinterpret_cast<char const *>(source.data() + source.size())
		);
		assert_test(converter.converted() == source.size());
		return result;
	}

	inline auto utf16_to_utf8 (
		std::u16string_view const & source
	) -> std::u8string {
		auto converter = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{};
		auto result = converter.to_bytes(
			source.data(),
			source.data() + source.size()
		);
		assert_test(converter.converted() == source.size());
		return reinterpret_cast<std::u8string &>(result);
	}

	#pragma endregion

	#pragma region function

	template <auto id, typename Result, typename ... Argument, typename Function>
	inline auto proxy_dynamic_function_in_current_thread (
		Function const & function
	) -> Result(*) (Argument ...) {
		static auto map = std::unordered_map<std::thread::id, Function>{};
		//
		auto thread_id = std::this_thread::get_id();
		auto item_pair = map.try_emplace(thread_id, function);
		assert_test(item_pair.second);
		return [] (Argument ... argument) -> Result {
			auto thread_id = std::this_thread::get_id();
			return map.at(thread_id)(std::forward<Argument>(argument) ...);
		};
	}

	#pragma endregion

	#pragma region interaction

	inline auto output (
		std::string const & text
	) -> void {
		#if defined M_system_windows
		auto state_b = BOOL{};
		auto text_16 = utf8_to_utf16(reinterpret_cast<std::u8string const &>(text));
		auto length = DWORD{};
		state_b = WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), text_16.data(), static_cast<DWORD>(text_16.size()), &length, nullptr);
		assert_test(state_b);
		return;
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		std::cout << text << std::flush;
		return;
		#endif
	}

	inline auto input (
	) -> std::string {
		#if defined M_system_windows
		auto state_b = BOOL{};
		auto text_16 = std::array<char16_t, 0x1000>{};
		auto length = DWORD{};
		state_b = ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), text_16.data(), static_cast<DWORD>(text_16.size()), &length, nullptr);
		assert_test(state_b);
		auto text_8 = utf16_to_utf8(std::u16string_view{text_16.data(), length - 2});
		return std::string{std::move(reinterpret_cast<std::string &>(text_8))};
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto text = std::string{};
		std::getline(std::cin, text);
		return text;
		#endif
	}

	#pragma endregion

}
