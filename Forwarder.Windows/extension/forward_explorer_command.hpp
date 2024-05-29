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

		#pragma region implement - IExplorerCommand

		virtual IFACEMETHODIMP GetTitle (
			IShellItemArray * psiItemArray,
			LPWSTR *          ppszName
		) override {
			if (SHStrDupW(g_application_name.data(), ppszName) != S_OK) {
				return S_FALSE;
			}
			return S_OK;
		}

		virtual IFACEMETHODIMP GetIcon (
			IShellItemArray * psiItemArray,
			LPWSTR *          ppszIcon
		) override {
			if (SHStrDupW(g_application_logo.data(), ppszIcon) != S_OK) {
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
			*pCmdState = ECS_ENABLED;
			return S_OK;
		}

		virtual IFACEMETHODIMP Invoke (
			IShellItemArray * psiItemArray,
			IBindCtx *        pbc
		) override {
			try {
				auto resource = get_shell_item_file_system_path(psiItemArray);
				thiz.forward_resource(resource);
			}
			catch (std::exception & e) {
				MessageBoxA(nullptr, std::format("{} : {}", typeid(e).name(), e.what()).c_str(), "Exception", MB_ICONERROR | MB_OK);
			}
			catch (...) {
				MessageBoxA(nullptr, "?", "Exception", MB_ICONERROR | MB_OK);
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
			REFIID   riid,
			void * * ppvSite
		) override {
			return thiz.m_site.CopyTo(riid, ppvSite);
		}

		#pragma endregion

		#pragma region utility

		auto forward_resource (
			std::vector<std::wstring> const & resource
		) -> void {
			auto script = get_roaming_app_data_directory_path() + L"\\TwinStar.Twinning.Forwarder\\forward.cmd";
			assert_test(std::filesystem::is_regular_file(script));
			auto program = std::wstring{L"C:\\Windows\\System32\\cmd.exe"};
			auto argument = std::vector<std::wstring>{};
			argument.emplace_back(L"/C");
			argument.emplace_back(script);
			argument.append_range(resource);
			spawn_child_process(program, argument);
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
