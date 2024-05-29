//

#include "./common.hpp"
#include "./forward_explorer_command.hpp"

#pragma region entry

STDAPI_(BOOL) DllMain (
	HINSTANCE hinstDLL,
	DWORD     fdwReason,
	LPVOID    lpvReserved
) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		Twinning::Forwarder::g_application_logo = Twinning::Forwarder::get_module_file_path(hinstDLL);
		DisableThreadLibraryCalls(hinstDLL);
	}
	return TRUE;
}

// ----------------

STDAPI DllGetActivationFactory (
	HSTRING                activatableClassId,
	IActivationFactory * * factory
) {
	return WRL::Module<WRL::InProc>::GetModule().GetActivationFactory(activatableClassId, factory);
}

STDAPI DllGetClassObject (
	REFCLSID     rclsid,
	REFIID       riid,
	LPVOID FAR * ppv
) {
	return WRL::Module<WRL::InProc>::GetModule().GetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow (
) {
	return WRL::Module<WRL::InProc>::GetModule().Terminate() ? S_OK : S_FALSE;
}

#pragma endregion

#pragma region class

using Twinning::Forwarder::ForwardExplorerCommand;
CoCreatableClass(ForwardExplorerCommand);
CoCreatableClassWrlCreatorMapInclude(ForwardExplorerCommand);

#pragma endregion
