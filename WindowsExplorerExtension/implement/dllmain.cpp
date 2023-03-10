//

#include "implement/common.hpp"
#include "implement/language.hpp"
#include "implement/method_invoke_command_expose.hpp"

#define M_define_co_creatable_class(_class)\
	CoCreatableClass(_class)\
	CoCreatableClassWrlCreatorMapInclude(_class)

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

#pragma region co creatable class

using namespace TwinStar::WindowsExplorerExtension;

M_define_co_creatable_class(MainMethodInvokeCommand);
M_define_co_creatable_class(LaunchMethodInvokeCommand);
M_define_co_creatable_class(JSGroupMethodInvokeCommand);
M_define_co_creatable_class(JSONGroupMethodInvokeCommand);
M_define_co_creatable_class(DataGroupMethodInvokeCommand);
M_define_co_creatable_class(ImageGroupMethodInvokeCommand);
M_define_co_creatable_class(WwiseEncodedMediaGroupMethodInvokeCommand);
M_define_co_creatable_class(WwiseSoundBankGroupMethodInvokeCommand);
M_define_co_creatable_class(MarmaladeDZipGroupMethodInvokeCommand);
M_define_co_creatable_class(PopCapZLibGroupMethodInvokeCommand);
M_define_co_creatable_class(PopCapREANIMGroupMethodInvokeCommand);
M_define_co_creatable_class(PopCapPARTICLEGroupMethodInvokeCommand);
M_define_co_creatable_class(PopCapRTONGroupMethodInvokeCommand);
M_define_co_creatable_class(PopCapPTXGroupMethodInvokeCommand);
M_define_co_creatable_class(PopCapPAMGroupMethodInvokeCommand);
M_define_co_creatable_class(PopCapPAKGroupMethodInvokeCommand);
M_define_co_creatable_class(PopCapRSGPGroupMethodInvokeCommand);
M_define_co_creatable_class(PopCapRSBGroupMethodInvokeCommand);
M_define_co_creatable_class(PopCapRSBPatchGroupMethodInvokeCommand);
M_define_co_creatable_class(PvZ2LawnStringTextGroupMethodInvokeCommand);
M_define_co_creatable_class(PvZ2RemoteAndroidHelperGroupMethodInvokeCommand);

#pragma endregion
