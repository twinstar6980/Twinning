#pragma once

#include "./common.hpp"

namespace Twinning::Forwarder {

	#pragma region implement

	class __declspec(uuid("9992EC48-22A5-86FA-EA42-72DA1A53F23D")) ForwardExplorerCommand :
		public WRL::RuntimeClass<WRL::RuntimeClassFlags<WRL::RuntimeClassType::ClassicCom>, IExplorerCommand, IObjectWithSite> {

	protected:

		WRL::ComPtr<IUnknown> m_site;

	public:

		#pragma region structor

		explicit ForwardExplorerCommand (
		) :
			WRL::RuntimeClass<WRL::RuntimeClassFlags<WRL::RuntimeClassType::ClassicCom>, IExplorerCommand, IObjectWithSite>{},
			m_site{} {
		}

		#pragma endregion

		#pragma region implement

		virtual IFACEMETHODIMP GetCanonicalName (
			_Out_ GUID * pguidCommandName
		) override {
			*pguidCommandName = GUID_NULL;
			return S_OK;
		}

		virtual IFACEMETHODIMP GetIcon (
			_In_opt_ IShellItemArray *            psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR * ppszIcon
		) override {
			if (SHStrDupW(g_application_logo.data(), ppszIcon) != S_OK) {
				return S_FALSE;
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP GetTitle (
			_In_opt_ IShellItemArray *            psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR * ppszName
		) override {
			if (SHStrDupW(g_application_name.data(), ppszName) != S_OK) {
				return S_FALSE;
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP GetToolTip (
			_In_opt_ IShellItemArray *            psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR * ppszInfotip
		) override {
			*ppszInfotip = nullptr;
			return E_NOTIMPL;
		}

		virtual IFACEMETHODIMP EnumSubCommands (
			_COM_Outptr_ IEnumExplorerCommand ** ppEnum
		) override {
			ppEnum = nullptr;
			return E_NOTIMPL;
		}

		virtual IFACEMETHODIMP GetFlags (
			_Out_ EXPCMDFLAGS * pFlags
		) override {
			*pFlags = ECF_DEFAULT;
			return S_OK;
		}

		virtual IFACEMETHODIMP GetState (
			_In_opt_ IShellItemArray * psiItemArray,
			_In_ BOOL                  fOkToBeSlow,
			_Out_ EXPCMDSTATE *        pCmdState
		) override {
			*pCmdState = ECS_ENABLED;
			return S_OK;
		}

		virtual IFACEMETHODIMP Invoke (
			_In_opt_ IShellItemArray * psiItemArray,
			_In_opt_ IBindCtx *        pbc
		) override {
			try {
				auto script = get_roaming_app_data_directory_path() + L"\\TwinStar.Twinning.Forwarder\\forward.cmd";
				assert_test(std::filesystem::is_regular_file(script));
				auto program = std::wstring{L"C:\\Windows\\System32\\cmd.exe"};
				auto argument = std::vector<std::wstring>{};
				argument.emplace_back(L"/C");
				argument.emplace_back(script);
				argument.append_range(get_shell_item_file_path(psiItemArray));
				create_process(program, argument);
			}
			catch (std::exception & e) {
				MessageBoxA(nullptr, std::format("{} : {}", typeid(e).name(), e.what()).c_str(), "Exception", MB_ICONERROR | MB_OK);
			}
			catch (...) {
				MessageBoxA(nullptr, "?", "Exception", MB_ICONERROR | MB_OK);
			}
			return S_OK;
		}

		// ----------------

		virtual IFACEMETHODIMP SetSite (
			_In_ IUnknown * pUnkSite
		) override {
			thiz.m_site = pUnkSite;
			return S_OK;
		}

		virtual IFACEMETHODIMP GetSite (
			_In_ REFIID          riid,
			_COM_Outptr_ void ** ppvSite
		) override {
			return thiz.m_site.CopyTo(riid, ppvSite);
		}

		#pragma endregion

	};

	#pragma endregion

}
