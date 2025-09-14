module;

#include <Shlwapi.h>
#include <ShlObj_core.h>
#include <winrt/base.h>
#include "./common.hpp"

#define ForwarderExplorerCommandClassFactory_UUID "BE4A1760-1939-4240-BB82-7199B184B702"

export module twinning.assistant_plus.forwarder.forwarder_explorer_command;

extern "C" IMAGE_DOS_HEADER __ImageBase;

export namespace Twinning::AssistantPlus::Forwarder {

	#pragma region type

	class ForwarderExplorerCommand :
		public winrt::implements<ForwarderExplorerCommand, IExplorerCommand> {

	private:

		std::wstring m_application_name;

		std::wstring m_application_logo;

		std::wstring m_state_file;

	public:

		#pragma region constructor

		explicit ForwarderExplorerCommand (
		) :
			winrt::implements<ForwarderExplorerCommand, IExplorerCommand>{},
			m_application_name{},
			m_application_logo{},
			m_state_file{} {
			thiz.m_application_name = L"Twinning Assistant Plus";
			thiz.m_application_logo = thiz.get_library_file_path();
			thiz.m_state_file = thiz.get_data_directory_path() + L"\\TwinStar.Twinning.AssistantPlus\\Forwarder";
			return;
		}

		#pragma endregion

		#pragma region implement IExplorerCommand

		virtual IFACEMETHODIMP GetTitle (
			IShellItemArray * psiItemArray,
			LPWSTR *          ppszName
		) override {
			if (SHStrDupW(thiz.m_application_name.data(), ppszName) != S_OK) {
				return S_FALSE;
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP GetIcon (
			IShellItemArray * psiItemArray,
			LPWSTR *          ppszIcon
		) override {
			if (SHStrDupW(thiz.m_application_logo.data(), ppszIcon) != S_OK) {
				return S_FALSE;
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP GetToolTip (
			IShellItemArray * psiItemArray,
			LPWSTR *          ppszInfotip
		) override {
			*ppszInfotip = nullptr;
			return E_NOTIMPL;
		}

		virtual IFACEMETHODIMP GetCanonicalName (
			GUID * pguidCommandName
		) override {
			*pguidCommandName = GUID_NULL;
			return E_NOTIMPL;
		}

		virtual IFACEMETHODIMP GetState (
			IShellItemArray * psiItemArray,
			BOOL              fOkToBeSlow,
			EXPCMDSTATE *     pCmdState
		) override {
			*pCmdState = ECS_HIDDEN;
			if (std::filesystem::exists(thiz.m_state_file)) {
				*pCmdState = ECS_ENABLED;
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP Invoke (
			IShellItemArray * psiItemArray,
			IBindCtx *        pbc
		) override {
			try {
				auto state_h = HRESULT{};
				if (psiItemArray == nullptr) {
					throw std::runtime_error{"selection item is null"};
				}
				auto resource = std::vector<std::wstring>{};
				auto item_count = DWORD{};
				state_h = psiItemArray->GetCount(&item_count);
				assert_test(state_h == S_OK);
				resource.reserve(item_count);
				for (auto item_index = DWORD{0}; item_index < item_count; ++item_index) {
					auto item = winrt::com_ptr<IShellItem>{};
					state_h = psiItemArray->GetItemAt(item_index, item.put());
					assert_test(state_h == S_OK);
					auto item_name = LPWSTR{};
					state_h = item->GetDisplayName(SIGDN_FILESYSPATH, &item_name);
					if (state_h != S_OK) {
						throw std::runtime_error{"selection item is not file-system object"};
					}
					resource.emplace_back(thiz.get_file_long_path(std::wstring{item_name}));
					std::replace(resource.back().begin(), resource.back().end(), L'\\', L'/');
					CoTaskMemFree(item_name);
				}
				thiz.forward_resource(resource);
			}
			catch (...) {
				thiz.show_exception(std::current_exception());
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP GetFlags (
			EXPCMDFLAGS * pFlags
		) override {
			*pFlags = ECF_DEFAULT;
			return S_OK;
		}

		virtual IFACEMETHODIMP EnumSubCommands (
			IEnumExplorerCommand * * ppEnum
		) override {
			*ppEnum = nullptr;
			return E_NOTIMPL;
		}

		#pragma endregion

	private:

		#pragma region utility

		auto get_library_file_path (
		) -> std::wstring {
			auto state_d = DWORD{};
			auto handle = reinterpret_cast<HMODULE>(&__ImageBase);
			auto result = std::wstring{};
			auto result_data = std::vector<wchar_t>{};
			result_data.reserve(256);
			while (true) {
				state_d = GetModuleFileNameW(handle, result_data.data(), static_cast<DWORD>(result_data.capacity()));
				if (state_d != 0 && state_d != result_data.capacity()) {
					break;
				}
				assert_test(GetLastError() == ERROR_INSUFFICIENT_BUFFER);
				result_data.reserve(result_data.capacity() * 2);
			}
			result = std::wstring{result_data.data(), state_d};
			return result;
		}

		auto get_data_directory_path (
		) -> std::wstring {
			auto state_h = DWORD{};
			auto result = std::wstring{};
			auto result_data = LPWSTR{};
			state_h = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &result_data);
			assert_test(state_h == S_OK);
			result = std::wstring{result_data};
			CoTaskMemFree(result_data);
			return result;
		}

		auto get_file_long_path (
			std::wstring const & source
		) -> std::wstring {
			auto state_d = DWORD{};
			auto destination = std::wstring{};
			if (!source.empty()) {
				state_d = GetLongPathNameW(source.data(), destination.data(), 0);
				assert_test(state_d != 0);
				destination.resize(state_d);
				state_d = GetLongPathNameW(source.data(), destination.data(), static_cast<DWORD>(destination.size()));
				assert_test(state_d == destination.size() - 1);
				destination.resize(destination.size() - 1);
			}
			return destination;
		}

		auto encode_percent_string (
			std::wstring const & source
		) -> std::wstring {
			auto data = winrt::to_string(source);
			auto destination = std::wstring{};
			destination.reserve(data.size() * 3);
			for (auto & character : data) {
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

		// ----------------

		auto open_link (
			std::wstring const & link
		) -> void {
			auto state_hi = HINSTANCE{};
			state_hi = ShellExecuteW(nullptr, L"open", link.data(), nullptr, nullptr, SW_SHOWNORMAL);
			assert_test(reinterpret_cast<INT_PTR>(state_hi) > 32);
			return;
		}

		// ----------------

		auto forward_resource (
			std::vector<std::wstring> const & resource
		) -> void {
			auto command = std::vector<std::wstring>{};
			command.emplace_back(L"-Forward");
			command.append_range(resource);
			auto link = std::wstring{};
			link.reserve(1024);
			link += L"twinstar.twinning.assistant-plus:/Application?";
			for (auto & item : command) {
				link += L"Command=";
				link += thiz.encode_percent_string(item);
				link += L"&";
			}
			if (!command.empty()) {
				link.pop_back();
			}
			thiz.open_link(link);
			return;
		}

		auto show_exception (
			std::exception_ptr const & exception
		) -> void {
			auto message = std::string{};
			try {
				std::rethrow_exception(exception);
			}
			catch (std::exception & e) {
				message = e.what();
			}
			catch (...) {
				message = "UnknownException";
			}
			auto message_h = winrt::to_hstring(message);
			auto original_thread_dpi_awareness_context = GetThreadDpiAwarenessContext();
			SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
			TaskDialog(nullptr, nullptr, thiz.m_application_name.data(), L"Exception", message_h.data(), TDCBF_CLOSE_BUTTON, TD_ERROR_ICON, nullptr);
			SetThreadDpiAwarenessContext(original_thread_dpi_awareness_context);
			return;
		}

		#pragma endregion

	};

	// ----------------

	class __declspec(uuid(ForwarderExplorerCommandClassFactory_UUID)) ForwarderExplorerCommandClassFactory :
		public winrt::implements<ForwarderExplorerCommandClassFactory, IClassFactory> {

	public:

		#pragma region constructor

		explicit ForwarderExplorerCommandClassFactory (
		) :
			winrt::implements<ForwarderExplorerCommandClassFactory, IClassFactory>{} {
			return;
		}

		#pragma endregion

		#pragma region implement IClassFactory

		virtual IFACEMETHODIMP CreateInstance (
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

		virtual IFACEMETHODIMP LockServer (
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

		#pragma endregion

	};

	#pragma endregion

}
