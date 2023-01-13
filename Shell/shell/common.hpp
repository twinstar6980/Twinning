#pragma once

#include "shell/core_interface.hpp"
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
#include <locale>
#include <thread>
#include <iostream>

#if defined M_system_windows
#include "shell/third_party/system/windows.hpp"
#endif
#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
#include "shell/third_party/system/posix.hpp"
#endif

#define thiz (*this)

#define assert_condition(...)\
	if (!(__VA_ARGS__)) {\
		throw std::runtime_error{"assert failed : " #__VA_ARGS__};\
	}\
	void()

namespace TwinStar::Shell {

	#pragma region namespace alias

	namespace Core = Core::Interface;

	#pragma endregion

	#pragma region using literal

	using namespace std::literals::string_literals;

	using namespace std::literals::string_view_literals;

	#pragma endregion

	#pragma region type alias

	template <typename It>
	using X = It;

	#pragma endregion

	#pragma region string utility

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

	inline auto string_to_integer (
		std::string const & string
	) -> std::int64_t {
		auto result = std::int64_t{};
		auto parse_result = std::from_chars(string.data(), string.data() + string.size(), result);
		assert_condition(parse_result.ec == std::errc{} && parse_result.ptr == string.data() + string.size());
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
		assert_condition(converter.converted() == source.size());
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
		assert_condition(converter.converted() == source.size());
		return reinterpret_cast<std::u8string &>(result);
	}

	#pragma endregion

	#pragma region Core type converter utility

	namespace CoreTypeConverter {

		inline auto from_size (
			Core::Size const & structure
		) -> std::size_t {
			return structure.value;
		}

		inline auto to_size (
			std::size_t const & value
		) -> Core::Size {
			return Core::Size{
				.value = value,
			};
		}

		// ----------------

		inline auto from_boolean (
			Core::Boolean const & structure
		) -> bool {
			return structure.value;
		}

		inline auto to_boolean (
			bool const & value
		) -> Core::Boolean {
			return Core::Boolean{
				.value = value,
			};
		}

		// ----------------

		inline auto from_string (
			Core::String const & structure
		) -> std::string {
			return std::string{reinterpret_cast<char const *>(structure.data), from_size(structure.size)};
		}

		inline auto to_string (
			std::string const & value
		) -> Core::String {
			return Core::String{
				.data = const_cast<Core::Character *>(reinterpret_cast<Core::Character const *>(value.data())),
				.size = to_size(value.size()),
				.capacity = to_size(value.size()),
			};
		}

		// ----------------

		inline auto from_string_list (
			Core::StringList const & structure
		) -> std::vector<std::string> {
			auto value = std::vector<std::string>{};
			value.reserve(from_size(structure.size));
			for (auto & element : std::span{structure.data, from_size(structure.size)}) {
				value.emplace_back(reinterpret_cast<char const *>(element.data), from_size(element.size));
			}
			return value;
		}

		inline auto allocate_string_list (
			std::vector<std::string> const & value
		) -> Core::StringList {
			auto structure = Core::StringList{
				.data = new Core::String[value.size()]{},
				.size = to_size(value.size()),
				.capacity = to_size(value.size()),
			};
			for (auto index = std::size_t{0}; index < value.size(); ++index) {
				auto & element = value[index];
				structure.data[index] = Core::String{
					.data = new Core::Character[element.size()]{},
					.size = to_size(element.size()),
					.capacity = to_size(element.size()),
				};
				std::memcpy(structure.data[index].data, element.data(), element.size());
			}
			return structure;
		}

		inline auto free_string_list (
			Core::StringList & structure
		) -> void {
			if (structure.data != nullptr) {
				for (auto & element : std::span{structure.data, from_size(structure.capacity)}) {
					delete[] element.data;
				}
				delete[] structure.data;
				structure.data = nullptr;
			}
			structure.size = to_size(0);
			structure.capacity = to_size(0);
			return;
		}

	}

	#pragma endregion

	#pragma region core type wrapper

	class CoreTypeStringListHandler {

	protected:

		Core::StringList m_value;

	public:

		#pragma region structor

		~CoreTypeStringListHandler (
		) {
			CoreTypeConverter::free_string_list(thiz.m_value);
		}

		// ----------------

		CoreTypeStringListHandler (
		) = default;

		CoreTypeStringListHandler (
			CoreTypeStringListHandler const & that
		) = delete;

		CoreTypeStringListHandler (
			CoreTypeStringListHandler && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator = (
			CoreTypeStringListHandler const & that
		) -> CoreTypeStringListHandler& = delete;

		auto operator = (
			CoreTypeStringListHandler && that
		) -> CoreTypeStringListHandler& = delete;

		#pragma endregion

		#pragma region value

		auto value (
		) const -> Core::StringList const& {
			return thiz.m_value;
		}

		auto imbue (
			Core::StringList const & value
		) -> void {
			CoreTypeConverter::free_string_list(thiz.m_value);
			thiz.m_value = value;
			return;
		}

		#pragma endregion

	};

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
		assert_condition(state_b);
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
		assert_condition(state_b);
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
