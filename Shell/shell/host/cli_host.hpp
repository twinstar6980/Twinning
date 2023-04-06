#pragma once

#include "shell/common.hpp"
#include "shell/utility/string.hpp"
#include "shell/utility/interaction.hpp"
#include "shell/host/host.hpp"

namespace TwinStar::Shell {

	#pragma region type

	class CLIHost :
		public Host {

	protected:

		bool m_running;

	public:

		#pragma region structor

		virtual ~CLIHost (
		) override {
		}

		// ----------------

		CLIHost (
		) = delete;

		CLIHost (
			CLIHost const & that
		) = delete;

		CLIHost (
			CLIHost && that
		) = delete;

		// ----------------

		explicit CLIHost (
			std::nullptr_t _
		):
			m_running{false} {
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			CLIHost const & that
		) -> CLIHost & = delete;

		auto operator = (
			CLIHost && that
		) -> CLIHost & = delete;

		#pragma endregion

		#pragma region function

		virtual auto start (
		) -> void override {
			assert_test(!thiz.m_running);
			thiz.m_running = true;
			return;
		}

		virtual auto finish (
		) -> void override {
			assert_test(thiz.m_running);
			thiz.m_running = false;
			return;
		}

		// ----------------

		virtual auto execute (
			std::vector<std::string> const & argument
		) -> std::vector<std::string> override {
			assert_test(thiz.m_running);
			auto result = std::vector<std::string>{};
			assert_test(argument.size() >= 1);
			auto method = argument[0];
			switch (hash_string(method)) {
				case hash_string("name"sv) : {
					assert_test(argument.size() == 1);
					auto name = thiz.name();
					result.emplace_back(std::move(name));
					break;
				}
				case hash_string("version"sv) : {
					assert_test(argument.size() == 1);
					auto version = thiz.version();
					result.emplace_back(std::to_string(version));
					break;
				}
				case hash_string("system"sv) : {
					assert_test(argument.size() == 1);
					auto system = thiz.system();
					result.emplace_back(std::move(system));
					break;
				}
				case hash_string("output"sv) : {
					assert_test(argument.size() == 2);
					auto text = argument[1];
					thiz.output(text);
					break;
				}
				case hash_string("input"sv) : {
					assert_test(argument.size() == 1);
					auto text = thiz.input();
					result.emplace_back(std::move(text));
					break;
				}
				default : {
					throw std::runtime_error{"invalid method"s};
				}
			}
			return result;
		}

		#pragma endregion

		#pragma region execute implement

		auto name (
		) -> std::string {
			return "cli"s;
		}

		auto version (
		) -> std::size_t {
			return std::size_t{M_version};
		}

		auto system (
		) -> std::string {
			return std::string{M_system};
		}

		// ----------------

		auto output (
			std::string const & text
		) -> void {
			return Interaction::output(text);
		}

		auto input (
		) -> std::string {
			return Interaction::input();
		}

		#pragma endregion

	};

	#pragma endregion

}
