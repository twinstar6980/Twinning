//

#include "implement/common.hpp"
#include "implement/language.hpp"
#include "implement/forward_explorer_command_exposed.hpp"

#define M_define_co_creatable_class(_class)\
	CoCreatableClass(_class)\
	CoCreatableClassWrlCreatorMapInclude(_class)

#pragma region dll implement

auto APIENTRY DllMain (
	HMODULE hModule,
	DWORD   ul_reason_for_call,
	LPVOID  lpReserved
) -> BOOL {
	TwinStar::ForwarderForWindows::g_dll_handle = hModule;
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

#pragma region co creatable class

using namespace TwinStar::ForwarderForWindows::Exposed;

M_define_co_creatable_class(MainForwardExplorerCommand);
M_define_co_creatable_class(LaunchForwardExplorerCommand);
M_define_co_creatable_class(JSGroupForwardExplorerCommand);
M_define_co_creatable_class(JSONGroupForwardExplorerCommand);
M_define_co_creatable_class(DataGroupForwardExplorerCommand);
M_define_co_creatable_class(TextureGroupForwardExplorerCommand);
M_define_co_creatable_class(WwiseMediaGroupForwardExplorerCommand);
M_define_co_creatable_class(WwiseSoundBankGroupForwardExplorerCommand);
M_define_co_creatable_class(MarmaladeDZipGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapZLibGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapCryptDataGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapReflectionObjectNotationGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapTextureGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapUTextureGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapSexyTextureGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapAnimationGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapReAnimationGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapParticleGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapTrailGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapParticleEffectGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapRenderEffectGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapCharacterFontWidget2GroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapPackageGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapResourceStreamGroupGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapResourceStreamBundleGroupForwardExplorerCommand);
M_define_co_creatable_class(PopCapResourceStreamBundlePatchGroupForwardExplorerCommand);
M_define_co_creatable_class(PvZ2TextTableGroupForwardExplorerCommand);
M_define_co_creatable_class(PvZ2RemoteAndroidHelperGroupForwardExplorerCommand);

#pragma endregion
