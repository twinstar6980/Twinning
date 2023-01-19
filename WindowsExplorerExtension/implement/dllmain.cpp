//

#include "implement/common.hpp"
#include "implement/method_invoke_command_expose.hpp"

#pragma region dll implement

auto APIENTRY DllMain (
	HMODULE hModule,
	DWORD   ul_reason_for_call,
	LPVOID  lpReserved
) -> BOOL {
	TwinStar::WindowsExplorerExtension::g_dll_handle = hModule;
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
	return Module<InProc>::GetModule().GetObjectCount() == 0 ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject (
	_In_ REFCLSID        rclsid,
	_In_ REFIID          riid,
	_COM_Outptr_ void ** instance
) {
	return Module<InProc>::GetModule().GetClassObject(rclsid, riid, instance);
}

STDAPI DllGetActivationFactory (
	_In_ HSTRING                       activatableClassId,
	_COM_Outptr_ IActivationFactory ** factory
) {
	return Module<InProc>::GetModule().GetActivationFactory(activatableClassId, factory);
}

#pragma endregion

#pragma region creatable class

using namespace TwinStar::WindowsExplorerExtension;

#define M_CoCreatableClass_X(_class)\
	CoCreatableClass(_class)\
	CoCreatableClassWrlCreatorMapInclude(_class)

M_CoCreatableClass_X(LaunchMethodInvokeCommand);
M_CoCreatableClass_X(JSEvaluateMethodInvokeCommand);
M_CoCreatableClass_X(JSONMethodInvokeGroupCommand);
M_CoCreatableClass_X(DataMethodInvokeGroupCommand);
M_CoCreatableClass_X(ImageAtlasMethodInvokeGroupCommand);
M_CoCreatableClass_X(WwiseEncodedMediaMethodInvokeGroupCommand);
M_CoCreatableClass_X(WwiseSoundBankMethodInvokeGroupCommand);
M_CoCreatableClass_X(MarmaladeDZipMethodInvokeGroupCommand);
M_CoCreatableClass_X(PopCapZLibMethodInvokeGroupCommand);
M_CoCreatableClass_X(PopCapReanimMethodInvokeGroupCommand);
M_CoCreatableClass_X(PopCapRTONMethodInvokeGroupCommand);
M_CoCreatableClass_X(PopCapPTXMethodInvokeGroupCommand);
M_CoCreatableClass_X(PopCapPAMMethodInvokeGroupCommand);
M_CoCreatableClass_X(PopCapPAKMethodInvokeGroupCommand);
M_CoCreatableClass_X(PopCapRSGPMethodInvokeGroupCommand);
M_CoCreatableClass_X(PopCapRSBMethodInvokeGroupCommand);
M_CoCreatableClass_X(PvZ2LawnStringTextMethodInvokeGroupCommand);
M_CoCreatableClass_X(PvZ2RemoteAndroidHelperMethodInvokeGroupCommand);

#pragma endregion
