#pragma once

#include "shell/common.hpp"
#include "shell/utility/string.hpp"
#include "shell/utility/interaction.hpp"
#include "shell/utility/system/windows.hpp"
#include "shell/host/host.hpp"
#include "shell/third/tinyfiledialogs.hpp"

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
		) :
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
				case hash_string("version"sv) : {
					assert_test(argument.size() == 1);
					auto number = thiz.version();
					result.emplace_back(std::to_string(number));
					break;
				}
				case hash_string("host"sv) : {
					assert_test(argument.size() == 1);
					auto name = thiz.host();
					result.emplace_back(std::move(name));
					break;
				}
				case hash_string("system"sv) : {
					assert_test(argument.size() == 1);
					auto name = thiz.system();
					result.emplace_back(std::move(name));
					break;
				}
				case hash_string("architecture"sv) : {
					assert_test(argument.size() == 1);
					auto name = thiz.architecture();
					result.emplace_back(std::move(name));
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
				case hash_string("pick_path"sv) : {
					assert_test(argument.size() == 2);
					auto type = argument[1];
					auto selection = thiz.pick_path(type);
					result.emplace_back(selection.value_or(""s));
					break;
				}
				case hash_string("push_notification"sv) : {
					assert_test(argument.size() == 3);
					auto title = argument[1];
					auto description = argument[2];
					thiz.push_notification(title, description);
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

		auto version (
		) -> std::size_t {
			return std::size_t{M_version};
		}

		auto host (
		) -> std::string {
			return std::string{"cli"};
		}

		auto system (
		) -> std::string {
			return std::string{M_system};
		}

		auto architecture (
		) -> std::string {
			return std::string{M_architecture};
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

		// ----------------

		auto pick_path (
			std::string const & type
		) -> std::optional<std::string> {
			auto selection = std::optional<std::string>{};
			#if defined M_system_windows
			switch (hash_string(type)) {
				case hash_string("file"sv) : {
					auto selection_raw = Windows::pick_path(false, false);
					if (!selection_raw.empty()) {
						selection.emplace(selection_raw[0]);
					}
					break;
				}
				case hash_string("directory"sv) : {
					auto selection_raw = Windows::pick_path(true, false);
					if (!selection_raw.empty()) {
						selection.emplace(selection_raw[0]);
					}
					break;
				}
				default : {
					throw std::runtime_error{"invalid path type"s};
				}
			}
			#endif
			#if defined M_system_linux || defined M_system_macintosh
			switch (hash_string(type)) {
				case hash_string("file"sv) : {
					auto selection_raw = Third::tinyfiledialogs::tinyfd_openFileDialog("", nullptr, 0, nullptr, nullptr, false);
					if (selection_raw != nullptr) {
						selection.emplace(selection_raw);
					}
					break;
				}
				case hash_string("directory"sv) : {
					auto selection_raw = Third::tinyfiledialogs::tinyfd_selectFolderDialog("", nullptr);
					if (selection_raw != nullptr) {
						selection.emplace(selection_raw);
					}
					break;
				}
				default : {
					throw std::runtime_error{"invalid path type"s};
				}
			}
			#endif
			#if defined M_system_android || defined M_system_iphone
			throw std::runtime_error{"unavailable method"s};
			#endif
			return selection;
		}

		auto push_notification (
			std::string const & title,
			std::string const & description
		) -> void {
			#if defined M_system_windows || defined M_system_linux || defined M_system_macintosh
			Third::tinyfiledialogs::tinyfd_notifyPopup(title.data(), description.data(), "info");
			#endif
			#if defined M_system_android || defined M_system_iphone
			throw std::runtime_error{"unavailable method"s};
			#endif
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
