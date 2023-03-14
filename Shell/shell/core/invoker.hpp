#pragma once

#include "shell/common.hpp"
#include "shell/core/interface.hpp"
#include "shell/core/converter.hpp"
#include "shell/core/library.hpp"

namespace TwinStar::Shell::Core {

	#pragma region type

	struct Invoker {

		#pragma region function

		static auto version (
			Library & library
		) -> std::size_t {
			auto result_structure = library.version();
			return Converter::parse_size(*result_structure);
		}

		static auto execute (
			Library &                                                                           library,
			std::function<std::vector<std::string>  (std::vector<std::string> const &)> const & callback,
			std::string const &                                                                 script,
			std::vector<std::string> const &                                                    argument
		) -> std::optional<std::string> {
			auto callback_result = Interface::StringList{};
			auto callback_proxy = proxy_dynamic_function_in_current_thread<&Invoker::execute, Interface::StringList const *, Interface::StringList const *>(
				[&callback, &callback_result] (
				Interface::StringList const * argument
			) -> Interface::StringList const* {
					auto result_value = std::vector<std::string>{};
					try {
						auto result_content = callback(Converter::parse_string_list(*argument));
						result_value.clear();
						result_value.emplace_back(""s);
						result_value.insert(result_value.end(), result_content.begin(), result_content.end());
					} catch (std::exception & exception) {
						result_value.clear();
						result_value.emplace_back(""s + typeid(exception).name() + " : " + exception.what());
					} catch (...) {
						result_value.clear();
						result_value.emplace_back("unknown exception"s);
					}
					Converter::destruct_string_list(callback_result);
					Converter::construct_string_list(callback_result, result_value);
					return &callback_result;
				}
			);
			Converter::construct_string_list(callback_result, {});
			auto callback_structure = Interface::Callback{};
			auto script_structure = Interface::String{};
			auto argument_structure = Interface::StringList{};
			Converter::construct_callback(callback_structure, callback_proxy);
			Converter::construct_string(script_structure, script);
			Converter::construct_string_list(argument_structure, argument);
			auto result_structure = library.execute(&callback_structure, &script_structure, &argument_structure);
			Converter::destruct_callback(callback_structure);
			Converter::destruct_string(script_structure);
			Converter::destruct_string_list(argument_structure);
			Converter::destruct_string_list(callback_result);
			return !result_structure ? (std::nullopt) : (std::make_optional<std::string>(Converter::parse_string(*result_structure)));
		}

		static auto prepare (
			Library & library
		) -> std::optional<std::string> {
			auto result_structure = library.prepare();
			return !result_structure ? (std::nullopt) : (std::make_optional<std::string>(Converter::parse_string(*result_structure)));
		}

		#pragma endregion

	};

	#pragma endregion

}
