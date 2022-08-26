#pragma once

#include "core/macro.hpp"
#include "core/third_party/fmt.hpp"

#if defined __cpp_lib_source_location
#include <source_location>
using std_source_location = std::source_location;
#else
using std_source_location = int;
#endif

namespace TwinKleS::Core {

	#pragma region type

	class SourceLocation {

	protected: //

		std::string_view m_file{};
		std::size_t      m_line{};
		std::string_view m_function{};

	public: //

		#pragma region structor

		constexpr ~SourceLocation (
		) = default;

		// ----------------

		constexpr SourceLocation (
		) = default;

		constexpr SourceLocation (
			SourceLocation const & that
		) = default;

		constexpr SourceLocation (
			SourceLocation && that
		) = default;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			SourceLocation const & that
		) -> SourceLocation& = default;

		constexpr auto operator = (
			SourceLocation && that
		) -> SourceLocation& = default;

		#pragma endregion

		#pragma region string convert

		auto to_string (
		) const -> std::string {
			return fmt::format("@ {}:{} {}", thiz.m_file, thiz.m_line, thiz.m_function);
		}

		#pragma endregion

	public: //

		#pragma region make

		static constexpr auto make (
			std_source_location const & location
		) -> SourceLocation {
			auto result = SourceLocation{};
			#if defined __cpp_lib_source_location
			result.m_file = location.file_name() + (std::string_view{std_source_location::current().file_name()}.size() - std::string_view{"/utility/exception/source_location.hpp"}.size());
			result.m_line = location.line();
			result.m_function = location.function_name();
			#endif
			return result;
		}

		#pragma endregion

	};

	#pragma endregion

}

#if defined __cpp_lib_source_location
#define M_current_source_location std_source_location::current()
#define M_current_source_location_x std_source_location::current()
#if defined M_compiler_clang
// TODO : clang bug : https://github.com/llvm/llvm-project/issues/56379
// TODO : clang bug : https://github.com/llvm/llvm-project/issues/48230 (fixed in new version)
#undef M_current_source_location_x
#define M_current_source_location_x std_source_location{}
#endif
#else
#define M_current_source_location std_source_location{}
#define M_current_source_location_x std_source_location{}
#endif
