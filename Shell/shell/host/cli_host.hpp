#pragma once

#include "shell/common.hpp"
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
		) -> CLIHost& = delete;

		auto operator = (
			CLIHost && that
		) -> CLIHost& = delete;

		#pragma endregion

		#pragma region life-time

		virtual auto start (
		) -> void override {
			assert_condition(!thiz.m_running);
			thiz.m_running = true;
			return;
		}

		virtual auto finish (
		) -> void override {
			assert_condition(thiz.m_running);
			thiz.m_running = false;
			return;
		}

		#pragma endregion

		#pragma region callback

		virtual auto callback (
			std::vector<std::string> const & argument
		) -> std::vector<std::string> override {
			assert_condition(thiz.m_running);
			auto result = std::vector<std::string>{};
			auto method = argument[0];
			switch (hash_string(method)) {
				case hash_string("name"sv) : {
					auto name = thiz.name();
					result.emplace_back(std::move(name));
					break;
				}
				case hash_string("version"sv) : {
					auto version = thiz.version();
					result.emplace_back(std::to_string(version));
					break;
				}
				case hash_string("system"sv) : {
					auto system = thiz.system();
					result.emplace_back(std::move(system));
					break;
				}
				case hash_string("output"sv) : {
					auto text = argument[1];
					thiz.output(text);
					break;
				}
				case hash_string("input"sv) : {
					auto text = thiz.input();
					result.emplace_back(std::move(text));
					break;
				}
					#if defined M_system_windows
				case hash_string("open_file_dialog"sv) : {
					auto pick_folder = string_to_boolean(argument[1]);
					auto multiple = string_to_boolean(argument[2]);
					auto selected_path = thiz.open_file_dialog(pick_folder, multiple);
					result.insert(result.end(), std::make_move_iterator(selected_path.begin()), std::make_move_iterator(selected_path.end()));
					break;
				}
					#endif
				default : {
					throw std::runtime_error{"invalid method"s};
				}
			}
			return result;
		}

		#pragma endregion

		#pragma region callback implement

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
			return Shell::output(text);
		}

		auto input (
		) -> std::string {
			return Shell::input();
		}

		// ----------------

		#if defined M_system_windows

		// NOTE : if arch == arm_32 : compile failed : undefined symbol "CoInitialize"...
		auto open_file_dialog (
			bool const & pick_folder,
			bool const & multiple
		) -> std::vector<std::string> {
			auto state_h = HRESULT{};
			auto result = std::vector<std::string>{};
			CoInitialize(nullptr);
			auto dialog = X<IFileOpenDialog *>{nullptr};
			state_h = CoCreateInstance(__uuidof(FileOpenDialog), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));
			assert_condition(state_h == S_OK);
			auto option = FILEOPENDIALOGOPTIONS{};
			state_h = dialog->GetOptions(&option);
			assert_condition(state_h == S_OK);
			option |= FOS_NOCHANGEDIR | FOS_FORCEFILESYSTEM | FOS_NODEREFERENCELINKS | FOS_DONTADDTORECENT | FOS_FORCESHOWHIDDEN;
			if (pick_folder) {
				option |= FOS_PICKFOLDERS;
			}
			if (multiple) {
				option |= FOS_ALLOWMULTISELECT;
			}
			state_h = dialog->SetOptions(option);
			assert_condition(state_h == S_OK);
			state_h = dialog->Show(nullptr);
			if (state_h != HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
				assert_condition(state_h == S_OK);
				auto selected_item_list = X<IShellItemArray *>{nullptr};
				state_h = dialog->GetResults(&selected_item_list);
				assert_condition(state_h == S_OK);
				auto count = DWORD{0};
				state_h = selected_item_list->GetCount(&count);
				assert_condition(state_h == S_OK);
				result.reserve(count);
				for (auto index = DWORD{0}; index < count; index++) {
					auto item = X<IShellItem *>{nullptr};
					auto display_name = LPWSTR{nullptr};
					state_h = selected_item_list->GetItemAt(index, &item);
					assert_condition(state_h == S_OK);
					state_h = item->GetDisplayName(SIGDN_FILESYSPATH, &display_name);
					assert_condition(state_h == S_OK);
					auto display_name_8 = utf16_to_utf8(std::u16string_view{reinterpret_cast<char16_t const *>(display_name)});
					result.emplace_back(std::move(reinterpret_cast<std::string &>(display_name_8)));
					CoTaskMemFree(display_name);
					item->Release();
				}
				selected_item_list->Release();
			}
			dialog->Release();
			return result;
		}

		#endif

		#pragma endregion

	};

	#pragma endregion

}
