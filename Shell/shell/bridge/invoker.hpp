#pragma once

#include "shell/common.hpp"
#include "shell/utility/function.hpp"
#include "shell/bridge/interface.hpp"
#include "shell/bridge/converter.hpp"
#include "shell/bridge/library.hpp"

namespace TwinStar::Shell::Bridge {

	#pragma region type

	struct Invoker {

		#pragma region function

		static auto version (
			Library & library
		) -> std::size_t {
			auto number_pointer = std::add_pointer_t<Interface::Size>{};
			{
			}
			auto exception_pointer = library.version(&number_pointer);
			{
			}
			auto number = Converter::parse_size(*number_pointer);
			if (exception_pointer != nullptr) {
				auto exception = Converter::parse_string(*exception_pointer);
				throw exception;
			}
			return number;
		}

		static auto execute (
			Library &                                                                           library,
			std::function<std::vector<std::string>  (std::vector<std::string> const &)> const & callback,
			std::string const &                                                                 script,
			std::vector<std::string> const &                                                    argument
		) -> std::string {
			auto callback_exception_handler = Interface::String{};
			auto callback_result_handler = Interface::StringList{};
			auto callback_proxy = proxy_dynamic_function_in_current_thread<&Invoker::execute, Interface::String *, Interface::StringList * *, Interface::StringList * *>(
				[&callback, &callback_exception_handler, &callback_result_handler] (
				Interface::StringList * * argument,
				Interface::StringList * * result
			) -> Interface::String * {
					Converter::destruct_string(callback_exception_handler);
					Converter::destruct_string_list(callback_result_handler);
					*result = &callback_result_handler;
					try {
						auto argument_value = Converter::parse_string_list(**argument);
						auto result_value = callback(argument_value);
						Converter::construct_string_list(callback_result_handler, result_value);
						return nullptr;
					} catch (...) {
						auto exception_value = parse_current_exception();
						Converter::construct_string(callback_exception_handler, exception_value);
						return &callback_exception_handler;
					}
				}
			);
			{
				Converter::construct_string(callback_exception_handler, {});
				Converter::construct_string_list(callback_result_handler, {});
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
				Converter::destruct_string(callback_exception_handler);
				Converter::destruct_string_list(callback_result_handler);
			}
			if (exception_pointer != nullptr) {
				auto exception = Converter::parse_string(*exception_pointer);
				throw exception;
			}
			return result;
		}

		static auto prepare (
			Library & library
		) -> void {
			{
			}
			auto exception_pointer = library.prepare();
			{
			}
			if (exception_pointer != nullptr) {
				auto exception = Converter::parse_string(*exception_pointer);
				throw exception;
			}
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
