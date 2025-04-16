module;
// ReSharper disable All

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <typeinfo>
#include <type_traits>
#include <source_location>
#include <exception>
#include <stdexcept>
#include <system_error>
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <limits>
#include <initializer_list>
#include <optional>
#include <variant>
#include <any>
#include <tuple>
#include <span>
#include <vector>
#include <array>
#include <list>
#include <bitset>
#include <bit>
#include <memory>
#include <cstring>
#include <string>
#include <string_view>
#include <mutex>
#include <thread>
#include <codecvt>
#include <locale>
#include <clocale>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <utility>
#include <algorithm>
#include <functional>
#include <compare>

#pragma clang diagnostic pop

export module twinning.common.std;

export {
	// operator
	namespace std {
		using ::std::operator +;
		using ::std::operator -;
		using ::std::operator ==;
		using ::std::operator <=>;
		using ::std::operator <<;
		using ::std::operator >>;
	}
	// traits
	namespace std {
		using ::std::is_constant_evaluated;
		using ::std::is_same_v;
		using ::std::conditional_t;
		using ::std::tuple_element_t;
		using ::std::is_const_v;
		using ::std::is_reference_v;
		using ::std::is_lvalue_reference_v;
		using ::std::is_rvalue_reference_v;
		using ::std::is_invocable_v;
		using ::std::add_const_t;
		using ::std::remove_const_t;
		using ::std::add_lvalue_reference_t;
		using ::std::add_rvalue_reference_t;
		using ::std::remove_reference_t;
		using ::std::remove_cv_t;
		using ::std::remove_cvref_t;
		using ::std::is_base_of_v;
		using ::std::is_convertible_v;
		using ::std::is_constructible_v;
		using ::std::is_default_constructible_v;
		using ::std::is_copy_constructible_v;
		using ::std::is_move_constructible_v;
		using ::std::is_destructible_v;
		using ::std::is_assignable_v;
		using ::std::is_copy_assignable_v;
		using ::std::is_move_assignable_v;
		using ::std::is_void_v;
		using ::std::is_pointer_v;
		using ::std::is_member_pointer_v;
		using ::std::add_pointer_t;
		using ::std::is_array_v;
		using ::std::is_enum_v;
		using ::std::underlying_type_t;
		using ::std::make_unsigned_t;
	}
	// exception
	namespace std {
		using ::std::source_location;
		using ::std::exception;
		using ::std::exception_ptr;
		using ::std::current_exception;
		using ::std::rethrow_exception;
		using ::std::runtime_error;
		using ::std::system_error;
		using ::std::errc;
	}
	// integer
	namespace std {
		using ::std::int8_t;
		using ::std::int16_t;
		using ::std::int32_t;
		using ::std::int64_t;
		using ::std::uint8_t;
		using ::std::uint16_t;
		using ::std::uint32_t;
		using ::std::uint64_t;
		using ::std::size_t;
		using ::std::ptrdiff_t;
		using ::std::numeric_limits;
	}
	// math
	namespace std {
		using ::std::floor;
		using ::std::round;
		using ::std::ceil;
	}
	// string
	namespace std {
		using ::std::char_traits;
		using ::std::strlen;
		using ::std::string;
		using ::std::string_view;
		using ::std::u8string;
		using ::std::u8string_view;
		using ::std::u16string;
		using ::std::u16string_view;
		using ::std::codecvt_utf8_utf16;
		using ::std::wstring_convert;
		using ::std::sscanf;
	}
	namespace std::inline literals::inline string_literals {
		using ::std::literals::string_literals::operator ""s;
	}
	namespace std::inline literals::inline string_view_literals {
		using ::std::literals::string_view_literals::operator ""sv;
	}
	// container
	namespace std {
		using ::std::initializer_list;
		using ::std::optional;
		using ::std::variant;
		using ::std::holds_alternative;
		using ::std::any;
		using ::std::any_cast;
		using ::std::pair;
		using ::std::make_pair;
		using ::std::tuple;
		using ::std::make_tuple;
		using ::std::span;
		using ::std::vector;
		using ::std::array;
		using ::std::list;
		using ::std::unordered_map;
		using ::std::bitset;
	}
	// memory
	namespace std {
		using ::std::memcpy;
		using ::std::unique_ptr;
		using ::std::make_unique;
		using ::std::shared_ptr;
	}
	// filesystem
	namespace std {
		using ::std::FILE;
		using ::std::fopen;
		using ::std::fclose;
		using ::std::fread;
		using ::std::fwrite;
	}
	namespace std::filesystem {
		using ::std::filesystem::filesystem_error;
		using ::std::filesystem::path;
		using ::std::filesystem::file_type;
		using ::std::filesystem::directory_iterator;
		using ::std::filesystem::copy_options;
		using ::std::filesystem::status;
		using ::std::filesystem::copy;
		using ::std::filesystem::rename;
		using ::std::filesystem::remove_all;
		using ::std::filesystem::create_symlink;
		using ::std::filesystem::create_directory_symlink;
		using ::std::filesystem::read_symlink;
		using ::std::filesystem::create_hard_link;
		using ::std::filesystem::file_size;
		using ::std::filesystem::resize_file;
		using ::std::filesystem::create_directories;
		using ::std::filesystem::current_path;
		using ::std::filesystem::begin;
		using ::std::filesystem::end;
		using ::std::filesystem::operator |;
	}
	// thread
	namespace std {
		using ::std::mutex;
		using ::std::thread;
	}
	namespace std::this_thread {
		using ::std::this_thread::yield;
		using ::std::this_thread::sleep_for;
	}
	// time
	namespace std::chrono {
		using ::std::chrono::milliseconds;
	}
	// io
	namespace std {
		using ::std::cin;
		using ::std::cout;
		using ::std::cerr;
		using ::std::flush;
		using ::std::getline;
	}
	// locale
	namespace std {
		using ::std::setlocale;
	}
	namespace stddef {
		inline constexpr auto $LC_ALL = LC_ALL;
	}
	// miscellaneous
	namespace std {
		using ::std::index_sequence;
		using ::std::make_index_sequence;
		using ::std::endian;
		using ::std::hash;
		using ::std::function;
		using ::std::strong_ordering;
		using ::std::swap;
		using ::std::min;
		using ::std::max;
		using ::std::move;
		using ::std::forward;
		using ::std::replace;
		using ::std::make_reverse_iterator;
		using ::std::make_move_iterator;
		using ::std::get;
		using ::std::system;
	}
	// literal
	using namespace ::std::literals;
}
