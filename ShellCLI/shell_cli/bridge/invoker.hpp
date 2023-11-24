#pragma once

#include "shell_cli/common.hpp"
#include "shell_cli/utility/function.hpp"
#include "shell_cli/bridge/interface.hpp"
#include "shell_cli/bridge/converter.hpp"
#include "shell_cli/bridge/library.hpp"

namespace TwinStar::ShellCLI::Bridge {

	#pragma region type

	struct Invoker {

		#pragma region function

		static auto execute (
			Library &                                                                           library,
			std::function<std::vector<std::string>  (std::vector<std::string> const &)> const & callback,
			std::string const &                                                                 script,
			std::vector<std::string> const &                                                    argument
		) -> std::string {
			auto callback_result_handler = std::add_pointer_t<Interface::StringList>{};
			auto callback_exception_handler = std::add_pointer_t<Interface::String>{};
			auto callback_proxy = proxy_dynamic_function_in_current_thread<&Invoker::execute, Interface::String *, Interface::StringList * *, Interface::StringList * *>(
				[&callback, &callback_result_handler, &callback_exception_handler] (
				Interface::StringList * * argument,
				Interface::StringList * * result
			) -> Interface::String * {
					Converter::destruct_string_list(*callback_result_handler);
					Converter::destruct_string(*callback_exception_handler);
					*result = callback_result_handler;
					try {
						auto argument_value = Converter::parse_string_list(**argument);
						auto result_value = callback(argument_value);
						Converter::construct_string_list(*callback_result_handler, result_value);
						return nullptr;
					}
					catch (...) {
						auto exception_value = parse_current_exception();
						Converter::construct_string(*callback_exception_handler, exception_value);
						return callback_exception_handler;
					}
				}
			);
			{
				callback_result_handler = new Interface::StringList{};
				callback_exception_handler = new Interface::String{};
				Converter::construct_string_list(*callback_result_handler, {});
				Converter::construct_string(*callback_exception_handler, {});
			}
			auto callback_pointer = std::add_pointer_t<Interface::Callback>{};
			auto script_pointer = std::add_pointer_t<Interface::String>{};
			auto argument_pointer = std::add_pointer_t<Interface::StringList>{};
			auto result_pointer = std::add_pointer_t<Interface::String>{};
			{
				callback_pointer = new Interface::Callback{};
				script_pointer = new Interface::String{};
				argument_pointer = new Interface::StringList{};
				Converter::construct_callback(*callback_pointer, callback_proxy);
				Converter::construct_string(*script_pointer, script);
				Converter::construct_string_list(*argument_pointer, argument);
			}
			auto exception_pointer = library.execute(&callback_pointer, &script_pointer, &argument_pointer, &result_pointer);
			{
				Converter::destruct_callback(*callback_pointer);
				Converter::destruct_string(*script_pointer);
				Converter::destruct_string_list(*argument_pointer);
				delete callback_pointer;
				delete script_pointer;
				delete argument_pointer;
			}
			auto result = Converter::parse_string(*result_pointer);
			{
				Converter::destruct_string_list(*callback_result_handler);
				Converter::destruct_string(*callback_exception_handler);
				delete callback_result_handler;
				delete callback_exception_handler;
			}
			if (exception_pointer != nullptr) {
				auto exception = Converter::parse_string(*exception_pointer);
				throw exception;
			}
			return result;
		}

		#pragma endregion

	};

	#pragma endregion

}
