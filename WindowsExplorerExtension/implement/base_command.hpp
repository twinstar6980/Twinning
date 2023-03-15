#pragma once

#include "implement/common.hpp"
#include "implement/language.hpp"

namespace TwinStar::WindowsExplorerExtension {

	#pragma region type

	class BaseCommand :
		public RuntimeClass<RuntimeClassFlags<ClassicCom>, IExplorerCommand, IObjectWithSite> {

	protected:

		ComPtr<IUnknown> m_site;

	public:

		#pragma region implement

		virtual IFACEMETHODIMP GetTitle (
			_In_opt_ IShellItemArray *            psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR * ppszName
		) override {
			try {
				*ppszName = nullptr;
				auto title_data = thiz.title();
				if (title_data == nullptr) {
					return S_FALSE;
				}
				auto title_s = wil::make_cotaskmem_string_nothrow(title_data);
				RETURN_IF_NULL_ALLOC(title_s);
				*ppszName = title_s.release();
				return S_OK;
			}
			CATCH_RETURN()
		}

		virtual IFACEMETHODIMP GetIcon (
			_In_opt_ IShellItemArray *            psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR * ppszIcon
		) override {
			try {
				*ppszIcon = nullptr;
				auto icon_data = thiz.icon();
				if (icon_data == nullptr) {
					return E_NOTIMPL;
				}
				auto icon_s = wil::make_cotaskmem_string_nothrow(icon_data);
				RETURN_IF_NULL_ALLOC(icon_s);
				*ppszIcon = icon_s.release();
				return S_OK;
			}
			CATCH_RETURN()
		}

		virtual IFACEMETHODIMP GetToolTip (
			_In_opt_ IShellItemArray *            psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR * ppszInfotip
		) override {
			*ppszInfotip = nullptr;
			return E_NOTIMPL;
		}

		virtual IFACEMETHODIMP GetCanonicalName (
			_Out_ GUID * pguidCommandName
		) override {
			*pguidCommandName = GUID_NULL;
			return S_OK;
		}

		virtual IFACEMETHODIMP GetState (
			_In_opt_ IShellItemArray * psiItemArray,
			_In_ BOOL                  fOkToBeSlow,
			_Out_ EXPCMDSTATE *        pCmdState
		) override {
			try {
				*pCmdState = thiz.state(psiItemArray);
				return S_OK;
			}
			CATCH_RETURN()
		}

		virtual IFACEMETHODIMP Invoke (
			_In_opt_ IShellItemArray * psiItemArray,
			_In_opt_ IBindCtx *        pbc
		) override {
			try {
				thiz.invoke(psiItemArray);
				return S_OK;
			}
			CATCH_RETURN()
		}

		virtual IFACEMETHODIMP GetFlags (
			_Out_ EXPCMDFLAGS * pFlags
		) override {
			try {
				*pFlags = thiz.flags();
				return S_OK;
			}
			CATCH_RETURN()
		}

		virtual IFACEMETHODIMP EnumSubCommands (
			_COM_Outptr_ IEnumExplorerCommand ** ppEnum
		) override {
			*ppEnum = nullptr;
			return E_NOTIMPL;
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

		#pragma region utility for implement

		virtual auto title (
		) -> LPCWSTR {
			return nullptr;
		}

		virtual auto icon (
		) -> LPCWSTR {
			return nullptr;
		}

		virtual auto state (
			_In_opt_ IShellItemArray * selection
		) -> EXPCMDSTATE {
			return ECS_ENABLED;
		}

		virtual auto flags (
		) -> EXPCMDFLAGS {
			return ECF_DEFAULT;
		}

		virtual auto invoke (
			_In_opt_ IShellItemArray * selection
		) -> void {
			return;
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region derived type

	class SeparatorCommand :
		public BaseCommand {

	public:

		#pragma region implement

		virtual auto flags (
		) -> EXPCMDFLAGS override {
			return ECF_ISSEPARATOR;
		}

		#pragma endregion

	};

	// ----------------

	template <typename TBase>
	class VisibleCommand :
		public TBase {

	protected:

		std::wstring m_id;

	public:

		#pragma region structor

		template <typename ... Argument>
		explicit VisibleCommand (
			std::wstring const & id,
			Argument && ...      argument
		):
			TBase{std::forward<Argument>(argument) ...},
			m_id{id} {
		}

		#pragma endregion

		#pragma region implement

		virtual auto state (
			_In_opt_ IShellItemArray * selection
		) -> EXPCMDSTATE override {
			auto visible = get_register_value_dword(k_register_key_parent, k_register_key_path, std::format(L"visible_{}", thiz.m_id)).value_or(0) != 0;
			if (!visible) {
				return ECS_HIDDEN;
			} else {
				return TBase::state(selection);
			}
		}

		#pragma endregion

	};

	#pragma endregion

}
