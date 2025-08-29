// twinning.assistant.forwarder.main

#include <unknwn.h>
#include <winrt/base.h>
#include "./common.hpp"

import twinning.assistant.forwarder.forwarder_explorer_command;
using Twinning::Assistant::Forwarder::ForwarderExplorerCommandClassFactory;

#pragma region main

STDAPI_(BOOL) DllMain (
	HINSTANCE hinstDLL,
	DWORD     fdwReason,
	LPVOID    lpvReserved
) {
	return TRUE;
}

STDAPI DllCanUnloadNow (
) {
	if (winrt::get_module_lock()) {
		return S_FALSE;
	}
	winrt::clear_factory_cache();
	return S_OK;
}

STDAPI DllGetClassObject (
	REFCLSID     rclsid,
	REFIID       riid,
	LPVOID FAR * ppv
) {
	if (ppv == nullptr) {
		return E_POINTER;
	}
	*ppv = nullptr;
	if (riid != IID_IClassFactory && riid != IID_IUnknown) {
		return E_NOINTERFACE;
	}
	try {
		if (rclsid == __uuidof(ForwarderExplorerCommandClassFactory)) {
			return winrt::make<ForwarderExplorerCommandClassFactory>()->QueryInterface(riid, ppv);
		}
		return E_INVALIDARG;
	}
	catch (...) {
		return winrt::to_hresult();
	}
}

#pragma endregion
