#pragma once

#include "shell/core_interface.hpp"
#include "shell/version.hpp"
#include <cstring>
#include <optional>
#include <span>
#include <array>
#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <charconv>
#include <codecvt>
#include <locale>
#include <thread>

#if defined M_system_windows
#include "Windows.h"
#include "ShlObj_core.h"
#endif

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

	#pragma region using

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
	) -> std::string {
		return std::string{reinterpret_cast<char const *>(structure.data), from_size_structure(structure.size)};
	}

	inline auto to_string_structure (
		std::string const & value
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
	) -> std::vector<std::string> {
		auto value = std::vector<std::string>{};
		value.reserve(from_size_structure(structure.size));
		for (auto & element : std::span{structure.data, from_size_structure(structure.size)}) {
			value.emplace_back(reinterpret_cast<char const *>(element.data), from_size_structure(element.size));
		}
		return value;
	}

	inline auto allocate_string_list_structure (
		std::vector<std::string> const & value
	) -> Core::StringList {
		auto structure = Core::StringList{
			.data = new Core::String[value.size()]{},
			.size = to_size_structure(value.size()),
			.capacity = to_size_structure(value.size()),
		};
		for (auto index = std::size_t{0}; index < value.size(); ++index) {
			auto & element = value[index];
			structure.data[index] = Core::String{
				.data = new Core::Character[element.size()]{},
				.size = to_size_structure(element.size()),
				.capacity = to_size_structure(element.size()),
			};
			std::memcpy(structure.data[index].data, element.data(), element.size());
		}
		return structure;
	}

	inline auto free_string_list_structure (
		Core::StringList & structure
	) -> void {
		if (structure.data != nullptr) {
			for (auto & element : std::span{structure.data, from_size_structure(structure.capacity)}) {
				delete[] element.data;
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

	#pragma region windows only

	#if defined M_system_windows

	namespace Windows {

		#pragma region open file dialog

		// NOTE : if arch == arm_32 : compile failed : undefined symbol "CoInitialize"...
		inline auto open_file_dialog (
			bool const & pick_folder,
			bool const & multiple
		) -> std::vector<std::string> {
			auto result = std::vector<std::string>{};
			CoInitialize(nullptr);
			auto dialog = X<IFileOpenDialog *>{nullptr};
			auto h_result = CoCreateInstance(__uuidof(FileOpenDialog), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));
			assert_condition(SUCCEEDED(h_result));
			auto option = FILEOPENDIALOGOPTIONS{};
			h_result = dialog->GetOptions(&option);
			assert_condition(SUCCEEDED(h_result));
			option |= FOS_NOCHANGEDIR | FOS_FORCEFILESYSTEM | FOS_NODEREFERENCELINKS | FOS_DONTADDTORECENT | FOS_FORCESHOWHIDDEN;
			if (pick_folder) {
				option |= FOS_PICKFOLDERS;
			}
			if (multiple) {
				option |= FOS_ALLOWMULTISELECT;
			}
			h_result = dialog->SetOptions(option);
			assert_condition(SUCCEEDED(h_result));
			h_result = dialog->Show(nullptr);
			if (h_result != HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
				assert_condition(SUCCEEDED(h_result));
				auto selected_item_list = X<IShellItemArray *>{nullptr};
				h_result = dialog->GetResults(&selected_item_list);
				assert_condition(SUCCEEDED(h_result));
				auto item_count = DWORD{0};
				h_result = selected_item_list->GetCount(&item_count);
				assert_condition(SUCCEEDED(h_result));
				result.reserve(item_count);
				for (auto index = DWORD{0}; index < item_count; index++) {
					auto item = X<IShellItem *>{nullptr};
					auto display_name = LPWSTR{nullptr};
					h_result = selected_item_list->GetItemAt(index, &item);
					assert_condition(SUCCEEDED(h_result));
					h_result = item->GetDisplayName(SIGDN_FILESYSPATH, &display_name);
					assert_condition(SUCCEEDED(h_result));
					auto display_name_8 = utf16_to_utf8(std::u16string_view{reinterpret_cast<char16_t const *>(display_name)});
					result.emplace_back(std::move(reinterpret_cast<std::string &>(display_name_8)));
					CoTaskMemFree(display_name);
					item->Release();
				}
				selected_item_list->Release();
			}
			dialog->Release();
			return result;
		}

		#pragma endregion

	}

	#endif

	#pragma endregion

}
