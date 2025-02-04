//

#include <activation.h>
#include <ObjectArray.h>
#include <wrl/implements.h>
#include <wrl/module.h>
#include "./common.hpp"

import twinning.assistant.forwarder.forwarder_explorer_command;

// ----------------

STDAPI_(BOOL) DllMain (
	HINSTANCE hinstDLL,
	DWORD     fdwReason,
	LPVOID    lpvReserved
) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinstDLL);
	}
	return TRUE;
}

STDAPI DllGetActivationFactory (
	HSTRING                activatableClassId,
	IActivationFactory * * factory
) {
	return Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule().GetActivationFactory(activatableClassId, factory);
}

STDAPI DllGetClassObject (
	REFCLSID     rclsid,
	REFIID       riid,
	LPVOID FAR * ppv
) {
	return Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule().GetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow (
) {
	return Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule().Terminate() ? S_OK : S_FALSE;
}

// ----------------

CoCreatableClass(ForwarderExplorerCommand);
CoCreatableClassWrlCreatorMapInclude(ForwarderExplorerCommand);
