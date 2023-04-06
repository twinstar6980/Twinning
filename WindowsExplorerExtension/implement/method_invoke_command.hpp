#pragma once

#include "implement/common.hpp"
#include "implement/language.hpp"
#include "implement/base_command.hpp"

namespace TwinStar::WindowsExplorerExtension {

	#pragma region config

	struct MethodInvokeCommandConfiguration {
		std::wstring                id;
		std::optional<bool>         type;
		std::optional<std::wregex>  rule;
		std::optional<std::wstring> method;
		std::wstring                argument;
	};

	struct GroupMethodInvokeCommandConfiguration {
		std::wstring                                  id;
		std::vector<MethodInvokeCommandConfiguration> item;
		std::vector<std::size_t>                      separator;
	};

	// ----------------

	inline auto test_method_available (
		MethodInvokeCommandConfiguration const & configuration,
		std::wstring const &                     input
	) -> bool {
		auto result = true;
		if (configuration.type) {
			if (configuration.type.value()) {
				result &= std::filesystem::is_directory(std::filesystem::path{input});
			} else {
				result &= std::filesystem::is_regular_file(std::filesystem::path{input});
			}
		}
		if (result && configuration.rule) {
			result &= std::regex_search(input, configuration.rule.value());
		}
		return result;
	}

	#pragma endregion

	#pragma region command

	class MethodInvokeCommand :
		public BaseCommand {

	protected:

		MethodInvokeCommandConfiguration const & m_configuration;
		bool                                     m_has_icon;

	public:

		#pragma region structor

		explicit MethodInvokeCommand (
			MethodInvokeCommandConfiguration const & configuration,
			bool const &                             has_icon = true
		):
			m_configuration{configuration},
			m_has_icon{has_icon} {
		}

		#pragma endregion

		#pragma region implement

		virtual auto title (
		) -> LPCWSTR override {
			return Language::query(std::format(L"method:{}", thiz.m_configuration.id)).data();
		}

		virtual auto icon (
		) -> LPCWSTR override {
			static auto dll_path = get_module_file_name(g_dll_handle);
			return !thiz.m_has_icon ? (nullptr) : (dll_path.data());
		}

		virtual auto state (
			_In_opt_ IShellItemArray * selection
		) -> EXPCMDSTATE override {
			assert_test(selection != nullptr);
			auto hidden_item_when_unavailable = get_register_value_dword(k_register_key_parent, k_register_key_path, L"hidden_item_when_unavailable").value_or(0) != 0;
			auto input_list = get_shell_item_file_path(selection);
			auto available = std::ranges::all_of(
				input_list,
				[&] (auto & input) {
					return test_method_available(thiz.m_configuration, input);
				}
			);
			auto state = !available ? (ECS_DISABLED) : (ECS_ENABLED);
			if (hidden_item_when_unavailable && state == ECS_DISABLED) {
				state = ECS_HIDDEN;
			}
			return state;
		}

		virtual auto invoke (
			_In_opt_ IShellItemArray * selection
		) -> void override {
			try {
				assert_test(selection != nullptr);
				auto input_list = get_shell_item_file_path(selection);
				auto launch_script = get_register_value_string(k_register_key_parent, k_register_key_path, L"launch_script").value_or(L"");
				auto launch_limit = get_register_value_dword(k_register_key_parent, k_register_key_path, L"launch_limit").value_or(0);
				if (!std::filesystem::is_regular_file(std::filesystem::path{launch_script})) {
					throw std::runtime_error{std::format("invalid launch script")};
				}
				if (launch_limit != 0 && input_list.size() > launch_limit) {
					throw std::runtime_error{std::format("too many item")};
				}
				auto program = std::wstring{L"C:\\Windows\\System32\\cmd.exe"};
				auto argument = std::vector<std::wstring>{};
				argument.emplace_back(L"/C");
				argument.emplace_back(launch_script);
				auto argument_count_per_command = std::ptrdiff_t{1 + (!thiz.m_configuration.method.has_value() ? (0) : (2)) + 2};
				for (auto & input : input_list) {
					argument.emplace_back(input);
					if (thiz.m_configuration.method.has_value()) {
						argument.emplace_back(L"-method");
						argument.emplace_back(thiz.m_configuration.method.value());
					}
					argument.emplace_back(L"-argument");
					argument.emplace_back(thiz.m_configuration.argument);
					if (launch_limit != 0) {
						create_process(program, argument);
						argument.erase(argument.end() - argument_count_per_command, argument.end());
					}
				}
				if (launch_limit == 0) {
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

	class MethodInvokeCommandEnum :
		public RuntimeClass<RuntimeClassFlags<ClassicCom>, IEnumExplorerCommand> {

	protected:

		std::vector<ComPtr<IExplorerCommand>>                 m_commands;
		std::vector<ComPtr<IExplorerCommand>>::const_iterator m_current;

	public:

		#pragma region structor

		explicit MethodInvokeCommandEnum (
			GroupMethodInvokeCommandConfiguration const & config
		) {
			auto separator_index = std::size_t{0};
			auto current_separator_section_count = std::size_t{0};
			thiz.m_commands.reserve(config.item.size() + config.separator.size());
			for (auto & element : config.item) {
				if (separator_index < config.separator.size() && current_separator_section_count == config.separator[separator_index]) {
					current_separator_section_count = 0;
					thiz.m_commands.emplace_back(Make<SeparatorCommand>());
					++separator_index;
				}
				thiz.m_commands.emplace_back(Make<MethodInvokeCommand>(element, false));
				++current_separator_section_count;
			}
			thiz.m_current = thiz.m_commands.cbegin();
		}

		#pragma endregion

		#pragma region implement

		virtual IFACEMETHODIMP Next (
			ULONG                                                      celt,
			__out_ecount_part(celt, *pceltFetched) IExplorerCommand ** pUICommand,
			__out_opt ULONG *                                          pceltFetched
		) override {
			auto fetched = ULONG{0};
			wil::assign_to_opt_param(pceltFetched, 0ul);
			for (auto i = ULONG{0}; i < celt && thiz.m_current != thiz.m_commands.cend(); i++) {
				thiz.m_current->CopyTo(&pUICommand[0]);
				++thiz.m_current;
				++fetched;
			}
			wil::assign_to_opt_param(pceltFetched, fetched);
			return fetched == celt ? (S_OK) : (S_FALSE);
		}

		virtual IFACEMETHODIMP Skip (
			ULONG celt
		) override {
			return E_NOTIMPL;
		}

		virtual IFACEMETHODIMP Reset (
		) override {
			thiz.m_current = thiz.m_commands.cbegin();
			return S_OK;
		}

		virtual IFACEMETHODIMP Clone (
			__deref_out IEnumExplorerCommand ** ppenum
		) override {
			*ppenum = nullptr;
			return E_NOTIMPL;
		}

		#pragma endregion

	};

	class GroupMethodInvokeCommand :
		public BaseCommand {

	protected:

		GroupMethodInvokeCommandConfiguration const & m_configuration;

	public:

		#pragma region structor

		explicit GroupMethodInvokeCommand (
			GroupMethodInvokeCommandConfiguration const & configuration
		):
			m_configuration{configuration} {
		}

		#pragma endregion

		#pragma region implement

		virtual IFACEMETHODIMP EnumSubCommands (
			_COM_Outptr_ IEnumExplorerCommand ** ppEnum
		) override {
			*ppEnum = nullptr;
			auto e = Make<MethodInvokeCommandEnum>(thiz.m_configuration);
			return e->QueryInterface(IID_PPV_ARGS(ppEnum));
		}

		// ----------------

		virtual auto title (
		) -> LPCWSTR override {
			return Language::query(std::format(L"method:{}", thiz.m_configuration.id)).data();
		}

		virtual auto icon (
		) -> LPCWSTR override {
			static auto dll_path = get_module_file_name(g_dll_handle);
			return dll_path.data();
		}

		virtual auto state (
			_In_opt_ IShellItemArray * selection
		) -> EXPCMDSTATE override {
			assert_test(selection != nullptr);
			auto hidden_group_when_unavailable = get_register_value_dword(k_register_key_parent, k_register_key_path, L"hidden_group_when_unavailable").value_or(0) != 0;
			auto input_list = get_shell_item_file_path(selection);
			auto available = std::ranges::any_of(
				thiz.m_configuration.item,
				[&] (auto & config) {
					return std::ranges::all_of(
						input_list,
						[&] (auto & input) {
							return test_method_available(config, input);
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

		virtual auto flags (
		) -> EXPCMDFLAGS override {
			return ECF_HASSUBCOMMANDS;
		}

		virtual auto invoke (
			_In_opt_ IShellItemArray * selection
		) -> void override {
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
