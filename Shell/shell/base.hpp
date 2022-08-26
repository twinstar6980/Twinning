#pragma once

#include "./interface.hpp"
#include <span>
#include <vector>
#include <string>
#include <string_view>
#include <codecvt>
#include <locale>
#include <cstring>

#define thiz (*this)

#define assert_condition(...)\
	if (!(__VA_ARGS__)) {\
		throw std::runtime_error{"assert failed : " #__VA_ARGS__};\
	}\
	void()

namespace TwinKleS::Shell {

	#pragma region namespace alias

	namespace Core = Core::Interface;

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

	#pragma region Core interface type utility

	inline auto from_size_structure (
		Core::Size const & structure
	) -> std::size_t {
		return structure.value;
	}

	inline auto to_size_structure (
		std::size_t const & value
	) -> Core::Size {
		return Core::Size{
			.value = value,
		};
	}

	// ----------------

	inline auto from_boolean_structure (
		Core::Boolean const & structure
	) -> bool {
		return structure.value;
	}

	inline auto to_boolean_structure (
		bool const & value
	) -> Core::Boolean {
		return Core::Boolean{
			.value = value,
		};
	}

	// ----------------

	inline auto from_string_structure (
		Core::String const & structure
	) -> std::string_view {
		return std::string_view{reinterpret_cast<char const *>(structure.data), from_size_structure(structure.size)};
	}

	inline auto to_string_structure (
		std::string_view const & value
	) -> Core::String {
		return Core::String{
			.data = const_cast<Core::Character *>(reinterpret_cast<Core::Character const *>(value.data())),
			.size = to_size_structure(value.size()),
			.capacity = to_size_structure(value.size()),
		};
	}

	// ----------------

	inline auto from_string_list_structure (
		Core::StringList const & structure
	) -> std::vector<std::string_view> {
		auto value = std::vector<std::string_view>{};
		value.reserve(from_size_structure(structure.size));
		for (auto & e : std::span{structure.data, from_size_structure(structure.size)}) {
			value.emplace_back(reinterpret_cast<char const *>(e.data), from_size_structure(e.size));
		}
		return value;
	}

	inline auto alloc_string_list_structure (
		std::vector<std::string> const & value
	) -> Core::StringList {
		auto structure = Core::StringList{
			.data = new Core::String[value.size()]{},
			.size = to_size_structure(value.size()),
			.capacity = to_size_structure(value.size()),
		};
		for (auto i = std::size_t{0}; i < value.size(); ++i) {
			auto & e = value[i];
			structure.data[i] = Core::String{
				.data = new Core::Character[e.size()]{},
				.size = to_size_structure(e.size()),
				.capacity = to_size_structure(e.size()),
			};
			std::memcpy(structure.data[i].data, e.data(), e.size());
		}
		return structure;
	}

	inline auto free_string_list_structure (
		Core::StringList & structure
	) -> void {
		if (structure.data != nullptr) {
			for (auto & item : std::span{structure.data, from_size_structure(structure.capacity)}) {
				delete[] item.data;
			}
			delete[] structure.data;
			structure.data = nullptr;
		}
		structure.size = to_size_structure(0);
		structure.capacity = to_size_structure(0);
		return;
	}

	#pragma endregion

	#pragma region core interface type wrapper

	class CTypeStringListHandler {

	protected: //

		Core::StringList m_value;

	public: //

		#pragma region structor

		~CTypeStringListHandler (
		) {
			free_string_list_structure(thiz.m_value);
		}

		// ----------------

		CTypeStringListHandler (
		) = default;

		CTypeStringListHandler (
			CTypeStringListHandler const & that
		) = delete;

		CTypeStringListHandler (
			CTypeStringListHandler && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator = (
			CTypeStringListHandler const & that
		) -> CTypeStringListHandler& = delete;

		auto operator = (
			CTypeStringListHandler && that
		) -> CTypeStringListHandler& = delete;

		#pragma endregion

		#pragma region value

		auto value (
		) const -> Core::StringList const& {
			return thiz.m_value;
		}

		auto imbue (
			Core::StringList const & value
		) -> void {
			free_string_list_structure(thiz.m_value);
			thiz.m_value = value;
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
