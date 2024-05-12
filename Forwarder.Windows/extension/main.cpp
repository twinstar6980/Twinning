//

#include "./common.hpp"
#include "./forward_explorer_command.hpp"

using namespace TwinStar::Forwarder;

#pragma region dll

auto APIENTRY DllMain (
	HMODULE hModule,
	DWORD   ul_reason_for_call,
	LPVOID  lpReserved
) -> BOOL {
	g_application_logo = get_module_file_path(hModule);
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH :
		case DLL_THREAD_ATTACH :
		case DLL_THREAD_DETACH :
		case DLL_PROCESS_DETACH :
			break;
	}
	return TRUE;
}

STDAPI DllCanUnloadNow (
) {
	return WRL::Module<WRL::InProc>::GetModule().GetObjectCount() == 0 ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject (
	_In_ REFCLSID        rclsid,
	_In_ REFIID          riid,
	_COM_Outptr_ void ** instance
) {
	return WRL::Module<WRL::InProc>::GetModule().GetClassObject(rclsid, riid, instance);
}

STDAPI DllGetActivationFactory (
	_In_ HSTRING                       activatableClassId,
	_COM_Outptr_ IActivationFactory ** factory
) {
	return WRL::Module<WRL::InProc>::GetModule().GetActivationFactory(activatableClassId, factory);
}

#pragma endregion

#pragma region class

CoCreatableClass(ForwardExplorerCommand);
CoCreatableClassWrlCreatorMapInclude(ForwardExplorerCommand);

#pragma endregion
