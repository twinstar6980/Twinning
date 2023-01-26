#pragma once

#include "shell/common.hpp"

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
			Core::Callback const &   callback,
			Core::String const &     script,
			Core::StringList const & argument
		) -> Core::String const* = 0;

		#pragma endregion

		#pragma region interface wrapper

		auto wrapped_version (
		) -> std::size_t {
			auto result_s = thiz.version();
			return CoreTypeConverter::from_size(*result_s);
		}

		auto wrapped_execute (
			Core::Callback const &           callback,
			std::string const &              script,
			std::vector<std::string> const & argument
		) -> std::optional<std::string> {
			auto script_s = CoreTypeConverter::to_string(script);
			auto argument_s = CoreTypeConverter::allocate_string_list(argument);
			auto result_s = thiz.execute(callback, script_s, argument_s);
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
		.execute = "?execute@Interface@Core@TwinStar@@YAPEBUString@123@AEBQ6AAEBUStringList@123@AEBU5123@@ZAEBU4123@0@Z"sv,
		#endif
		#if defined M_compiler_clang
		.version = "_ZN8TwinStar4Core9Interface7versionEv"sv,
		.execute = "_ZN8TwinStar4Core9Interface7executeERKPFRKNS1_10StringListES4_ERKNS1_6StringES4_"sv,
		#endif
	};

	#pragma endregion

}
