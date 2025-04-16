module;

#include <typeinfo>
#include <Shlwapi.h>
#include <ShlObj_core.h>
#include <wrl/implements.h>
#include "./common.hpp"

export module twinning.assistant_plus.forwarder.forwarder_explorer_command;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

export {

	class __declspec(uuid("BE4A1760-1939-4240-BB82-7199B184B702")) ForwarderExplorerCommand :
		public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::ClassicCom>, IExplorerCommand, IObjectWithSite> {

	private:

		Microsoft::WRL::ComPtr<IUnknown> m_site;

		std::wstring m_application_name;

		std::wstring m_application_logo;

		std::wstring m_state_file;

	public:

		#pragma region structor

		explicit ForwarderExplorerCommand (
		) :
			Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::ClassicCom>, IExplorerCommand, IObjectWithSite>{},
			m_site{},
			m_application_name{},
			m_application_logo{},
			m_state_file{} {
			thiz.m_application_name = L"Twinning Assistant Plus";
			thiz.m_application_logo = thiz.get_library_file_path();
			thiz.m_state_file = thiz.get_roaming_directory_path() + L"\\TwinStar.Twinning.AssistantPlus\\Forwarder";
		}

		#pragma endregion

		#pragma region implement - IExplorerCommand

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
			return S_OK;
		}

		virtual IFACEMETHODIMP GetCanonicalName (
			GUID * pguidCommandName
		) override {
			*pguidCommandName = GUID_NULL;
			return S_OK;
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
				assert_test(psiItemArray != nullptr);
				auto resource = std::vector<std::wstring>{};
				auto resource_count = DWORD{};
				state_h = psiItemArray->GetCount(&resource_count);
				assert_test(state_h == S_OK);
				resource.reserve(resource_count);
				for (auto index = DWORD{0}; index < resource_count; ++index) {
					auto item = std::add_pointer_t<IShellItem>{};
					state_h = psiItemArray->GetItemAt(index, &item);
					assert_test(state_h == S_OK);
					auto display_name = LPWSTR{};
					state_h = item->GetDisplayName(SIGDN_FILESYSPATH, &display_name);
					assert_test(state_h == S_OK);
					resource.emplace_back(thiz.get_file_long_path(std::wstring{display_name}));
					std::replace(resource.back().begin(), resource.back().end(), L'\\', L'/');
					CoTaskMemFree(display_name);
					item->Release();
				}
				thiz.forward_resource(resource);
			}
			catch (std::exception & e) {
				thiz.show_exception(std::format("{} : {}", typeid(e).name(), e.what()));
			}
			catch (...) {
				thiz.show_exception("?");
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
			ppEnum = nullptr;
			return S_OK;
		}

		#pragma endregion

		#pragma region implement - IObjectWithSite

		virtual IFACEMETHODIMP SetSite (
			IUnknown * pUnkSite
		) override {
			thiz.m_site = pUnkSite;
			return S_OK;
		}

		virtual IFACEMETHODIMP GetSite (
			REFIID  riid,
			void ** ppvSite
		) override {
			return thiz.m_site.CopyTo(riid, ppvSite);
		}

		#pragma endregion

	private:

		#pragma region utility

		auto get_library_file_path (
		) -> std::wstring {
			auto state_d = DWORD{};
			auto handle = reinterpret_cast<HMODULE>(&__ImageBase);
			auto result = std::wstring{};
			auto result_data = std::array<wchar_t, 1024>{};
			state_d = GetModuleFileNameW(handle, result_data.data(), static_cast<DWORD>(result_data.size()));
			// NOTE : assertion failed when ERROR_INSUFFICIENT_BUFFER
			assert_test(state_d != 0 && state_d != result_data.size());
			result = std::wstring{result_data.data(), state_d};
			return result;
		}

		auto get_roaming_directory_path (
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
			auto & utf16_source = reinterpret_cast<std::u16string const &>(source);
			auto   utf8_converter = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{};
			auto   utf8_source = utf8_converter.to_bytes(
				utf16_source.data(),
				utf16_source.data() + utf16_source.size()
			);
			assert_test(utf8_converter.converted() == utf16_source.size());
			auto & data = reinterpret_cast<std::u8string &>(utf8_source);
			auto   destination = std::wstring{};
			destination.reserve(data.size() * 3);
			for (auto & character : data) {
				if ((u8'0' <= character && character <= u8'9') ||
					(u8'a' <= character && character <= u8'z') ||
					(u8'A' <= character && character <= u8'Z') ||
					(character == u8'-') ||
					(character == u8'.') ||
					(character == u8'_') ||
					(character == u8'~')) {
					destination.push_back(character);
				}
				else {
					auto high_digit = character / 0x10u;
					auto low_digit = character % 0x10u;
					destination.push_back(u8'%');
					destination.push_back(static_cast<char8_t>((high_digit < 0xAu) ? (u8'0' + high_digit) : (u8'A' + high_digit - 0xAu)));
					destination.push_back(static_cast<char8_t>((low_digit < 0xAu) ? (u8'0' + low_digit) : (u8'A' + low_digit - 0xAu)));
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
			std::string const & exception
		) -> void {
			MessageBoxA(nullptr, exception.data(), "Exception", MB_ICONERROR | MB_OK);
			return;
		}

		#pragma endregion

	};

}
