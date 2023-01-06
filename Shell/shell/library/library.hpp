#pragma once

#include "shell/base.hpp"

namespace TwinStar::Shell {

	#pragma region type

	class Library {

	public:

		#pragma region structor

		virtual ~Library (
		) = default;

		// ----------------

		Library (
		) = default;

		Library (
			Library const & that
		) = delete;

		Library (
			Library && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Library const & that
		) -> Library& = delete;

		auto operator = (
			Library && that
		) -> Library& = delete;

		#pragma endregion

		#pragma region interface

		virtual auto version (
		) -> Core::Size const* = 0;

		virtual auto execute (
			Core::String const &        script,
			Core::Boolean const &       script_is_path,
			Core::StringList const &    argument,
			Core::ShellCallback const & shell_callback
		) -> Core::String const* = 0;

		#pragma endregion

		#pragma region interface wrapper

		auto wrapped_version (
		) -> std::size_t {
			auto result_s = thiz.version();
			return CoreTypeConverter::from_size(*result_s);
		}

		auto wrapped_execute (
			std::string const &              script,
			bool const &                     script_is_path,
			std::vector<std::string> const & argument,
			Core::ShellCallback const &      shell_callback
		) -> std::optional<std::string> {
			auto script_s = CoreTypeConverter::to_string(script);
			auto script_is_path_s = CoreTypeConverter::to_boolean(script_is_path);
			auto argument_s = CoreTypeConverter::allocate_string_list(argument);
			auto result_s = thiz.execute(script_s, script_is_path_s, argument_s, shell_callback);
			CoreTypeConverter::free_string_list(argument_s);
			return result_s ? std::make_optional<std::string>(CoreTypeConverter::from_string(*result_s)) : std::nullopt;
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region common

	struct SymbolTable {
		decltype(Core::version) * version;
		decltype(Core::execute) * execute;
	};

	inline constexpr struct {
		std::string_view version;
		std::string_view execute;
	}
	k_symbol_id = {
		#if defined M_compiler_msvc
		.version = "?version@Interface@Core@TwinStar@@YAPEBUSize@123@XZ"sv,
		.execute = "?execute@Interface@Core@TwinStar@@YAPEBUString@123@AEBU4123@AEBUBoolean@123@AEBUStringList@123@AEBQ6AAEBU6123@2@Z@Z"sv,
		#endif
		#if defined M_compiler_clang
		.version = "_ZN8TwinStar4Core9Interface7versionEv"sv,
		.execute = "_ZN8TwinStar4Core9Interface7executeERKNS1_6StringERKNS1_7BooleanERKNS1_10StringListERKPFSA_SA_E"sv,
		#endif
	};

	#pragma endregion

}
