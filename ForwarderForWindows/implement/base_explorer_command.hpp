#pragma once

#include "implement/common.hpp"
#include "implement/language.hpp"

namespace TwinStar::ForwarderForWindows {

	#pragma region type

	class BaseExplorerCommand :
		public WRL::RuntimeClass<WRL::RuntimeClassFlags<WRL::RuntimeClassType::ClassicCom>, IExplorerCommand, IObjectWithSite> {

	protected:

		WRL::ComPtr<IUnknown> m_site;

	public:

		#pragma region structor

		explicit BaseExplorerCommand (
		) :
			WRL::RuntimeClass<WRL::RuntimeClassFlags<WRL::RuntimeClassType::ClassicCom>, IExplorerCommand, IObjectWithSite>{} {
		}

		#pragma endregion

		#pragma region implement

		virtual IFACEMETHODIMP GetCanonicalName (
			_Out_ GUID * pguidCommandName
		) override {
			try {
				*pguidCommandName = GUID_NULL;
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
				auto icon_string = wil::make_cotaskmem_string_nothrow(icon_data);
				RETURN_IF_NULL_ALLOC(icon_string);
				*ppszIcon = icon_string.release();
				return S_OK;
			}
			CATCH_RETURN()
		}

		virtual IFACEMETHODIMP GetTitle (
			_In_opt_ IShellItemArray *            psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR * ppszName
		) override {
			try {
				*ppszName = nullptr;
				auto title_data = thiz.title();
				if (title_data == nullptr) {
					return S_OK;
				}
				auto title_string = wil::make_cotaskmem_string_nothrow(title_data);
				RETURN_IF_NULL_ALLOC(title_string);
				*ppszName = title_string.release();
				return S_OK;
			}
			CATCH_RETURN()
		}

		virtual IFACEMETHODIMP GetToolTip (
			_In_opt_ IShellItemArray *            psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR * ppszInfotip
		) override {
			try {
				*ppszInfotip = nullptr;
				auto tip_data = thiz.icon();
				if (tip_data == nullptr) {
					return E_NOTIMPL;
				}
				auto tip_string = wil::make_cotaskmem_string_nothrow(tip_data);
				RETURN_IF_NULL_ALLOC(tip_string);
				*ppszInfotip = tip_string.release();
				return S_OK;
			}
			CATCH_RETURN()
		}

		virtual IFACEMETHODIMP EnumSubCommands (
			_COM_Outptr_ IEnumExplorerCommand ** ppEnum
		) override {
			try {
				*ppEnum = thiz.sub();
				if (*ppEnum == nullptr) {
					return E_NOTIMPL;
				}
				return S_OK;
			}
			CATCH_RETURN()
		}

		virtual IFACEMETHODIMP GetFlags (
			_Out_ EXPCMDFLAGS * pFlags
		) override {
			try {
				*pFlags = thiz.flag();
				return S_OK;
			}
			CATCH_RETURN()
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

		#pragma region interface

		virtual auto icon (
		) -> LPWSTR {
			return nullptr;
		}

		virtual auto title (
		) -> LPWSTR {
			return nullptr;
		}

		virtual auto tip (
		) -> LPWSTR {
			return nullptr;
		}

		virtual auto sub (
		) -> IEnumExplorerCommand * {
			return nullptr;
		}

		virtual auto flag (
		) -> EXPCMDFLAGS {
			return ECF_DEFAULT;
		}

		virtual auto state (
			IShellItemArray * selection
		) -> EXPCMDSTATE {
			return ECS_ENABLED;
		}

		virtual auto invoke (
			IShellItemArray * selection
		) -> void {
			return;
		}

		#pragma endregion

	};

	class BaseEnumExplorerCommand :
		public WRL::RuntimeClass<WRL::RuntimeClassFlags<WRL::RuntimeClassType::ClassicCom>, IEnumExplorerCommand> {

	protected:

		std::vector<WRL::ComPtr<IExplorerCommand>> m_command;
		std::size_t                                m_current;

	public:

		#pragma region structor

		explicit BaseEnumExplorerCommand (
		) :
			WRL::RuntimeClass<WRL::RuntimeClassFlags<WRL::RuntimeClassType::ClassicCom>, IEnumExplorerCommand>{},
			m_command{},
			m_current{} {
		}

		#pragma endregion

		#pragma region implement

		virtual IFACEMETHODIMP Next (
			ULONG                                                      celt,
			__out_ecount_part(celt, *pceltFetched) IExplorerCommand ** pUICommand,
			__out_opt ULONG *                                          pceltFetched
		) override {
			auto fetched = ULONG{std::min(celt, static_cast<ULONG>(thiz.m_command.size() - thiz.m_current))};
			if (fetched != 0) {
				thiz.m_command[thiz.m_current + fetched - 1].CopyTo(&pUICommand[0]);
				thiz.m_current += fetched;
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
			thiz.m_current = 0;
			return S_OK;
		}

		virtual IFACEMETHODIMP Clone (
			__deref_out IEnumExplorerCommand ** ppenum
		) override {
			*ppenum = nullptr;
			return E_NOTIMPL;
		}

		#pragma endregion

		#pragma region interface

		#pragma endregion

	};

	#pragma endregion

	#pragma region derived type

	class SeparatorExplorerCommand :
		public BaseExplorerCommand {

	public:

		#pragma region structor

		explicit SeparatorExplorerCommand (
		) :
			BaseExplorerCommand{} {
		}

		#pragma endregion

		#pragma region implement

		virtual auto flag (
		) -> EXPCMDFLAGS override {
			return ECF_ISSEPARATOR;
		}

		#pragma endregion

	};

	// ----------------

	template <typename TBase>
	class VisibleExplorerCommand :
		public TBase {

	protected:

		std::wstring m_id;

	public:

		#pragma region structor

		template <typename ... Argument>
		explicit VisibleExplorerCommand (
			std::wstring const & id,
			Argument && ...      argument
		) :
			TBase{std::forward<Argument>(argument) ...},
			m_id{id} {
		}

		#pragma endregion

		#pragma region implement

		virtual auto state (
			IShellItemArray * selection
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
