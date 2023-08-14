#pragma once

#include "implement/common.hpp"
#include "implement/language.hpp"
#include "implement/base_explorer_command.hpp"

namespace TwinStar::ForwarderForWindows {

	#pragma region configuration

	struct ForwardExplorerCommandConfiguration {
		std::wstring                id;
		std::optional<bool>         type;
		std::optional<std::wregex>  rule;
		std::optional<std::wstring> method;
		std::wstring                argument;
	};

	struct GroupForwardExplorerCommandConfiguration {
		std::wstring                                     id;
		std::vector<ForwardExplorerCommandConfiguration> item;
		std::vector<std::size_t>                         separator;
	};

	// ----------------

	inline auto test_forward_available (
		ForwardExplorerCommandConfiguration const & configuration,
		std::wstring const &                        input_short,
		std::wstring const &                        input_long
	) -> bool {
		auto result = true;
		if (configuration.type) {
			if (!configuration.type.value()) {
				result &= std::filesystem::is_regular_file(std::filesystem::path{input_short});
			} else {
				result &= std::filesystem::is_directory(std::filesystem::path{input_short});
			}
		}
		if (result && configuration.rule) {
			result &= std::regex_search(input_long, configuration.rule.value());
		}
		return result;
	}

	#pragma endregion

	#pragma region command

	class ForwardExplorerCommand :
		public BaseExplorerCommand {

	protected:

		ForwardExplorerCommandConfiguration const & m_configuration;
		bool                                        m_has_icon;

	public:

		#pragma region structor

		explicit ForwardExplorerCommand (
			ForwardExplorerCommandConfiguration const & configuration,
			bool const &                                has_icon = true
		) :
			BaseExplorerCommand{},
			m_configuration{configuration},
			m_has_icon{has_icon} {
		}

		#pragma endregion

		#pragma region implement

		virtual auto icon (
		) -> LPWSTR override {
			static auto dll_path = get_module_file_name(g_dll_handle);
			return !thiz.m_has_icon ? (nullptr) : (dll_path.data());
		}

		virtual auto title (
		) -> LPWSTR override {
			return const_cast<LPWSTR>(Language::query(std::format(L"forward:{}", thiz.m_configuration.id)).data());
		}

		virtual auto flag (
		) -> EXPCMDFLAGS override {
			return ECF_DEFAULT;
		}

		virtual auto state (
			IShellItemArray * selection
		) -> EXPCMDSTATE override {
			assert_test(selection != nullptr);
			auto hidden_item_when_unavailable = get_register_value_dword(k_register_key_parent, k_register_key_path, L"hidden_item_when_unavailable").value_or(0) != 0;
			auto input_list = get_shell_item_file_path(selection);
			auto available = std::ranges::all_of(
				input_list,
				[&] (auto & input) {
					return test_forward_available(thiz.m_configuration, input, get_file_long_path(input));
				}
			);
			auto state = !available ? (ECS_DISABLED) : (ECS_ENABLED);
			if (hidden_item_when_unavailable && state == ECS_DISABLED) {
				state = ECS_HIDDEN;
			}
			return state;
		}

		virtual auto invoke (
			IShellItemArray * selection
		) -> void override {
			try {
				assert_test(selection != nullptr);
				auto input_list = get_shell_item_file_path(selection);
				auto script = get_register_value_string(k_register_key_parent, k_register_key_path, L"script").value_or(L"");
				auto limit = get_register_value_dword(k_register_key_parent, k_register_key_path, L"limit").value_or(0);
				if (!std::filesystem::is_regular_file(std::filesystem::path{script})) {
					throw std::runtime_error{std::format("invalid script")};
				}
				if (limit != 0 && input_list.size() > limit) {
					throw std::runtime_error{std::format("too many item")};
				}
				auto program = std::wstring{L"C:\\Windows\\System32\\cmd.exe"};
				auto argument = std::vector<std::wstring>{};
				argument.emplace_back(L"/C");
				argument.emplace_back(script);
				auto argument_count_per_command = std::ptrdiff_t{1 + (!thiz.m_configuration.method.has_value() ? (0) : (2)) + 2};
				for (auto & input : input_list) {
					argument.emplace_back(get_file_long_path(input));
					if (thiz.m_configuration.method.has_value()) {
						argument.emplace_back(L"-method");
						argument.emplace_back(thiz.m_configuration.method.value());
					}
					argument.emplace_back(L"-argument");
					argument.emplace_back(thiz.m_configuration.argument);
					if (limit != 0) {
						create_process(program, argument);
						argument.erase(argument.end() - argument_count_per_command, argument.end());
					}
				}
				if (limit == 0) {
					create_process(program, argument);
				}
			} catch (std::exception const & exception) {
				// TODO : suppose encoding is ANSI, right ?
				MessageBoxA(nullptr, exception.what(), "ERROR", MB_OK | MB_ICONERROR);
			}
			return;
		}

		#pragma endregion

	};

	class ForwardEnumExplorerCommand :
		public BaseEnumExplorerCommand {

	public:

		#pragma region structor

		explicit ForwardEnumExplorerCommand (
			GroupForwardExplorerCommandConfiguration const & configuration
		) :
			BaseEnumExplorerCommand{} {
			auto separator_index = std::size_t{0};
			auto current_separator_section_count = std::size_t{0};
			thiz.m_command.reserve(configuration.item.size() + configuration.separator.size());
			for (auto & element : configuration.item) {
				if (separator_index < configuration.separator.size() && current_separator_section_count == configuration.separator[separator_index]) {
					current_separator_section_count = 0;
					thiz.m_command.emplace_back(WRL::Make<SeparatorExplorerCommand>());
					++separator_index;
				}
				thiz.m_command.emplace_back(WRL::Make<ForwardExplorerCommand>(element, false));
				++current_separator_section_count;
			}
			thiz.m_current = 0;
		}

		#pragma endregion

	};

	class GroupForwardExplorerCommand :
		public BaseExplorerCommand {

	protected:

		GroupForwardExplorerCommandConfiguration const & m_configuration;

	public:

		#pragma region structor

		explicit GroupForwardExplorerCommand (
			GroupForwardExplorerCommandConfiguration const & configuration
		) :
			BaseExplorerCommand{},
			m_configuration{configuration} {
		}

		#pragma endregion

		#pragma region implement

		virtual auto icon (
		) -> LPWSTR override {
			static auto dll_path = get_module_file_name(g_dll_handle);
			return dll_path.data();
		}

		virtual auto title (
		) -> LPWSTR override {
			return const_cast<LPWSTR>(Language::query(std::format(L"forward:{}", thiz.m_configuration.id)).data());
		}

		virtual auto sub (
		) -> IEnumExplorerCommand * override {
			auto result = std::add_pointer_t<IEnumExplorerCommand>{};
			auto pointer = WRL::Make<ForwardEnumExplorerCommand>(thiz.m_configuration);
			pointer->QueryInterface(IID_PPV_ARGS(&result));
			return result;
		}

		virtual auto flag (
		) -> EXPCMDFLAGS override {
			return ECF_HASSUBCOMMANDS;
		}

		virtual auto state (
			IShellItemArray * selection
		) -> EXPCMDSTATE override {
			assert_test(selection != nullptr);
			auto hidden_group_when_unavailable = get_register_value_dword(k_register_key_parent, k_register_key_path, L"hidden_group_when_unavailable").value_or(0) != 0;
			auto input_list = get_shell_item_file_path(selection);
			auto available = std::ranges::any_of(
				thiz.m_configuration.item,
				[&] (auto & configuration) {
					return std::ranges::all_of(
						input_list,
						[&] (auto & input) {
							return test_forward_available(configuration, input, get_file_long_path(input));
						}
					);
				}
			);
			auto state = !available ? (ECS_DISABLED) : (ECS_ENABLED);
			if (hidden_group_when_unavailable && state == ECS_DISABLED) {
				state = ECS_HIDDEN;
			}
			return state;
		}

		virtual auto invoke (
			IShellItemArray * selection
		) -> void override {
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
