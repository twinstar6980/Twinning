module;

#include <Shlwapi.h>
#include <ShlObj_core.h>
#include <Windows.h>
#include <winrt/base.h>
#include <winrt/windows.applicationmodel.h>
#include "./common.hpp"

#define M_forwarder_explorer_command_class_factory_uuid "BE4A1760-1939-4240-BB82-7199B184B702"

export module twinning.assistant_plus.forwarder.forwarder_explorer_command;

extern "C" IMAGE_DOS_HEADER __ImageBase;

export namespace Twinning::AssistantPlus::Forwarder {

	#pragma region type

	class ForwarderExplorerCommand :
		public winrt::implements<ForwarderExplorerCommand, IExplorerCommand> {

	public:

		#pragma region constructor

		explicit ForwarderExplorerCommand(
		) :
			winrt::implements<ForwarderExplorerCommand, IExplorerCommand>{} {
			return;
		}

		#pragma endregion

		#pragma region implement IExplorerCommand

		// ReSharper disable CppInconsistentNaming CppEnforceFunctionDeclarationStyle

		virtual IFACEMETHODIMP GetTitle(
			IShellItemArray * psiItemArray,
			LPWSTR *          ppszName
		) override {
			if (!SUCCEEDED(SHStrDupW(winrt::to_hstring(thiz.query_application_name()).data(), ppszName))) {
				return S_FALSE;
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP GetIcon(
			IShellItemArray * psiItemArray,
			LPWSTR *          ppszIcon
		) override {
			if (!SUCCEEDED(SHStrDupW(winrt::to_hstring(thiz.query_application_logo()).data(), ppszIcon))) {
				return S_FALSE;
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP GetToolTip(
			IShellItemArray * psiItemArray,
			LPWSTR *          ppszInfotip
		) override {
			*ppszInfotip = nullptr;
			return E_NOTIMPL;
		}

		virtual IFACEMETHODIMP GetCanonicalName(
			GUID * pguidCommandName
		) override {
			*pguidCommandName = GUID_NULL;
			return E_NOTIMPL;
		}

		virtual IFACEMETHODIMP GetState(
			IShellItemArray * psiItemArray,
			BOOL              fOkToBeSlow,
			EXPCMDSTATE *     pCmdState
		) override {
			*pCmdState = ECS_HIDDEN;
			auto state_file = std::format("{}\\{}\\forwarder", thiz.query_known_folder_path(FOLDERID_RoamingAppData), thiz.query_application_identifier());
			if (std::filesystem::exists(std::filesystem::path{reinterpret_cast<std::u8string const &>(state_file)})) {
				*pCmdState = ECS_ENABLED;
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP Invoke(
			IShellItemArray * psiItemArray,
			IBindCtx *        pbc
		) override {
			try {
				auto state_h = HRESULT{};
				if (psiItemArray == nullptr) {
					throw std::runtime_error{"Exception: selection item is null"};
				}
				auto resource = std::vector<std::string>{};
				auto item_count = DWORD{};
				state_h = psiItemArray->GetCount(&item_count);
				winrt::check_hresult(state_h);
				resource.reserve(item_count);
				for (auto item_index = DWORD{0}; item_index < item_count; ++item_index) {
					auto item = winrt::com_ptr<IShellItem>{};
					state_h = psiItemArray->GetItemAt(item_index, item.put());
					winrt::check_hresult(state_h);
					auto item_path = thiz.resolve_shell_item_path(item);
					auto item_path_long = thiz.resolve_storage_long_path(item_path);
					resource.emplace_back(item_path_long);
				}
				thiz.forward_resource(resource);
			}
			catch (...) {
				thiz.show_exception(thiz.parse_current_exception());
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP GetFlags(
			EXPCMDFLAGS * pFlags
		) override {
			*pFlags = ECF_DEFAULT;
			return S_OK;
		}

		virtual IFACEMETHODIMP EnumSubCommands(
			IEnumExplorerCommand * * ppEnum
		) override {
			*ppEnum = nullptr;
			return E_NOTIMPL;
		}

		// ReSharper restore CppInconsistentNaming CppEnforceFunctionDeclarationStyle

		#pragma endregion

	private:

		#pragma region utility

		auto query_application_identifier(
		) const -> std::string {
			return winrt::to_string(winrt::Windows::ApplicationModel::Package::Current().Id().Name());
		}

		auto query_application_name(
		) const -> std::string {
			return winrt::to_string(winrt::Windows::ApplicationModel::Package::Current().DisplayName());
		}

		auto query_application_logo(
		) const -> std::string {
			return std::format("{},0", thiz.query_module_file_path(reinterpret_cast<HMODULE>(&__ImageBase)));
		}

		// ----------------

		auto parse_current_exception(
		) const -> std::string {
			auto exception = std::current_exception();
			auto message = std::string{};
			try {
				std::rethrow_exception(exception);
			}
			catch (std::exception & e) {
				message = e.what();
			}
			catch (winrt::hresult_error & e) {
				message = winrt::to_string(e.message());
			}
			catch (...) {
				message = "UnknownException";
			}
			return message;
		}

		auto query_known_folder_path(
			KNOWNFOLDERID const & type
		) const -> std::string {
			auto state_h = HRESULT{};
			auto path_p = LPWSTR{};
			state_h = SHGetKnownFolderPath(type, 0, nullptr, &path_p);
			winrt::check_hresult(state_h);
			auto path_h = std::wstring{path_p};
			CoTaskMemFree(path_p);
			return winrt::to_string(path_h);
		}

		auto query_module_file_path(
			HMODULE const & handle
		) const -> std::string {
			auto state_d = DWORD{};
			auto path_data = std::vector<wchar_t>{};
			path_data.reserve(256);
			while (true) {
				state_d = GetModuleFileNameW(handle, path_data.data(), static_cast<DWORD>(path_data.capacity()));
				if (state_d != 0 && state_d != path_data.capacity()) {
					break;
				}
				assert_test(GetLastError() == ERROR_INSUFFICIENT_BUFFER);
				path_data.reserve(path_data.capacity() * 2);
			}
			auto path_h = std::wstring{path_data.data(), state_d};
			return winrt::to_string(path_h);
		}

		auto resolve_storage_long_path(
			std::string const & source
		) const -> std::string {
			auto state_d = DWORD{};
			auto source_h = winrt::to_hstring(source);
			auto destination_h = std::wstring{};
			if (!source.empty()) {
				state_d = GetLongPathNameW(source_h.data(), destination_h.data(), 0);
				assert_test(state_d != 0);
				destination_h.resize(state_d);
				state_d = GetLongPathNameW(source_h.data(), destination_h.data(), static_cast<DWORD>(destination_h.size()));
				assert_test(state_d == destination_h.size() - 1);
				destination_h.resize(destination_h.size() - 1);
			}
			return winrt::to_string(destination_h);
		}

		auto resolve_shell_item_path(
			winrt::com_ptr<IShellItem> const & item
		) const -> std::string {
			auto state_h = HRESULT{};
			auto item_display = LPWSTR{};
			state_h = item->GetDisplayName(SIGDN_FILESYSPATH, &item_display);
			winrt::check_hresult(state_h);
			auto item_display_h = std::wstring{item_display};
			CoTaskMemFree(item_display);
			return winrt::to_string(item_display_h);
		}

		auto encode_percent_string(
			std::string const & source
		) const -> std::string {
			auto destination = std::string{};
			destination.reserve(source.size() * 3);
			for (auto & character : source) {
				if (('0' <= character && character <= '9') ||
					('a' <= character && character <= 'z') ||
					('A' <= character && character <= 'Z') ||
					(character == '-') ||
					(character == '.') ||
					(character == '_') ||
					(character == '~')) {
					destination.push_back(character);
				}
				else {
					auto high_digit = static_cast<std::uint8_t>(character) / 0x10u;
					auto low_digit = static_cast<std::uint8_t>(character) % 0x10u;
					destination.push_back('%');
					destination.push_back(static_cast<char>((high_digit < 0xAu) ? ('0' + high_digit) : ('A' + high_digit - 0xAu)));
					destination.push_back(static_cast<char>((low_digit < 0xAu) ? ('0' + low_digit) : ('A' + low_digit - 0xAu)));
				}
			}
			return destination;
		}

		auto open_link(
			std::string const & link
		) const -> void {
			auto state_hi = HINSTANCE{};
			auto link_h = winrt::to_hstring(link);
			state_hi = ShellExecuteW(nullptr, L"open", link_h.data(), nullptr, nullptr, SW_SHOWNORMAL);
			assert_test(reinterpret_cast<INT_PTR>(state_hi) > 32);
			return;
		}

		// ----------------

		auto show_exception(
			std::string const & exception
		) const -> void {
			auto title_h = winrt::to_hstring(thiz.query_application_name());
			auto message_h = winrt::to_hstring(exception);
			auto original_thread_dpi_awareness_context = GetThreadDpiAwarenessContext();
			SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
			TaskDialog(nullptr, nullptr, title_h.data(), L"Exception", message_h.data(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, nullptr);
			SetThreadDpiAwarenessContext(original_thread_dpi_awareness_context);
			return;
		}

		auto forward_resource(
			std::vector<std::string> const & resource
		) const -> void {
			auto command = std::vector<std::string>{};
			command.emplace_back("-forward");
			command.append_range(resource);
			auto link = std::format(
				"{}:/application?{}",
				thiz.query_application_identifier(),
				command
				| std::views::transform(
					[&](auto & item) {
						return std::format("command={}", thiz.encode_percent_string(item));
					}
				)
				| std::views::join_with('&')
				| std::ranges::to<std::string>()
			);
			thiz.open_link(link);
			return;
		}

		#pragma endregion

	};

	// ----------------

	class __declspec(uuid(M_forwarder_explorer_command_class_factory_uuid)) ForwarderExplorerCommandClassFactory :
		public winrt::implements<ForwarderExplorerCommandClassFactory, IClassFactory> {

	public:

		#pragma region constructor

		explicit ForwarderExplorerCommandClassFactory(
		) :
			winrt::implements<ForwarderExplorerCommandClassFactory, IClassFactory>{} {
			return;
		}

		#pragma endregion

		#pragma region implement IClassFactory

		// ReSharper disable CppInconsistentNaming CppEnforceFunctionDeclarationStyle

		virtual IFACEMETHODIMP CreateInstance(
			IUnknown * pUnkOuter,
			REFIID     riid,
			void * *   ppvObject
		) override {
			try {
				return winrt::make<ForwarderExplorerCommand>()->QueryInterface(riid, ppvObject);
			}
			catch (...) {
				return winrt::to_hresult();
			}
		}

		virtual IFACEMETHODIMP LockServer(
			BOOL fLock
		) override {
			if (fLock) {
				++winrt::get_module_lock();
			}
			else {
				--winrt::get_module_lock();
			}
			return S_OK;
		}

		// ReSharper restore CppInconsistentNaming CppEnforceFunctionDeclarationStyle

		#pragma endregion

	};

	#pragma endregion

}
